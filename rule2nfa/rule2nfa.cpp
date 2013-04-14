#include "stdafx.h"
#include "rule2nfa.h"
#include "CombineTree.h"
#include "stdafx.h"

struct OPTIONCONTENT : public RULEOPTION
{
	std::vector<BYTE> vecconts;
	int nOffset;
	int nDepth;
	int nDistance;
	int nWithin;
};

struct OPTIONPCRE : public RULEOPTION
{
	std::string strPattern;
};

enum OPTIONCONTENTFLAGS
{
	CF_NOCASE		= (1 << 0),//1
	CF_OFFSET		= (1 << 1),//2
	CF_DEPTH		= (1 << 2),//4
	CF_DISTANCE		= (1 << 3),//8
	CF_WITHIN		= (1 << 4),//16
};

enum PCREFLAGS
{
	PF_i = (1 << 0),
	PF_s = (1 << 1),
	PF_m = (1 << 2),
	PF_x = (1 << 3),
	PF_A = (1 << 4),
	PF_E = (1 << 5),
	PF_G = (1 << 6),
	PF_R = (1 << 7),
	PF_U = (1 << 8),
	PF_B = (1 << 9),
	PF_P = (1 << 10),
	PF_H = (1 << 11),
	PF_M = (1 << 12),
	PF_C = (1 << 13),
	PF_O = (1 << 14),
	PF_I = (1 << 15),
	PF_D = (1 << 16),
	PF_K = (1 << 17),
	PF_S = (1 << 18),
	PF_Y = (1 << 19)
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
private:
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

struct SEARCHOPTIONS
{
	bool operator()(RULEOPTIONRAW &rp)
	{
		if ((0 == stricmp("content", rp.name.c_str())) || (0 == stricmp("sid", rp.name.c_str()))
			|| (0 == stricmp("pcre", rp.name.c_str())) || (0 == stricmp("uricontent", rp.name.c_str())))
		{
			return true;
		}
		return false;
	}
};

struct SEARCHBYTEOPTION
{
	bool operator()(RULEOPTIONRAW &rp)
	{
		if ((0 == stricmp("byte_test", rp.name.c_str())) || (0 == stricmp("byte_jump", rp.name.c_str())))
		{
			return true;
		}
		return false;
	}
};

/*
* read rules from a file
*/
size_t LoadFile(LPCTSTR fileName, std::vector<std::string> &rules)
{
	std::ifstream fin(fileName);
	if(!fin)
	{
		std::cerr << "Open file Failed!" << std::endl;
		return -1;
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
size_t FormatPcre (_Iter pBeg, _Iter pEnd, OPTIONPCRE &pcre)
{
	pcre.nFlags = 0;
	//If rule includes '!',we should delete this rule
	if (*std::find_if_not(pBeg, pEnd, ISSPACE()) == '!')
	{
		return size_t(-2);
	}
	if (!QuotedContext(pBeg, pEnd))
	{
		return size_t(-1);
	}

	_Iter iPcreBeg = std::find(pBeg, pEnd, '/');
	_Iter iPcreEnd = pEnd;
	for(; *iPcreEnd != '/'; --iPcreEnd);

	if ((iPcreBeg + 1) > iPcreEnd)
	{
		return size_t(-1);
	}
	else
	{
		//pcre.strPattern = std::string(iPcreBeg + 1, iPcreEnd);
		pcre.strPattern = std::string(iPcreBeg, pEnd);
	}

	std::string temp = std::string(iPcreEnd + 1, pEnd);
	
	for(_Iter j = temp.begin(); j != temp.end(); ++j)
	{
		switch (*j)
		{
		case 'i':
			pcre.nFlags |= PF_i;
			continue;
		case 's':
			pcre.nFlags |= PF_s;
			continue;
		case 'm':
			pcre.nFlags |= PF_m;
			continue;
		case 'x':
			pcre.nFlags |= PF_x;
			continue;
		case 'A':
			pcre.nFlags |= PF_A;
			continue;
		case 'E':
			pcre.nFlags |= PF_E;
			continue;
		case 'G':
			pcre.nFlags |= PF_G;
			continue;
		case 'R':
			pcre.nFlags |= PF_R;
			continue;
		case 'U':
			pcre.nFlags |= PF_U;
			continue;
		case 'B':
			pcre.nFlags |= PF_B;
			continue;
		case 'P':
			pcre.nFlags |= PF_P;
			continue;
		case 'H':
			pcre.nFlags |= PF_H;
			continue;
		case 'M':
			pcre.nFlags |= PF_M;
			continue;
		case 'C':
			pcre.nFlags |= PF_C;
			continue;
		case 'O':
			pcre.nFlags |= PF_O;
			continue;
		case 'I':
			pcre.nFlags |= PF_I;
			continue;
		case 'D':
			pcre.nFlags |= PF_D;
			continue;
		case 'K':
			pcre.nFlags |= PF_K;
			continue;
		case 'S':
			pcre.nFlags |= PF_S;
			continue;
		case 'Y':
			pcre.nFlags |= PF_Y;
			continue;			
		default:
			pcre.nFlags |= 0;
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
				m_CharMap[i] = i - '0';
			}
			for (int i = 'A'; i <= 'F'; ++i)
			{
				m_CharMap[i] = i - 'A' + 10;
			}
			for (int i = 'a'; i <= 'f'; ++i)
			{
				m_CharMap[i] = i - 'a' + 10;
			}
		};
	} hexbithash;

	return hexbithash.m_CharMap[value];
}

inline BYTE HexByte(const char *p2Bytes)
{
	return (HexBit(p2Bytes[0]) << 4 | HexBit(p2Bytes[1]));
}

//The string type is converted into unsigned char type
template<typename _Iter>
size_t FormatOptionContent (_Iter cBeg, _Iter cEnd, std::vector<BYTE> &content)
{
	if (*std::find_if_not(cBeg, cEnd, ISSPACE()) == '!')
	{
		return size_t(-2);
	}
	if (!QuotedContext(cBeg, cEnd))
	{
		return size_t(-1);
	}

	//Transform ducted data into hex number
	for (_Iter i = cBeg; i != cEnd; ++i)
	{
		char cHexVal[2];
		UINT nFlag = 0;
		switch (*i)
		{
		case '\\':
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
						content.push_back(HexByte(cHexVal));
					}
				}
			}
			break;
		default:
			content.push_back(*i);
		}
	}
	return 0;
}

