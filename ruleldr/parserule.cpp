#include "stdafx.h"
#include "parserule.h"

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

DWORD ProcessOption(std::string &ruleOptions, CSnortRule &snortRule)
{
	std::vector<RULEOPTIONRAW> options;
	ExtractOption(ruleOptions, options);

	//Mark process mode, "0" is error ,"1" is normal
	DWORD dwResult = DWORD(-1);

	//snortRule.nFlags = 0;

	//Read one rule and only store "sid","pcre","content" and related options

	size_t nFlag = 0;
	if (std::find_if (options.begin(), options.end(), SEARCHBYTEOPTION()) == options.end())
	{
		nFlag |= CSnortRule::RULE_HASBYTE;
	}
	for(std::vector<RULEOPTIONRAW>::iterator i = options.begin(); ;)
	{
		std::vector<RULEOPTIONRAW>::iterator iBeg = std::find_if(i, options.end(), SEARCHOPTIONS());
		if (iBeg == options.end())
		{
			dwResult = 0;
			break;
		}
		std::vector<RULEOPTIONRAW>::iterator iEnd = std::find_if(iBeg + 1, options.end(), SEARCHOPTIONS());		

		if (0 == stricmp("sid", iBeg->name.c_str()))
		{
			std::string::iterator opValueBeg = iBeg->value.begin();
			std::string::iterator opValueEnd = iBeg->value.end();
			opValueBeg = std::find_if_not(opValueBeg, opValueEnd, ISSPACE());
			//*(int*)(&snortRule.Sid) = atoi(&*opValueBeg);
			snortRule.SetSid(atoi(&*opValueBeg));
		}
		else if (0 == stricmp("pcre", iBeg->name.c_str()))
		{
			std::string::iterator opValueBeg = iBeg->value.begin();
			std::string::iterator opValueEnd = iBeg->value.end();
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
		else if (0 == stricmp("content", iBeg->name.c_str()) ||
			0 == stricmp("uricontent", iBeg->name.c_str()))
		{
			OPTIONCONTENT *pContent = new OPTIONCONTENT;
			size_t nr = ExtractOptionContent(iBeg, iEnd, *pContent);
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
		i = iEnd;
	}
	snortRule.SetFlag(nFlag);
	return dwResult;
}


/*
* read rules from a file
* then process the rules to CSnortRule
* callback function RECIEVER to handle CSnortRule
*/
PARSERULE size_t ParseRule(LPCTSTR fileName, RECIEVER recv, LPVOID lpUser)
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
