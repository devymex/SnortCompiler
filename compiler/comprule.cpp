/**
**	@file		rule2nfa.cpp
**
**	@author		Lab 435, Xidian University
**
**	@brief		Support functions for transforming a rule to a nfa tree
**
**	This nfa tree processing for rule options.
**
*/

#include "stdafx.h"
#include <hwprj\ruleoption.h>
#include "pcre2nfa.h"

#include "comprule.h"

#pragma warning(disable:4996)

double rule2pcretime = 0.0;
double pcre2nfatime = 0.0;
double nfa2dfatime = 0.0;
double dfamintimetime = 0.0;

#define nfaReserve 10000

/*
**	some special symbols to be considered when construct "content" to "pcre"
*/
#define ANY '.'
#define ESCAPE '\\'
#define STAR '*'
#define PLUS '+'
#define OPT '?'
#define OPEN_RBRACKET '('
#define CLOSE_RBRACKET ')'
#define OPEN_SBRACKET '['
#define CLOSE_SBRACKET ']'
#define OPEN_QBRACKET '{'
#define CLOSE_QBRACKET '}'
#define COMMA ','
#define TILDE '^'
#define OR '|'
#define MINUS_RANGE '-'  


/*
**	content type
**	CONBYTE: transform content to vector<byte>
**	CONPCRE: transform content to string
*/
enum CONTYPE
{
	CONBYTE,
	CONPCRE
};

struct RULEOPTIONRAW
{
	std::string name;
	std::string value;
};

struct ISSPACE
{
	ISSPACE()
	{
		ZeroMemory(g_map, sizeof(g_map) / sizeof(g_map[0]));
		g_map[' '] = 1;
		g_map['\t'] = 1;
		g_map['\n'] = 1;
		g_map['\r'] = 1;
	}
	bool operator()(char c)
	{
		return g_map[c] == 1;
	}
protected:
	char g_map[256];
};

struct EMPTYRULE
{
	bool operator()(const std::string &str)
	{
		if (str.empty())
		{
			return true;
		}
		std::string::const_iterator i = std::find_if_not(str.begin(), str.end(), ISSPACE());
		return (i == str.end() || *i == '#');
	}
};

struct CONTENTNUM
{
	bool operator()(RULEOPTIONRAW &rp)
	{
		if (0 == stricmp("content", rp.name.c_str()) ||
			0 == stricmp("uricontent", rp.name.c_str()))
		{
			return true;
		}
		return false;
	}
};




/*
* read rules from a file
*/
ulong LoadFile(const char *fileName, std::vector<std::string> &rules)
{
	std::ifstream fin(fileName);
	if(!fin)
	{
		std::cerr << "Open file Failed!" << std::endl;
		return (ulong)-1;
	}
	for (std::string strRule; std::getline(fin, strRule);)
	{
		 rules.push_back(strRule);
	}
	fin.close();
	rules.erase(std::remove_if(rules.begin(), rules.end(), EMPTYRULE()), rules.end());
	return 0;
}

void ExtractOption(std::string &ruleOptions, std::vector<RULEOPTIONRAW> &options)
{
	// Split the options of rule with semicolon and extract related options 
	for (std::string::iterator i = ruleOptions.begin(); ;)
	{
		std::string::iterator iComma = std::find(i, ruleOptions.end(), ';');
		if (iComma == ruleOptions.end())
		{
			break;
		}
		if (*(iComma - 1) != '\\')
		{
			RULEOPTIONRAW or;
			std::string::iterator iNameBeg = std::find_if(i, iComma, isalpha);
			std::string::iterator iValueBeg = std::find(iNameBeg + 1, iComma, ':');

			// Get the end position of option name and assign into RULEOPTIONRAW
			std::string::iterator iNameEnd = iValueBeg;
			for (; ISSPACE()(*iNameEnd); --iNameEnd);
			or.name.assign(iNameBeg,iNameEnd);
			if (iValueBeg == iComma)
			{
				or.value.assign(iValueBeg,iComma);
			}
			else
			{
				or.value.assign(iValueBeg + 1,iComma);
			}			
			options.push_back(or);
		}
		i = iComma + 1;
	}
}

