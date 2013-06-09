/**
**  @file        rule2nfa.cpp
**
**  @author      Lab 435, Xidian University
**
**  @brief       Support functions for transforming a rule to a nfa tree
**
**  This nfa tree processing for rule options.
**
*/

#include "stdafx.h"
#include "rule2nfa.h"
#include "stdafx.h"

#define nfaTreeReserve 100
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
**	CONBYTE: transform content to vector<BYTE>
**	CONPCRE: transform content to string
*/
enum CONTYPE
{
	CONBYTE,
	CONPCRE
};

class CRuleOption
{
public:
	CRuleOption();
	CRuleOption(const CRuleOption &other);
	const CRuleOption& operator = (const CRuleOption &other);
	virtual ~CRuleOption();

	void SetPattern(LPCSTR lpStr);
	size_t GetPattern(LPSTR lpStr, size_t nLen) const;

	size_t GetFlag() const;
	void SetFlag(size_t nFlag);
	void AddFlag(size_t nFlag);
	BOOL TestFlag(size_t nFlag) const;

protected:
	size_t m_nFlag;
	std::string *m_pPattern;
};

CRuleOption::CRuleOption()
	: m_nFlag(0)
{
	m_pPattern = new std::string;
}

CRuleOption::CRuleOption(const CRuleOption &other)
{
	m_pPattern = new std::string;
	*this = other;
}

CRuleOption::~CRuleOption()
{
	delete m_pPattern;
}

const CRuleOption& CRuleOption::operator=(const CRuleOption &other)
{
	*m_pPattern = *other.m_pPattern;
	m_nFlag = other.m_nFlag;
	return *this;
}

size_t CRuleOption::GetPattern(LPSTR lpStr, size_t nLen) const
{
	if (lpStr == NULL || nLen == 0)
	{
		return m_pPattern->length();
	}
	if (nLen > m_pPattern->length())
	{
		nLen = m_pPattern->length();
	}
	CopyMemory(lpStr, &(*m_pPattern)[0], nLen);
	return nLen;
}

void CRuleOption::SetPattern(LPCSTR lpStr)
{
	*m_pPattern = lpStr;
}

size_t CRuleOption::GetFlag() const
{
	return m_nFlag;
}

void CRuleOption::SetFlag(size_t nFlag)
{
	m_nFlag = nFlag;
}

void CRuleOption::AddFlag(size_t nFlag)
{
	m_nFlag |= nFlag;
}

BOOL CRuleOption::TestFlag(size_t nFlag) const
{
	return ((m_nFlag & nFlag) != 0);
}

struct OPTIONCONTENT : public CRuleOption
{
	std::vector<BYTE> vecconts;//content data
	int nOffset;// offset constraint in snort rule
	int nDepth;//depth constraint in snort rule
	int nDistance;//distance constraint in snort rule
	int nWithin;//within constraint in snort rule
};