//Extract OPTIONCONTENT string and related options
template<typename _Iter>
size_t ExtractOptionContent(_Iter &iBeg, _Iter &iEnd, OPTIONCONTENT &opCont)
{
	opCont.nFlags = 0;
	opCont.nOffset = 0;
	opCont.nDepth =0;
	opCont.nDistance = 0;
	opCont.nWithin = 0;
	for (_Iter i = iBeg; i!= iEnd; ++i)
	{
		std::string::iterator iValueBeg = i->value.begin();
		std::string::iterator iValueEnd = i->value.end();
		iValueBeg = std::find_if_not(iValueBeg, iValueEnd, ISSPACE());
		size_t nr = FormatOptionContent(iValueBeg, iValueEnd, opCont.vecconts);
		if (nr != 0)
		{
			return nr;
		}
		if (0 == stricmp("content", i->name.c_str()))
		{
			return size_t(-1);
		}
		else if (0 == stricmp("uricontent", i->name.c_str()))
		{
			return size_t(-1);
		}
		else if (0 == stricmp("nocase", i->name.c_str()))
		{
			opCont.nFlags |= CF_NOCASE;
		}
		else if (0 == stricmp("offset", i->name.c_str()))
		{
			*(int*)(&opCont.nOffset) = atoi(&*iValueBeg);
			opCont.nFlags |= CF_OFFSET;
		}
		else if (0 == stricmp("depth", i->name.c_str()))
		{
			*(int*)(&opCont.nDepth) = atoi(&*iValueBeg);
			opCont.nFlags |= CF_DEPTH;
		}
		else if (0 == stricmp("distance", i->name.c_str()))
		{
			*(int*)(&opCont.nDistance) = atoi(&*iValueBeg);
			opCont.nFlags |= CF_DISTANCE;
		}
		else if (0 == stricmp("within", i->name.c_str()))
		{
			*(int*)(&opCont.nWithin) = atoi(&*iValueBeg);
			opCont.nFlags |= CF_WITHIN;
		}	
	}
	return 0;
}