//eliminate quotation mark
template<typename _Iter>
bool QuotedContext(_Iter &beg, _Iter &end)
{
	_Iter iBeg = std::find(beg, end, '"'), iEnd = end - 1;
	if (iBeg == end)
	{
		return false;
	}
	++iBeg;

	for (; iEnd > iBeg && *iEnd != '"'; --iEnd);

	if (iEnd > iBeg)
	{
		beg = iBeg;
		end = iEnd;
		return true;
	}
	return false;
}

//Extract pcre string and mark process mode
template<typename _Iter>
ulong FormatPcre (_Iter pBeg, _Iter pEnd, OPTIONPCRE &pcre)
{
	pcre.SetFlag(0);

	if (*std::find_if_not(pBeg, pEnd, ISSPACE()) == '!')
	{
		return ulong(-2);
	}
	if (!QuotedContext(pBeg, pEnd))
	{
		return ulong(-1);
	}

	_Iter iPcreBeg = std::find(pBeg, pEnd, '/');
	_Iter iPcreEnd = pEnd;
	for(; *iPcreEnd != '/'; --iPcreEnd);

	if ((iPcreBeg + 1) > iPcreEnd)
	{
		return ulong(-1);
	}
	else
	{
		pcre.SetPattern(std::string(iPcreBeg, pEnd).c_str());
	}

	std::string temp = std::string(iPcreEnd + 1, pEnd);
	
	for(_Iter j = temp.begin(); j != temp.end(); ++j)
	{
		switch (*j)
		{
		case 'i':
			pcre.AddFlag(PF_i);
			continue;
		case 's':
			pcre.AddFlag(PF_s);
			continue;
		case 'm':
			pcre.AddFlag(PF_m);
			continue;
		case 'x':
			pcre.AddFlag(PF_x);
			continue;
		case 'A':
			pcre.AddFlag(PF_A);
			continue;
		case 'E':
			pcre.AddFlag(PF_E);
			continue;
		case 'G':
			pcre.AddFlag(PF_G);
			continue;
		case 'R':
			pcre.AddFlag(PF_R);
			continue;
		case 'U':
			pcre.AddFlag(PF_U);
			continue;
		case 'B':
			pcre.AddFlag(PF_B);
			continue;
		case 'P':
			pcre.AddFlag(PF_P);
			continue;
		case 'H':
			pcre.AddFlag(PF_H);
			continue;
		case 'M':
			pcre.AddFlag(PF_M);
			continue;
		case 'C':
			pcre.AddFlag(PF_C);
			continue;
		case 'O':
			pcre.AddFlag(PF_O);
			continue;
		case 'I':
			pcre.AddFlag(PF_I);
			continue;
		case 'D':
			pcre.AddFlag(PF_D);
			continue;
		case 'K':
			pcre.AddFlag(PF_K);
			continue;
		case 'S':
			pcre.AddFlag(PF_S);
			continue;
		case 'Y':
			pcre.AddFlag(PF_Y);
			continue;			
		default:
			pcre.AddFlag(0);
		}
	}
	return 0;
}

inline char HexBit(char value)
{
	static struct HEXBITHASH
	{
		char m_CharMap[256];
		HEXBITHASH()
		{
			ZeroMemory(m_CharMap, sizeof(m_CharMap));
			for (int i = '0'; i <= '9'; ++i)
			{
				m_CharMap[i] = char(i - '0');
			}
			for (int i = 'A'; i <= 'F'; ++i)
			{
				m_CharMap[i] = char(i - 'A' + 10);
			}
			for (int i = 'a'; i <= 'f'; ++i)
			{
				m_CharMap[i] = char(i - 'a' + 10);
			}
		};
	} hexbithash;

	return hexbithash.m_CharMap[value];
}

inline byte HexByte(const char *p2Bytes)
{
	return (HexBit(p2Bytes[0]) << 4 | HexBit(p2Bytes[1]));
}

