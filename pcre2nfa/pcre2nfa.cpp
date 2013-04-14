#include "stdafx.h"
#include "p2nmain.h"
#include "pcre.h"
#include "pcre2nfa.h"

#define OVECCOUNT 30 /* should be a multiple of 3 */
#define EBUFLEN 128
#define BUFLEN 1024

//使用Pcre8.32库解析单个pcre
void PcreToCode(const std::string &OnePcre, std::vector<unsigned char> &code)
{
	std::string Pcre;//pcre的具体内容
	std::string attribute;//pcre之后的修饰

	int BegPos;//pcre中第一个/出现位置
	int EndPos;//pcre中最后一个/出现位置
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
	//至此pcre的具体内容已存放至变量Pcre中

	attribute = OnePcre.substr(EndPos + 1, OnePcre.size() - EndPos - 1);
	//pcre的修饰内容已存放入变量attribute中

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

bool ExceedLimit(const std::string &OnePcre)
{
	size_t CurPos = OnePcre.find('{', 0);
	size_t EndPos;
	while (CurPos != std::string::npos)
	{
		if (OnePcre[CurPos - 1] != '\\')
		{
			EndPos = OnePcre.find('}', CurPos + 1);
			std::string StrInBrace = OnePcre.substr(CurPos + 1, EndPos - CurPos - 1);
			size_t commaPos = StrInBrace.find(',', 0);
			if (commaPos == std::string::npos)
			{
				std::stringstream ss(StrInBrace);
				size_t count = 0;
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
				size_t min = 0;
				size_t max = 0;
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

//把单个pcre转化为NFA
PCRETONFA size_t PcreToNFA(const char *pPcre, CNfa &nfa)
{
	std::vector<unsigned char> code;
	std::string strPcre(pPcre);
	PcreToCode(strPcre, code);
	std::vector<unsigned char>::iterator Beg, End;
	Beg = code.begin();
	End = code.end();
	if (!CanProcess(Beg, End))
	{
		return size_t(SC_ERROR);
	}
	Beg = code.begin();
	End = code.end();
	if (ExceedLimit(strPcre))
	{
		return SC_EXCEED;
	}
	nfa.reserve(10000);
	size_t flag = ProcessPcre(Beg, End, nfa);

	if (flag != SC_SUCCESS)
	{
		return flag;
	}
	return SC_SUCCESS;
}