size_t ProcessOption(std::string &ruleOptions, CSnortRule &snortRule)
{
	std::vector<RULEOPTIONRAW> options;
	ExtractOption(ruleOptions, options);

	////Mark process mode, "0" is error ,"1" is normal
	size_t nResult = size_t(-1);

	////snortRule.nFlags = 0;

	////Read one rule and only store "sid","pcre","content" and related options

	size_t nFlag = 0;
	for(std::vector<RULEOPTIONRAW>::iterator iOp = options.begin(); iOp != options.end(); ++iOp)
	{
		if (0 == stricmp("sid", iOp->name.c_str()))
		{
			std::string::iterator opValueBeg = iOp->value.begin();
			std::string::iterator opValueEnd = iOp->value.end();
			opValueBeg = std::find_if_not(opValueBeg, opValueEnd, ISSPACE());
			snortRule.SetSid(atoi(&*opValueBeg));
		}
		else if (0 == stricmp("pcre", iOp->name.c_str()))
		{
			std::string::iterator opValueBeg = iOp->value.begin();
			std::string::iterator opValueEnd = iOp->value.end();
			opValueBeg = std::find_if_not(opValueBeg, opValueEnd, ISSPACE());

			OPTIONPCRE *pPcre = new OPTIONPCRE;
			size_t nr = FormatPcre(opValueBeg, opValueEnd, *pPcre);
			if (nr != 0)
			{
				if (nr == size_t(-2))
				{
					nFlag |= CSnortRule::RULE_HASNOT;
				}
				delete pPcre;
				break;
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
			pContent->nFlags = 0;
			pContent->nOffset = 0;
			pContent->nDepth =0;
			pContent->nDistance = 0;
			pContent->nWithin = 0;

			std::string::iterator iValueBeg = iOp->value.begin();
			std::string::iterator iValueEnd = iOp->value.end();
			iValueBeg = std::find_if_not(iValueBeg, iValueEnd, ISSPACE());
			size_t nr = FormatOptionContent(iValueBeg, iValueEnd, pContent->vecconts);
			if (nr != 0)
			{
				if (nr == size_t(-2))
				{
					nFlag |= CSnortRule::RULE_HASNOT;
				}
				delete pContent;
				break;
			}
			snortRule.PushBack(pContent);
		}
		else if (0 == stricmp("nocase", iOp->name.c_str()))
		{			
			size_t last = snortRule.Size() - 1;
			if (NULL != dynamic_cast<OPTIONCONTENT *>(snortRule[last]))
			{
				nResult = size_t(-1);
			}
			else
			{
				snortRule[last]->nFlags |= CF_NOCASE;
			}
		}
		else if (0 == stricmp("offset", iOp->name.c_str()))
		{
			size_t last = snortRule.Size() - 1;
			if (NULL != dynamic_cast<OPTIONCONTENT *>(snortRule[last]))
			{
				nResult = size_t(-1);
			}
			else
			{
				std::string::iterator iValueBeg = iOp->value.begin();
				std::string::iterator iValueEnd = iOp->value.end();
				iValueBeg = std::find_if_not(iValueBeg, iValueEnd, ISSPACE());	
				//*(int*)(&opCont.nOffset) = atoi(&*iValueBeg);
				snortRule[last]->nFlags |= CF_OFFSET;
			}
		}
		else if (0 == stricmp("depth", iOp->name.c_str()))
		{
			size_t last = snortRule.Size() - 1;
			if (NULL != dynamic_cast<OPTIONCONTENT *>(snortRule[last]))
			{
				nResult = size_t(-1);
			}
			else
			{
				std::string::iterator iValueBeg = iOp->value.begin();
				std::string::iterator iValueEnd = iOp->value.end();
				iValueBeg = std::find_if_not(iValueBeg, iValueEnd, ISSPACE());	
				//*(int*)(&opCont.nOffset) = atoi(&*iValueBeg);
				snortRule[last]->nFlags |= CF_DEPTH;
			}
		}
		else if (0 == stricmp("distance", iOp->name.c_str()))
		{
			size_t last = snortRule.Size() - 1;
			if (NULL != dynamic_cast<OPTIONCONTENT *>(snortRule[last]))
			{
				nResult = size_t(-1);
			}
			else
			{
				std::string::iterator iValueBeg = iOp->value.begin();
				std::string::iterator iValueEnd = iOp->value.end();
				iValueBeg = std::find_if_not(iValueBeg, iValueEnd, ISSPACE());	
				//*(int*)(&opCont.nOffset) = atoi(&*iValueBeg);
				snortRule[last]->nFlags |= CF_DISTANCE;
			}
		}
		else if (0 == stricmp("within", iOp->name.c_str()))
		{
			size_t last = snortRule.Size() - 1;
			if (NULL != dynamic_cast<OPTIONCONTENT *>(snortRule[last]))
			{
				nResult = size_t(-1);
			}
			else
			{
				std::string::iterator iValueBeg = iOp->value.begin();
				std::string::iterator iValueEnd = iOp->value.end();
				iValueBeg = std::find_if_not(iValueBeg, iValueEnd, ISSPACE());	
				//*(int*)(&opCont.nOffset) = atoi(&*iValueBeg);
				snortRule[last]->nFlags |= CF_WITHIN;
			}
		}
	}
	snortRule.SetFlag(nFlag);
	return 0;
}


/*
* read rules from a file
* then process the rules to CSnortRule
* callback function RECIEVER to handle CSnortRule
*/

CRECHANFA size_t ParseRule(LPCTSTR fileName, RECIEVER recv, LPVOID lpUser)
{
	if(recv == NULL)
	{
		return (size_t)-1;
	}
	std::vector<std::string> rules;
	if(0 == LoadFile(fileName, rules))
	{
		if(!rules.empty())
		{
			for(std::vector<std::string>::iterator rIt = rules.begin();
				rIt != rules.end(); ++rIt)
			{
				//Delete the rule header, reserve the rule options
				rIt->erase(rIt->begin(), find(rIt->begin(), rIt->end(), '(') + 1);
				rIt->erase(find(rIt->rbegin(), rIt->rend(), ')').base() - 1, rIt->end());

				CSnortRule snortRule;
				if (0 == ProcessOption(*rIt, snortRule))
				{
					recv(snortRule, lpUser);
				}
			}
		}
	}
	return 0;
}

/*
如果有depth或者within，则构造成线性的FA
*/
void contentToLinearNFA(OPTIONCONTENT *content, CNfa &nfa)
{
	size_t patternLen = content->vecconts.size();
	size_t mustCnt = 0, maxCnt = 0;//mustCnt和maxCnt分别代表经过任意字符跳转出现的最少和最多次数
	if((content->nFlags & CF_DEPTH))
	{
		mustCnt = content->nOffset;//必须跳转mustCnt次
		maxCnt = content->nDepth - patternLen;//跳转了mustCnt次后，还需跳0-maxCnt次
	}
	else if((content->nFlags & CF_WITHIN))
	{
		mustCnt = content->nDistance;
		maxCnt = content->nWithin - patternLen;
	}

	//共offset + depth + 1或者distance + within + 1个状态，有一个初始状态
	size_t stateID = mustCnt + maxCnt;//从stateID开始进行content字符串的跳转

	//mustCnt：0-255任意字符跳转，必须要跳转的状态数
	for(size_t i = 0; i < (mustCnt + maxCnt); ++i)
	{
		nfa.PushBack(CNfaRow());
		CNfaRow &row = nfa.Back();
		size_t id = i + 1;
		for(int j = 0; j < 256; ++j)
		{
			row[j].PushBack(id);
		}
		if(i >= mustCnt)
		{
			row[EMPTYEDGE].PushBack(stateID);
		}
	}

	++stateID;
	for(std::vector<BYTE>::const_iterator iter = content->vecconts.begin();
		iter != content->vecconts.end(); ++iter)
	{
		nfa.PushBack(CNfaRow());
		CNfaRow &row = nfa.Back();

		if((content->nFlags & CF_NOCASE) && isalpha(*iter))
		{
			row[toupper(*iter)].PushBack(stateID);
			row[tolower(*iter)].PushBack(stateID);
		}
		else
		{
			row[*iter].PushBack(stateID);
		}

		++stateID;
	}

}

//没有depth或者within标记的
void contentToDefaultNFA(OPTIONCONTENT *content, CNfa &nfa)
{
	size_t mustCnt = 0;//经过的偏移字符个数
	if((content->nFlags & CF_OFFSET))
	{
		mustCnt = content->nOffset;

	}
	else if((content->nFlags & CF_DISTANCE))
	{
		mustCnt = content->nDistance;
	}

	for(size_t i = 0; i < mustCnt; ++i)
	{
		nfa.PushBack(CNfaRow());
		CNfaRow &row = nfa.Back();
		
		size_t id = i + 1;
		for(size_t j = 0; j < 256; ++j)
		{
			row[j].PushBack(id);
		}
	}

	size_t stateID = mustCnt;//从stateID开始进行content.vecconts的匹配
	size_t patternLen = content->vecconts.size(); 
	std::vector<BYTE> pattern;
	for(std::vector<BYTE>::const_iterator iter = content->vecconts.begin();
		iter != content->vecconts.end(); ++iter)
	{
		BYTE c = (content->nFlags & CF_NOCASE) ? tolower(*iter) : *iter;
		pattern.push_back(c);//如果content有nocase标记，则把模式串用字符的小写形式表示
	}

	//在前面加.*
	for(size_t i = 0; i < patternLen; ++i)
	{
		nfa.PushBack(CNfaRow());
		CNfaRow &row = nfa.Back();
		size_t id = stateID + i + 1;
		for(size_t c = 0; c < 256; ++c)
		{
			if(i == 0)
			{
				row[c].PushBack(stateID);
			}
			BYTE character = (content->nFlags & CF_NOCASE) ? tolower(c) : c;
			if(character == pattern[i])
			{
				row[c].PushBack(id);
			}
		}
	}
}

void content2Nfa(OPTIONCONTENT *content, CNfa &nfa)
{
	/*
	如果有depth或者within，则构造成线性的FA; 否则，构造成默认FA
	*/
	if((content->nFlags & CF_DEPTH) || (content->nFlags & CF_WITHIN))
	{
		contentToLinearNFA(content, nfa);
	}
	else
	{
		contentToDefaultNFA(content, nfa);
	}
}
CRECHANFA size_t InterpretRule(const CSnortRule &rule, CNfaTree &outTree)
{
	size_t flag = 0;
	outTree.PushBack(CNfaChain());
	for(size_t i = 0; i < rule.Size(); ++i)
	{
		OPTIONCONTENT *pContent = dynamic_cast<OPTIONCONTENT*>(rule[i]);
		OPTIONPCRE *pPcre = dynamic_cast<OPTIONPCRE*>(rule[i]);
		if(pContent != NULL)
		{
			if(!((pContent->nFlags & CF_DISTANCE) || (pContent->nFlags& CF_WITHIN)))
			{
				if(outTree.Back().Size() != 0)
				{
					outTree.PushBack(CNfaChain());
				}
			}
			outTree.Back().PushBack(CNfa());
			content2Nfa(pContent, outTree.Back().Back());
		}
		else if(pPcre != NULL)
		{
			if(!(pPcre->nFlags & PF_R))
			{
				if(outTree.Back().Size() != 0)
				{
					outTree.PushBack(CNfaChain());
				}
			}
			outTree.Back().PushBack(CNfa());
			flag = PcreToNFA(pPcre->strPattern.c_str(), outTree.Back().Back());
			if(flag != 0)
			{
				return flag;
			}
		}
	}
	return 0;
}

CRECHANFA void SerializeNfa(CNfaChain &nfaChain, CNfa &seriaNfa)
{
	CNfaRow oneSta;
	size_t staNum = seriaNfa.Size();
	for(size_t n = 0; n < nfaChain.Size(); ++n)
	{

		IncreNfaStaNum(seriaNfa.Size(), nfaChain[n]);
		for (size_t i = 0; i < nfaChain[n].Size(); ++i)
		{
			seriaNfa.PushBack(nfaChain[n][i]);
		}

		if(n != nfaChain.Size() - 1)
		{
			seriaNfa.Back()[EMPTYEDGE].PushBack(seriaNfa.Size());
			seriaNfa.PushBack(oneSta);
			seriaNfa.Back()[EMPTYEDGE].PushBack(seriaNfa.Size());
		}		

		staNum = seriaNfa.Size();
	}
}