//The string type is converted into byte type
template<typename _Iter>
ulong FormatOptionContent (_Iter cBeg, _Iter cEnd, BYTEARY &content, CONTYPE TYPE)
{

	//Transform ducted data into hex number
	for (_Iter i = cBeg; i != cEnd; ++i)
	{
		char cHexVal[2];
		UINT nFlag = 0;
		switch (*i)
		{
		case '\\':
			if(TYPE == CONPCRE)
			{
				content.push_back(*i);
			}
			content.push_back(*++i);
			break;
		case '|':
			for (++i; i != cEnd && *i != '|'; ++i)
			{
				if (*i != ' ' && *i != '\t')
				{
					cHexVal[nFlag] = *i;
					nFlag = !nFlag;
					if (!nFlag)
					{
						if(TYPE == CONBYTE)
						{
							content.push_back(HexByte(cHexVal));
						}
						else if(TYPE == CONPCRE)
						{
							content.push_back('\\');
							content.push_back('x');
							content.push_back(cHexVal[0]);
							content.push_back(cHexVal[1]);
						}
					}
				}
			}
			break;
		default:
			if(TYPE == CONPCRE && (*i == ANY ||	*i == ESCAPE || *i == STAR || *i == PLUS || *i == OPT
				|| *i == OPEN_RBRACKET || *i == CLOSE_RBRACKET || *i == OPEN_SBRACKET || *i == CLOSE_SBRACKET
				|| *i == OPEN_QBRACKET || *i == CLOSE_QBRACKET || *i == COMMA || *i == TILDE || *i == OR || *i == MINUS_RANGE))
			{
				content.push_back('\\');
			}
			content.push_back(*i);
		}
	}
	if(TYPE == CONPCRE)
	{
		content.push_back('\0');
	}
	return 0;
}


ulong ProcessOption(std::string &ruleOptions, CSnortRule &snortRule)
{
	std::vector<RULEOPTIONRAW> options;
	ExtractOption(ruleOptions, options);

	ulong nCONT = 0;
	ulong sum = 0;
	nCONT = std::count_if(options.begin(), options.end(), CONTENTNUM());

	//Mark process mode, "0" is error ,"1" is normal
	ulong nResult = 0;
	ulong nFlag = 0;

	for(std::vector<RULEOPTIONRAW>::iterator iOp = options.begin(); iOp != options.end(); ++iOp)
	{
		std::string::iterator opValueBeg = iOp->value.begin();
		std::string::iterator opValueEnd = iOp->value.end();
		opValueBeg = std::find_if_not(opValueBeg, opValueEnd, ISSPACE());

		if (0 == stricmp("sid", iOp->name.c_str()))
		{
			snortRule.SetSid(atoi(&*opValueBeg));
		}
		else if (0 == stricmp("pcre", iOp->name.c_str()))
		{
			OPTIONPCRE *pPcre = new OPTIONPCRE;
			ulong nr = FormatPcre(opValueBeg, opValueEnd, *pPcre);
			if (nr != 0)
			{
				if (nr == ulong(-2))
				{
					nFlag |= CSnortRule::RULE_HASNOT;
				}
				else
				{
					nResult = ulong(-1);
					delete pPcre;
					break;
				}
			}
			snortRule.PushBack(pPcre);
		}
		else if (0 == stricmp ("byte_jump", iOp->name.c_str()) ||
			0 == stricmp("byte_test", iOp->name.c_str()))
		{
			nFlag |= CSnortRule::RULE_HASBYTE;
		}
		else if (0 == stricmp("content", iOp->name.c_str()) ||
			0 == stricmp("uricontent", iOp->name.c_str()))
		{
			OPTIONCONTENT *pContent = new OPTIONCONTENT;
			pContent->SetFlag(0);
			pContent->nOffset = 0;
			pContent->nDepth =0;
			pContent->nDistance = 0;
			pContent->nWithin = 0;

			if (*std::find_if_not(opValueBeg, opValueEnd, ISSPACE()) == '!')
			{
				nFlag |= CSnortRule::RULE_HASNOT;
			}
			else if (!QuotedContext(opValueBeg, opValueEnd))
			{
				nResult = ulong(-1);
				delete pContent;
				break;
			}
			else
			{
				std::string str(opValueBeg, opValueEnd);
				pContent->SetPattern(str.c_str());
				FormatOptionContent(opValueBeg, opValueEnd, pContent->vecconts, CONBYTE);
				if (pContent->vecconts.size() < 4)
				{
					++sum;
				}
			}
			snortRule.PushBack(pContent);
		}
		else if (0 == stricmp("nocase", iOp->name.c_str()))
		{			
			ulong last = snortRule.Size() - 1;
			OPTIONCONTENT * temp = dynamic_cast<OPTIONCONTENT *>(snortRule[last]);
			if (NULL == temp)
			{
				nResult = ulong(-1);
				break;
			}
			else
			{
				temp->AddFlag(CF_NOCASE);
			}
		}
		else if (0 == stricmp("offset", iOp->name.c_str()))
		{
			ulong last = snortRule.Size() - 1;
			OPTIONCONTENT *pTemp = dynamic_cast<OPTIONCONTENT*>(snortRule[last]);
			if (NULL == pTemp)
			{
				nResult = ulong(-1);
				break;
			}
			else
			{
				pTemp->nOffset = atoi(&*opValueBeg);
				pTemp->AddFlag(CF_OFFSET);
			}
		}
		else if (0 == stricmp("depth", iOp->name.c_str()))
		{
			ulong last = snortRule.Size() - 1;
			OPTIONCONTENT *pTemp = dynamic_cast<OPTIONCONTENT *>(snortRule[last]);
			if (NULL == pTemp)
			{
				nResult = ulong(-1);
				break;
			}
			else
			{
				pTemp->nDepth = atoi(&*opValueBeg);
				pTemp->AddFlag(CF_DEPTH);
			}
		}
		else if (0 == stricmp("distance", iOp->name.c_str()))
		{
			ulong last = snortRule.Size() - 1;
			OPTIONCONTENT *pTemp = dynamic_cast<OPTIONCONTENT *>(snortRule[last]);
			if (NULL == pTemp)
			{
				nResult = ulong(-1);
				break;
			}
			else
			{
				pTemp->nDistance = atoi(&*opValueBeg);
				pTemp->AddFlag(CF_DISTANCE);
			}
		}
		else if (0 == stricmp("within", iOp->name.c_str()))
		{
			ulong last = snortRule.Size() - 1;
			OPTIONCONTENT *pTemp = dynamic_cast<OPTIONCONTENT *>(snortRule[last]);
			if (NULL == pTemp)
			{
				nResult = ulong(-1);
				break;
			}
			else
			{
				pTemp->nWithin = atoi(&*opValueBeg);
				pTemp->AddFlag(CF_WITHIN);
			}
		}
	}
	if (sum == nCONT)
	{
		nFlag |= CSnortRule::RULE_HASNOSIG;
	}
	snortRule.SetFlag(nFlag);
	return nResult;
}


