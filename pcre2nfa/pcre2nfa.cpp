#include "stdafx.h"
#include "p2nmain.h"
#include "pcre.h"
#include "getsig.h"
#include <hwprj/pcre2nfa.h>

#define OVECCOUNT 30 /* should be a multiple of 3 */
#define EBUFLEN 128
#define BUFLEN 1024

//ʹ��Pcre8.32���������pcre
ULONG PcreToCode(const std::string &OnePcre, std::vector<unsigned char> &code)
{
	ULONG nFromBeg = 0;
	std::string Pcre;//pcre�ľ�������
	std::string attribute;//pcre֮�������

	int BegPos;//pcre�е�һ��/����λ��
	int EndPos;//pcre�����һ��/����λ��
	BegPos = OnePcre.find("/", 0);
	EndPos = OnePcre.find_last_of("/");
	if (BegPos != -1 && EndPos != -1 && BegPos < EndPos)
	{
		Pcre = OnePcre.substr(BegPos + 1, EndPos - BegPos - 1);
	}
	else
	{
		std::cout << "Input Pcre Error!" << std::endl;
		return ULONG(-2);
	}
	if (!Pcre.empty() && Pcre[0] != '^')
	{
		nFromBeg = ULONG(-1);
		Pcre = "(" + Pcre + ")";
	}
	//����pcre�ľ��������Ѵ��������Pcre��

	attribute = OnePcre.substr(EndPos + 1, OnePcre.size() - EndPos - 1);
	//pcre�����������Ѵ�������attribute��

	const char* pattern = Pcre.c_str();
	int options = 0;
	for (std::string::iterator i = attribute.begin(); i != attribute.end(); ++i)
	{
		switch(*i)
		{
		case 's':
			options |= PCRE_DOTALL;
			break;
		case 'm':
			options |= PCRE_MULTILINE;
			break;
		case 'i':
			options |= PCRE_CASELESS;
			break;
		}
	}

	pcre *re;
	const char *error;
	int erroffset;
	re = pcre_compile(pattern, options, &error, &erroffset, NULL);
	if (re == NULL)
	{
		std::cout << pattern << std::endl;
		printf("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return ULONG(-2);
	}

	unsigned int size;
	unsigned short name_table_offset;

	size = *((unsigned int*)re + 1);
	name_table_offset = *((unsigned short*)re + 12);

	for (ULONG i = 0; i < size - name_table_offset; ++i)
	{
		code.push_back((unsigned char)*((unsigned char*)re + name_table_offset + i));
	}

	return nFromBeg;
}

bool ExceedLimit(const std::string &OnePcre)
{
	ULONG CurPos = OnePcre.find('{', 0);
	ULONG EndPos;
	while (CurPos != std::string::npos)
	{
		if (OnePcre[CurPos - 1] != '\\')
		{
			EndPos = OnePcre.find('}', CurPos + 1);
			std::string StrInBrace = OnePcre.substr(CurPos + 1, EndPos - CurPos - 1);
			ULONG commaPos = StrInBrace.find(',', 0);
			if (commaPos == std::string::npos)
			{
				std::stringstream ss(StrInBrace);
				ULONG count = 0;
				ss >> count;
				if (count > SC_LIMIT)
				{
					return true;
				}
			}
			else
			{
				std::string minstr = StrInBrace.substr(0, commaPos);
				std::string maxstr = StrInBrace.substr(commaPos + 1, StrInBrace.size() - commaPos);
				ULONG min = 0;
				ULONG max = 0;
				std::stringstream ss;
				ss << minstr;
				ss >> min;
				ss.clear();
				ss << maxstr;
				ss >> max;
				if (min > SC_LIMIT || max > SC_LIMIT)
				{
					return true;
				}
			}
		}
		CurPos = OnePcre.find('{', CurPos + 1);
	}

	return false;
}

//�ѵ���pcreת��ΪNFA
PCRE2NFA ULONG PcreToNFA(const char *pPcre, CNfa &nfa, CSignatures &sigs)
{
	std::vector<unsigned char> code;
	std::string strPcre(pPcre);
	ULONG nFromBeg = PcreToCode(strPcre, code);
	std::vector<unsigned char>::iterator Beg, End;
	Beg = code.begin();
	End = code.end();
	if (!CanProcess(Beg, End))
	{
		return ULONG(SC_ERROR);
	}
	Beg = code.begin();
	End = code.end();
	if (ExceedLimit(strPcre))
	{
		return SC_EXCEED;
	}

	if (code.size() > 0)
	{
		std::vector<std::vector<unsigned char>> strs;
		GetSignature(code, strs);

		if (strs.size() > 0)
		{
			for (ULONG i = 0; i < strs.size(); ++i)
			{
				for(std::vector<unsigned char>::iterator iter = strs[i].begin(); iter != strs[i].end(); ++iter)
				{
					if ((*iter >= 65) && (*iter <= 90))
					{
						*iter = (unsigned char)tolower(*iter);
					}
				}
			}

			for (ULONG i = 0; i < strs.size(); ++i)
			{
				for (std::vector<unsigned char>::iterator iter = strs[i].begin(); iter + 3 != strs[i].end(); ++iter)
				{
					SIGNATURE sig = *(SIGNATURE*)&(*iter);
					sigs.PushBack(sig);
				}
			}
		}
	}

	if (nFromBeg == ULONG(-1))
	{
		ULONG nCurSize = nfa.Size();
		nfa.Resize(nCurSize + 1);
		CNfaRow &row = nfa.Back();
		for (ULONG i = 0; i < EMPTY; ++i)
		{
			row.AddDest(i, nCurSize);
		}
		row.AddDest(EMPTY, nCurSize + 1);
	}
	ULONG nr = ProcessPcre(Beg, End, nfa);
	return nr;
}