struct OPTIONPCRE : public CRuleOption
{
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
size_t LoadFile(LPCTSTR fileName, std::vector<std::string> &rules)
{
	std::ifstream fin(fileName);
	if(!fin)
	{
		std::cerr << "Open file Failed!" << std::endl;
		return (size_t)-1;
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
	pcre.SetFlag(0);

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

inline BYTE HexByte(const char *p2Bytes)
{
	return (HexBit(p2Bytes[0]) << 4 | HexBit(p2Bytes[1]));
}

//The string type is converted into unsigned char type
template<typename _Iter>
size_t FormatOptionContent (_Iter cBeg, _Iter cEnd, std::vector<BYTE> &content, CONTYPE TYPE)
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


size_t ProcessOption(std::string &ruleOptions, CSnortRule &snortRule)
{
	std::vector<RULEOPTIONRAW> options;
	ExtractOption(ruleOptions, options);

	size_t nCONT = 0;
	size_t sum = 0;
	nCONT = std::count_if(options.begin(), options.end(), CONTENTNUM());

	//Mark process mode, "0" is error ,"1" is normal
	size_t nResult = 0;
	size_t nFlag = 0;

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
			size_t nr = FormatPcre(opValueBeg, opValueEnd, *pPcre);
			if (nr != 0)
			{
				if (nr == size_t(-2))
				{
					nFlag |= CSnortRule::RULE_HASNOT;
				}
				else
				{
					nResult = size_t(-1);
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
				nResult = size_t(-1);
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
			size_t last = snortRule.Size() - 1;
			OPTIONCONTENT * temp = dynamic_cast<OPTIONCONTENT *>(snortRule[last]);
			if (NULL == temp)
			{
				nResult = size_t(-1);
				break;
			}
			else
			{
				temp->AddFlag(CF_NOCASE);
			}
		}
		else if (0 == stricmp("offset", iOp->name.c_str()))
		{
			size_t last = snortRule.Size() - 1;
			OPTIONCONTENT *pTemp = dynamic_cast<OPTIONCONTENT*>(snortRule[last]);
			if (NULL == pTemp)
			{
				nResult = size_t(-1);
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
			size_t last = snortRule.Size() - 1;
			OPTIONCONTENT *pTemp = dynamic_cast<OPTIONCONTENT *>(snortRule[last]);
			if (NULL == pTemp)
			{
				nResult = size_t(-1);
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
			size_t last = snortRule.Size() - 1;
			OPTIONCONTENT *pTemp = dynamic_cast<OPTIONCONTENT *>(snortRule[last]);
			if (NULL == pTemp)
			{
				nResult = size_t(-1);
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
			size_t last = snortRule.Size() - 1;
			OPTIONCONTENT *pTemp = dynamic_cast<OPTIONCONTENT *>(snortRule[last]);
			if (NULL == pTemp)
			{
				nResult = size_t(-1);
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
* process one rule
*/
CRECHANFA void CompileRule(LPCSTR rule, RECIEVER recv, LPVOID lpUser)
{
	//Delete the rule header, reserve the rule options
	std::string strRule(rule);
	strRule.erase(strRule.begin(), find(strRule.begin(), strRule.end(), '(') + 1);
	strRule.erase(find(strRule.rbegin(), strRule.rend(), ')').base() - 1, strRule.end());

	CSnortRule snortRule;
	if (0 == ProcessOption(strRule, snortRule))
	{
		recv(snortRule, lpUser);
	}
}

/*
* read rules from a file
* then process the rules to CSnortRule
* callback function RECIEVER to handle CSnortRule
*/

CRECHANFA size_t CompileRuleSet(LPCTSTR fileName, RECIEVER recv, LPVOID lpUser)
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
				std::cout << rIt - rules.begin() + 1 << std::endl;
				//std::cout << ": " << g_dTimer << std::endl;
				CompileRule(rIt->c_str(), recv, lpUser);
			}
		}
	}
	return 0;
}

/*
* content has depth or within constraint
* construct it to linear NFA
*/
void contentToLinearNFA(OPTIONCONTENT *pContent, CNfa &nfa)
{
	nfa.Reserve(nfaReserve);
	size_t state_size = 0;

	size_t patternLen = pContent->vecconts.size();
	size_t mustCnt = 0, maxCnt = 0;//mustCnt和maxCnt分别代表经过任意字符跳转出现的最少和最多次数
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
	size_t stateID = mustCnt + maxCnt;//从stateID开始进行content字符串的跳转

	//mustCnt：0-255任意字符跳转，必须要跳转的状态数
	for(size_t i = 0; i < (mustCnt + maxCnt); ++i)
	{
		nfa.Resize(++state_size);
		//nfa.PushBack(CNfaRow());
		CNfaRow &row = nfa.Back();
		size_t id = i + 1;
		for(int j = 0; j < 256; ++j)
		{
			row.AddDest(j, id);
		}
		if(i >= mustCnt)
		{
			row.AddDest(DFACOLSIZE, stateID);
		}
	}

	++stateID;
	for(std::vector<BYTE>::const_iterator iter = pContent->vecconts.begin();
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
	size_t state_size = 0;

	size_t mustCnt = 0;//经过的偏移字符个数
	if((pContent->GetFlag() & CF_OFFSET))
	{
		mustCnt = pContent->nOffset;

	}
	else if((pContent->GetFlag() & CF_DISTANCE))
	{
		mustCnt = pContent->nDistance;
	}

	for(size_t i = 0; i < mustCnt; ++i)
	{
		nfa.Resize(++state_size);
		//nfa.PushBack(CNfaRow());
		CNfaRow &row = nfa.Back();
		
		size_t id = i + 1;
		for(size_t j = 0; j < 256; ++j)
		{
			row.AddDest(j, id);
		}
	}

	size_t stateID = mustCnt;//从stateID开始进行content.vecconts的匹配
	size_t patternLen = pContent->vecconts.size(); 
	std::vector<BYTE> pattern;
	for(std::vector<BYTE>::const_iterator iter = pContent->vecconts.begin();
		iter != pContent->vecconts.end(); ++iter)
	{
		BYTE c = BYTE((pContent->GetFlag() & CF_NOCASE) ? tolower(*iter) : *iter);
		pattern.push_back(c);//如果content有nocase标记，则把模式串用字符的小写形式表示
	}

	//在前面加.*
	for(size_t i = 0; i < patternLen; ++i)
	{
		nfa.Resize(++state_size);
		//nfa.PushBack(CNfaRow());
		CNfaRow &row = nfa.Back();
		size_t id = stateID + i + 1;
		for(size_t c = 0; c < 256; ++c)
		{
			if(i == 0)
			{
				row.AddDest(c, stateID);
			}
			BYTE character = BYTE((pContent->GetFlag() & CF_NOCASE) ? tolower(c) : c);
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
	size_t stateNum = nfa.Size();
	std::ofstream fout(fileName);
	fout << "\t";
	for(size_t t = 0; t < 257; ++t)
	{
		fout << t << "\t";
	}
	fout << std::endl;
	for(size_t i = 0; i < stateNum; ++i)
	{
		const CNfaRow &row = nfa[i];
		fout << i << "\t";
		for(size_t j = 0; j < 257; ++j)
		{
			size_t nCnt = row.DestCnt(j);
			if(nCnt == 0)
			{
				fout << -1 << "\t";
			}
			else
			{
				for(size_t k = 0; k < nCnt; ++k)
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
**	@param pContent    pointer to the original content option
**	@param pcreStr	   the transformed pcre
**
**	@return
**  @retval  0 function successful
**  @retval -1 fatal error
*/
size_t content2Pcre(OPTIONCONTENT *pContent, CCString &pcreStr)
{
	std::stringstream ss;
	pcreStr = CCString("/^");
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
			pcreStr = CCString("/");
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
	std::vector<BYTE> conVec;

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
**  NAME
**    Rule2PcreList::
*/
/**
**  This function converts a CSnortRule to a CRegRule and extract signatures from content option
**
**	According to the constraints of rule options, we split a snort rule into some option chains.
**	For every option chain, the datapacket matchs from the first byte.
**	Then we transfrom every option into pcre.
**
**  @param rule       a CSnortRule object which contains the original information
**					  of a snort rule. 
**  @param regrule    the transformed CRegRule object which makes up of a number of pcre lists
**                    and the signatures in every pcre list.
**
**  @return integer
**
**  @retval  0 function successful
**  @retval <>0 fatal error
*/

CRECHANFA CSnortRule::CSnortRule()
	: m_nSid(0), m_nFlag(0)
{
	m_pOptions = new std::vector<CRuleOption*>;
}

CRECHANFA CSnortRule::CSnortRule(const CSnortRule &other)
{
	m_pOptions = new std::vector<CRuleOption*>;
	*this = other;
}

CRECHANFA const CSnortRule& CSnortRule::operator = (const CSnortRule &other)
{
	m_nSid = other.m_nSid;
	m_nFlag = other.m_nFlag;
	*m_pOptions = *other.m_pOptions;
	return *this;
}

CRECHANFA CSnortRule::~CSnortRule()
{
	std::vector<CRuleOption*> &opts = *m_pOptions;
	for (std::vector<CRuleOption*>::iterator i = opts.begin(); i != opts.end(); ++i)
	{
		delete *i;
	}
	opts.clear();
	delete m_pOptions;
}

CRECHANFA size_t CSnortRule::Size() const
{
	return m_pOptions->size();
}

CRECHANFA void CSnortRule::SetSid(size_t sid)
{
	m_nSid = sid;
}

CRECHANFA size_t CSnortRule::GetSid() const
{
	return m_nSid;
}

CRECHANFA void CSnortRule::SetFlag(size_t flag)
{
	m_nFlag = flag;
}

CRECHANFA size_t CSnortRule::GetFlag() const
{
	return m_nFlag;
}

CRECHANFA void CSnortRule::PushBack(CRuleOption* ruleoption)
{
	m_pOptions->push_back(ruleoption);
}

CRECHANFA void CSnortRule::PopBack()
{
	m_pOptions->pop_back();
}

CRECHANFA CRuleOption* CSnortRule::operator[](size_t nIdx) const
{
	return (*m_pOptions)[nIdx];
}

CRECHANFA size_t Rule2PcreList(const CSnortRule &rule, CRegRule &regrule)
{
	regrule.Reserve(nfaTreeReserve);
	size_t regChain_size = 0;
	regrule.Resize(++regChain_size);
	int cFlag = 0;

	for(size_t i = 0; i < rule.Size(); ++i)
	{
		OPTIONCONTENT *pContent = dynamic_cast<OPTIONCONTENT*>(rule[i]);
		OPTIONPCRE *pPcre = dynamic_cast<OPTIONPCRE*>(rule[i]);
		
		if(pContent != NULL)
		{
			if(!(pContent->TestFlag(CF_DISTANCE) || pContent->TestFlag(CF_WITHIN)))
			{
				if(regrule.Back().Size() != 0)
				{
					regrule.Resize(++regChain_size);
				}
			}
			CCString conPcreStr;

			//transfrom content to pcre
			cFlag = content2Pcre(pContent, conPcreStr);
			if(cFlag != 0)
			{
				return cFlag;
			}
			if(pContent->vecconts.size() >= 4)
			{
				//extract signatures
				std::vector<BYTE> contentTmp;
				contentTmp.reserve(pContent->vecconts.size());
				for(std::vector<BYTE>::iterator itTmp = pContent->vecconts.begin();
					itTmp != pContent->vecconts.end(); ++itTmp)
				{
					BYTE c = *itTmp;
					if (c >= 'A' && c <= 'Z')
					{
						contentTmp.push_back(c - 'A' + 'a');
					}
					else
					{
						contentTmp.push_back(c);
					}
				}

				for(std::vector<BYTE>::iterator sigIt = contentTmp.begin();
					sigIt + 3 != contentTmp.end(); ++sigIt)
				{
					SIGNATURE sig = *(SIGNATURE*)&(*sigIt);
					regrule.Back().GetSigs().PushBack(sig);
				}
			}
			regrule.Back().PushBack(conPcreStr);
		}
		else if(pPcre != NULL)
		{
			if(!(pPcre->GetFlag() & PF_R))
			{
				if(regrule.Back().Size() != 0)
				{
					regrule.Resize(++regChain_size);
				}
			}
			std::string tmpStr;
			tmpStr.resize(pPcre->GetPattern(NULL, 0));
			pPcre->GetPattern(&tmpStr[0], tmpStr.size());
			CCString strPattern(tmpStr.c_str());
			regrule.Back().PushBack(strPattern);
		}
	}

	//regrule.Reserve(++regChain_size);
	for(size_t i = 0; i < regrule.Size(); ++i)
	{
		if(regrule[i].GetSigs().Size() > 1)
		{
			regrule[i].GetSigs().Unique();
		}
	}
	return 0;
}


/*
**  NAME
**    CRegChainToNFA::
*/
/**
**  This function converts a CRegChain to a CNfa
**
**	use pcre library to construct a nfa from a pcre
**	
**  @param regchain   a CRegChain object which contains a pcre list
**  @param nfa        the transformed CNfa object 
**
**  @return integer
**
**  @retval  0 function successful
**  @retval <>0 fatal error
*/

CRECHANFA size_t CRegChainToNFA(CRegChain &regchain, CNfa &nfa)
{
	nfa.Reserve(nfaReserve);
	int flag = 0;
	for(size_t i = 0; i < regchain.Size(); ++i)
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



CRECHANFA CRegRule::CRegRule()
{
	m_pRegVec = new std::vector<CRegChain>;
}

CRECHANFA CRegRule::~CRegRule()
{
	delete m_pRegVec;
}

CRECHANFA CRegRule::CRegRule(const CRegRule &other)
{
	m_pRegVec = new std::vector<CRegChain>;
	*this = other;
}

CRECHANFA size_t CRegRule::Size() const
{
	return m_pRegVec->size();
}

CRECHANFA CRegChain& CRegRule::Back() const
{
	return m_pRegVec->back();
}

CRECHANFA void CRegRule::Reserve(size_t nCount)
{
	m_pRegVec->reserve(nCount);
}
CRECHANFA void CRegRule::Resize(size_t nSize)
{
	m_pRegVec->resize(nSize);
}

CRECHANFA void CRegRule::PushBack(const CRegChain &nRegChain)
{
	m_pRegVec->push_back(nRegChain);
}

CRECHANFA CRegChain& CRegRule::operator[](size_t nIdx)
{
	return (*m_pRegVec)[nIdx];
}

CRECHANFA const CRegChain& CRegRule::operator[](size_t nIdx) const
{
	return (*m_pRegVec)[nIdx];
}

CRECHANFA const CRegRule& CRegRule::operator = (const CRegRule &other)
{
	*this->m_pRegVec = *other.m_pRegVec;
	return *this;
}

CRECHANFA CRegChain::CRegChain()
{
	m_pRegList = new std::vector<CCString>;
}

CRECHANFA CRegChain::~CRegChain()
{
	delete m_pRegList;
}

CRECHANFA CRegChain::CRegChain(const CRegChain &other)
{
	m_pRegList = new std::vector<CCString>;
	*this = other;
}

CRECHANFA size_t CRegChain::Size() const
{
	return m_pRegList->size();
}

CRECHANFA CCString& CRegChain::Back() const
{
	return m_pRegList->back();
}

CRECHANFA void CRegChain::PushBack(const CCString &pcreStr)
{
	m_pRegList->push_back(pcreStr);
}

CRECHANFA CCString& CRegChain::operator[](size_t nIdx)
{
	return (*m_pRegList)[nIdx];
}

CRECHANFA const CRegChain& CRegChain::operator = (const CRegChain &other)
{
	*this->m_pRegList = *other.m_pRegList;
	this->m_sigs = other.m_sigs;
	return *this;
}

CRECHANFA void CRegChain::Resize(size_t nSize)
{
	m_pRegList->resize(nSize);
}

CRECHANFA CSignatures& CRegChain::GetSigs()
{
	return m_sigs;
}

CRECHANFA const CSignatures& CRegChain::GetSigs() const
{
	return m_sigs;
}