/*
* content has depth or within constraint
* construct it to linear NFA
*/
void contentToLinearNFA(OPTIONCONTENT *pContent, CNfa &nfa)
{
	nfa.Reserve(nfaReserve);
	ulong state_size = 0;

	ulong patternLen = pContent->vecconts.size();
	ulong mustCnt = 0, maxCnt = 0;//mustCnt和maxCnt分别代表经过任意字符跳转出现的最少和最多次数
	if(pContent->TestFlag(CF_DEPTH))
	{
		mustCnt = pContent->nOffset;//必须跳转mustCnt次
		maxCnt = pContent->nDepth - patternLen;//跳转了mustCnt次后，还需跳0-maxCnt次
	}
	else if(pContent->TestFlag(CF_WITHIN))
	{
		mustCnt = pContent->nDistance;
		maxCnt = pContent->nWithin - patternLen;
	}

	//共offset + depth + 1或者distance + within + 1个状态，有一个初始状态
	ulong stateID = mustCnt + maxCnt;//从stateID开始进行content字符串的跳转

	//mustCnt：0-255任意字符跳转，必须要跳转的状态数
	for(ulong i = 0; i < (mustCnt + maxCnt); ++i)
	{
		nfa.Resize(++state_size);
		//nfa.PushBack(CNfaRow());
		CNfaRow &row = nfa.Back();
		ulong id = i + 1;
		for(int j = 0; j < 256; ++j)
		{
			row.AddDest(j, id);
		}
		if(i >= mustCnt)
		{
			row.AddDest(SC_DFACOLCNT, stateID);
		}
	}

	++stateID;
	for(BYTEARY_CITER iter = pContent->vecconts.begin();
		iter != pContent->vecconts.end(); ++iter)
	{
		nfa.Resize(++state_size);
		//nfa.PushBack(CNfaRow());
		CNfaRow &row = nfa.Back();

		if(pContent->TestFlag(CF_NOCASE) && isalpha(*iter))
		{
			row.AddDest(toupper(*iter), stateID);
			row.AddDest(tolower(*iter), stateID);
		}
		else
		{
			row.AddDest(*iter, stateID);
		}

		++stateID;
	}

	nfa.Reserve(++state_size);
}

