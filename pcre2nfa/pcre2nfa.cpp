#include "stdafx.h"
#include "p2nmain.h"
#include "pcre.h"
#include "pcre2nfa.h"

#define OVECCOUNT 30 /* should be a multiple of 3 */
#define EBUFLEN 128
#define BUFLEN 1024

//ʹ��Pcre8.32���������pcre
void PcreToCode(const std::string &OnePcre, std::vector<unsigned char> &code)
{
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
		return;
	}

	if (!Pcre.empty() && Pcre[0] != '^')
	{
		Pcre = ".*(" + Pcre + ")";
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
		return;
	}

	unsigned int size;
	unsigned short name_table_offset;

	size = *((unsigned int*)re + 1);
	name_table_offset = *((unsigned short*)re + 12);

	for (size_t i = 0; i < size - name_table_offset; ++i)
	{
		code.push_back((unsigned char)*((unsigned char*)re + name_table_offset + i));
	}
}

//�ѵ���pcreת��ΪNFA
PCRETONFA bool PcreToNFA(const char *pPcre, CNfa &nfa)
{
	std::vector<unsigned char> code;
	std::string strPcre(pPcre);
	PcreToCode(strPcre, code);
	std::vector<unsigned char>::iterator Beg, End;
	Beg = code.begin();
	End = code.end();
	if (!CanProcess(Beg, End))
	{
		return false;
	}
	Beg = code.begin();
	End = code.end();
	nfa.reserve(10000);
	ProcessPcre(Beg, End, nfa);
	return true;
}