//没有depth或者within标记的
void contentToDefaultNFA(OPTIONCONTENT *pContent, CNfa &nfa)
{
	nfa.Reserve(nfaReserve);
	ulong state_size = 0;

	ulong mustCnt = 0;//经过的偏移字符个数
	if((pContent->GetFlag() & CF_OFFSET))
	{
		mustCnt = pContent->nOffset;

	}
	else if((pContent->GetFlag() & CF_DISTANCE))
	{
		mustCnt = pContent->nDistance;
	}

	for(ulong i = 0; i < mustCnt; ++i)
	{
		nfa.Resize(++state_size);
		//nfa.PushBack(CNfaRow());
		CNfaRow &row = nfa.Back();
		
		ulong id = i + 1;
		for(ulong j = 0; j < 256; ++j)
		{
			row.AddDest(j, id);
		}
	}

	ulong stateID = mustCnt;//从stateID开始进行content.vecconts的匹配
	ulong patternLen = pContent->vecconts.size(); 
	BYTEARY pattern;
	for(BYTEARY_CITER iter = pContent->vecconts.begin();
		iter != pContent->vecconts.end(); ++iter)
	{
		byte c = byte((pContent->GetFlag() & CF_NOCASE) ? tolower(*iter) : *iter);
		pattern.push_back(c);//如果content有nocase标记，则把模式串用字符的小写形式表示
	}

	//在前面加.*
	for(ulong i = 0; i < patternLen; ++i)
	{
		nfa.Resize(++state_size);
		//nfa.PushBack(CNfaRow());
		CNfaRow &row = nfa.Back();
		ulong id = stateID + i + 1;
		for(ulong c = 0; c < 256; ++c)
		{
			if(i == 0)
			{
				row.AddDest(c, stateID);
			}
			byte character = byte((pContent->GetFlag() & CF_NOCASE) ? tolower(c) : c);
			if(character == pattern[i])
			{
				row.AddDest(c, id);
			}
		}
	}
	nfa.Reserve(++state_size);
}

void content2Nfa(OPTIONCONTENT *pContent, CNfa &nfa)
{
	/*
	如果有depth或者within，则构造成线性的FA; 否则，构造成默认FA
	*/
	if((pContent->GetFlag() & CF_DEPTH) || (pContent->GetFlag() & CF_WITHIN))
	{
		contentToLinearNFA(pContent, nfa);
	}
	else
	{
		contentToDefaultNFA(pContent, nfa);
	}
}

//test function: output a nfa
void PrintDfaToText(CNfa &nfa, std::string &fileName)
{
	ulong stateNum = nfa.Size();
	std::ofstream fout(fileName);
	fout << "\t";
	for(ulong t = 0; t < 257; ++t)
	{
		fout << t << "\t";
	}
	fout << std::endl;
	for(ulong i = 0; i < stateNum; ++i)
	{
		const CNfaRow &row = nfa[i];
		fout << i << "\t";
		for(ulong j = 0; j < 257; ++j)
		{
			ulong nCnt = row.DestCnt(j);
			if(nCnt == 0)
			{
				fout << -1 << "\t";
			}
			else
			{
				for(ulong k = 0; k < nCnt; ++k)
				{
					fout << row.GetDest(j, k) << ", ";
				}
				fout << "\t";
			}
		}
		fout << std::endl;
	}
	fout.close();
}

/*
**	this function transforms content to pcre
**	based on the content constraints: distance, within, offset, depth, nocase
**
**	@param pContent	 pointer to the original content option
**	@param pcreStr		the transformed pcre
**
**	@return
**	@retval  0 function successful
**	@retval -1 fatal error
*/
ulong content2Pcre(OPTIONCONTENT *pContent, CDllString &pcreStr)
{
	std::stringstream ss;
	pcreStr = CDllString("/^");
	if(pContent->TestFlag(CF_OFFSET) && pContent->nOffset > 0)
	{
		ss.str("");
		ss << pContent->nOffset;
		pcreStr.Append(".{");
		pcreStr.Append(ss.str().c_str());
		pcreStr.Append("}");
	}
	if(pContent->TestFlag(CF_DISTANCE) && pContent->nDistance > 0)
	{
		ss.str("");
		ss << pContent->nDistance;
		pcreStr.Append(".{");
		pcreStr.Append(ss.str().c_str());
		pcreStr.Append("}");
	}
	if(!(pContent->TestFlag(CF_DEPTH) || pContent->TestFlag(CF_WITHIN)))
	{
		//without depth and without within
		if(!(pContent->TestFlag(CF_OFFSET) || pContent->TestFlag(CF_DISTANCE))
			|| (pContent->TestFlag(CF_OFFSET) && pContent->nOffset == 0)
			|| (pContent->TestFlag(CF_DISTANCE) && pContent->nDistance == 0))
		{
			//without offset and without distance
			pcreStr = CDllString("/");
		}
		else
		{
			pcreStr.Append(".*");
		}
	}
	else
	{
		int n = 0;
		if(pContent->TestFlag(CF_DEPTH))
		{
			n = pContent->nDepth - pContent->vecconts.size();
		}
		else if(pContent->TestFlag(CF_WITHIN))
		{
			n = pContent->nWithin - pContent->vecconts.size();
		}
		if(n < 0)
		{
			return SC_ERROR;
		}
		else if(n > 0)
		{
			ss.str("");
			ss << n;
			pcreStr.Append(".{0,");
			pcreStr.Append(ss.str().c_str());
			pcreStr.Append("}");
		}
	}

	//handle the data in content pipe
	std::string con;
	con.resize(pContent->GetPattern(NULL, 0));
	pContent->GetPattern(&con[0], con.size());
	std::string::iterator  opValueBeg= con.begin(), opValueEnd = con.end();
	BYTEARY conVec;

	//transform the pipe data into a special string
	FormatOptionContent(opValueBeg, opValueEnd, conVec, CONPCRE);
	const char* str = (char*)&conVec[0];
	pcreStr.Append(str);

	if(pContent->TestFlag(CF_NOCASE))
	{
		pcreStr.Append("/si");
	}
	else
	{
		pcreStr.Append("/s");
	}
	return 0;
}




/*
**	NAME
**	 CRegChainToNFA::
*/
/**
**	This function converts a CRegChain to a CNfa
**
**	use pcre library to construct a nfa from a pcre
**	
**	@param regchain	a CRegChain object which contains a pcre list
**	@param nfa		the transformed CNfa object 
**
**	@return integer
**
**	@retval  0 function successful
**	@retval <>0 fatal error
*/

ulong CRegChainToNFA(CRegChain &regchain, CNfa &nfa)
{
	nfa.Reserve(nfaReserve);
	int flag = 0;
	for(ulong i = 0; i < regchain.Size(); ++i)
	{
		flag = PcreToNFA(regchain[i].GetStr(), nfa, regchain.GetSigs());
		if(flag != 0)
		{
			nfa.Clear();
			return flag;
		}
	}
	regchain.GetSigs().Unique();
	return 0;
}

/* assign all the signatures of each rule to all its option list

Arguments:
  result		the compile result
  BegIdx		the index of the first regex which belongs to this rule
  EndIdx		the next index of the last regex which belongs to this rule

Returns:		nothing

*/
void AssignSig(CCompileResults &result, ulong BegIdx, ulong EndIdx)
{
	//all the signatures of this rule
	std::vector<SIGNATURE> vecRuleSigs;
	for (ulong i = BegIdx; i < EndIdx; ++i)
	{
		for (ulong j = 0; j < result.GetRegexTbl()[i].GetSigs().Size(); ++j)
		{
			if (std::find(vecRuleSigs.begin(), vecRuleSigs.end(),
				result.GetRegexTbl()[i].GetSigs()[j]) == vecRuleSigs.end())
			{
				vecRuleSigs.push_back(result.GetRegexTbl()[i].GetSigs()[j]);
			}
		}
	}

	//assign all the signatures of each rule to all its option list
	for (ulong i = BegIdx; i < EndIdx; ++i)
	{
		result.GetRegexTbl()[i].GetSigs().Clear();
		for (ulong j = 0; j < vecRuleSigs.size(); ++j)
		{
			result.GetRegexTbl()[i].GetSigs().PushBack(vecRuleSigs[j]);
		}
	}
}
