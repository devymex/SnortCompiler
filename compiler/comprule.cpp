/*!*
* @file				rule2nfa.cpp
**
* @author			Lab 435, Xidian University
**
* @brief			Support functions for transforming a rule to a nfa tree
**
* This nfa tree processing for rule options.
**
*/

#include "stdafx.h"
#include <hwprj\ruleoption.h>
#include "contopt.h"
#include <hwprj\pcreopt.h>
#include "comprule.h"

#pragma warning(disable:4996)

ISSPACE g_isSpace;

std::map<std::string, std::size_t> KeyTypeMap;

std::string type1Strs[] = //合法的格式"abc";
{
	"msg",
	"content",
	"uricontent"
};

std::string type2Strs[] = //合法的格式"ab\"c";
{
	"pcre"
};

std::string type3Strs[] = //合法的格式为;
{
	"reference",
	"gid",
	"sid",
	"rev",
	"classtype",
	"priority",
	"metadata",
	"nocase",
	"rawbytes",
	"depth",
	"offset",
	"distance",
	"within",
	"http_client_body",
	"http_cookie",
	"http_raw_cookie",
	"http_header",
	"http_raw_header",
	"http_method",
	"http_uri",
	"http_raw_uri",
	"http_stat_code",
	"http_stat_msg",
	"http_encode",
	"fast_pattern",
	"urilen",
	"isdataat",
	"pkt_data",
	"file_data",
	"base64_decode",
	"base64_data",
	"byte_test",
	"byte_jump",
	"byte_extract",
	"ftpbounce",
	"asn1",
	"cvs",
	"dce_iface",
	"dce_opnum",
	"dce_stub_data",
	"sip_method",
	"sip_stat_code",
	"sip_header",
	"sip_body",
	"gtp_type",
	"gtp_info",
	"gtp_version",
	"ssl_version",
	"ssl_state",
	"fragoffset",
	"ttl",
	"tos",
	"id",
	"ipopts",
	"fragbits",
	"dsize",
	"flags",
	"flow",
	"flowbits",
	"seq",
	"ack",
	"window",
	"itype",
	"icode",
	"icmp_id",
	"icmp_seq",
	"rpc",
	"ip_proto",
	"sameip",
	"stream_reassemble",
	"stream_size",
	"logto",
	"session",
	"resp",
	"react",
	"tag",
	"activates",
	"activated_by",
	"count",
	"replace",
	"detection_filter",
	"threshold"
};

/*! complie one rule

Arguments:
	parseRes		the parse result
  lpVoid		the compile result

Returns:		nothing

*/
ulong __stdcall CompileCallback(const PARSERESULT &parseRes, void *lpVoid)
{
	CCompileResults &result = *(CCompileResults*)lpVoid;
	
	result.GetSidDfaIds().PushBack(COMPILEDINFO());
	COMPILEDINFO &ruleResult = result.GetSidDfaIds().Back();

	ruleResult.m_nSid = parseRes.ulSid;
	ruleResult.m_nResult = COMPILEDINFO::RES_SUCCESS;

	if (parseRes.ulFlag & PARSEFLAG::PARSE_EMPTY)
	{
		ruleResult.m_nResult |= COMPILEDINFO::RES_EMPTY;
	}
	if (parseRes.ulFlag & PARSEFLAG::PARSE_ERROR)
	{
		ruleResult.m_nResult |= COMPILEDINFO::RES_OPTIONERROR;
	}
	if (parseRes.ulFlag & PARSEFLAG::PARSE_HASNOT)
	{
		ruleResult.m_nResult |= COMPILEDINFO::RES_HASNOT;
	}
	if (parseRes.ulFlag & PARSEFLAG::PARSE_HASBYTE)
	{
		ruleResult.m_nResult |= COMPILEDINFO::RES_HASBYTE;
	}

	if (ruleResult.m_nResult == COMPILEDINFO::RES_SUCCESS)
	{
		Rule2Dfas(parseRes.regRule, result);
	}

	return ruleResult.m_nResult;
}

/*!
* read rules from a file
*/
ulong LoadFile(const char *fileName, std::vector<std::string> &rules)
{
	std::ifstream fin(fileName);
	if (!fin)
	{
		std::cerr << "Open file Failed!" << std::endl;
		return (ulong)-1;
	}
	for (std::string strRule; std::getline(fin, strRule);)
	{
		 rules.push_back(strRule);
	}
	fin.close();
	rules.erase(
		std::remove_if(rules.begin(), rules.end(), ISEMPTYRULE()),
		rules.end());
	return 0;
}

std::string::iterator FindEndOther(std::string &rule, std::string::iterator iBeg)
{
	for (; ; ++iBeg)
	{
		iBeg = std::find(iBeg, rule.end(), ';');
		if (iBeg == rule.end())
		{
			TTHROW(TI_INVALIDDATA);
		}
		if (*(iBeg - 1) != '\\')
		{
			break;
		}
	}

	return iBeg;
}

std::string::iterator FindEndContent(std::string &rule, std::string::iterator iBeg)
{
	std::string::iterator iEnd = std::find(iBeg, rule.end(), '\"');
	if (iEnd == rule.end())
	{
		TTHROW(TI_INVALIDDATA);
	}
	iEnd = std::find(iEnd + 1, rule.end(), '\"');
	if (iEnd == rule.end())
	{
		TTHROW(TI_INVALIDDATA);
	}

	return FindEndOther(rule, iEnd + 1);
}

std::string::iterator FindEndPcre(std::string &rule, std::string::iterator iBeg)
{
	std::string::iterator iEnd = std::find(iBeg, rule.end(), '\"');
	if (iEnd == rule.end())
	{
		TTHROW(TI_INVALIDDATA);
	}
	for (; ;)
	{
		iEnd = std::find(iEnd + 1, rule.end(), '\"');
		if (iEnd == rule.end())
		{
			TTHROW(TI_INVALIDDATA);
		}
		if (*(iEnd - 1) != '\\')
		{
			break;
		}
	}
	
	return FindEndOther(rule, iEnd + 1);
}

void SplitOption(std::string &ruleOptions, std::vector<RULEOPTIONRAW> &options)
{
	if (KeyTypeMap.empty())
	{
		for (size_t i = 0; i < sizeof(type1Strs)/sizeof(type1Strs[0]); ++i)
		{
			KeyTypeMap[type1Strs[i]] = 1;
		}

		for (size_t i = 0; i < sizeof(type2Strs)/sizeof(type2Strs[0]); ++i)
		{
			KeyTypeMap[type2Strs[i]] = 2;
		}

		for (size_t i = 0; i < sizeof(type3Strs)/sizeof(type3Strs[0]); ++i)
		{
			KeyTypeMap[type3Strs[i]] = 3;
		}
	}

	struct ISKEYNAME
	{
		bool operator()(char c)
		{
			return isalnum(c) || c == '_';
		}
	};

	std::string::iterator iBeg = ruleOptions.begin();
	for (;;)
	{
		iBeg = std::find_if(iBeg, ruleOptions.end(), ISKEYNAME());
		if (iBeg == ruleOptions.end())
		{
			break;
		}
		std::string::iterator iEnd = std::find_if_not(iBeg, ruleOptions.end(), ISKEYNAME());
		if (iEnd == ruleOptions.end())
		{
			TTHROW(TI_INVALIDDATA);
		}

		std::string strName;
		for (std::string::iterator i = iBeg; i != iEnd; ++i)
		{
			strName.push_back(char(tolower(*i)));
		}

		iEnd = std::find_if_not(iEnd, ruleOptions.end(), ISSPACE());
		if (iEnd == ruleOptions.end())
		{
			TTHROW(TI_INVALIDDATA);
		}

		if (*iEnd == ':')
		{
			++iEnd;
		}
		else if (*iEnd != ';')
		{
			TTHROW(TI_INVALIDDATA);
		}

		iBeg = iEnd;
		switch (KeyTypeMap[strName])
		{
		case 1:
			iEnd = FindEndContent(ruleOptions, iEnd);
			break;
		case 2:
			iEnd = FindEndPcre(ruleOptions, iEnd);
			break;
		case 3:
			iEnd = FindEndOther(ruleOptions, iEnd);
			break;
		default:
			TTHROW(TI_INVALIDDATA);
		}

		RULEOPTIONRAW pr;
		pr.name = strName;
		pr.value = std::string(iBeg, iEnd);
		options.push_back(pr);
		iBeg = iEnd + 1;
	}
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

void ParseOptions(std::string &ruleOptions, CSnortRule &snortRule)
{
	std::vector<RULEOPTIONRAW> options;
	SplitOption(ruleOptions, options);

	ulong nContCnt = 0;
	nContCnt = std::count_if(options.begin(), options.end(), ISCONTENT());

	for(std::vector<RULEOPTIONRAW>::iterator iOp = options.begin(); iOp != options.end(); ++iOp)
	{
		STRING_ITER opValueBeg = iOp->value.begin();
		STRING_ITER opValueEnd = iOp->value.end();
		opValueBeg = std::find_if_not(opValueBeg, opValueEnd, ISSPACE());

		if (0 == stricmp("sid", iOp->name.c_str()))
		{
			long lSid = atoi(&*opValueBeg);
			if (lSid <= 0)
			{
				TTHROW(TI_INVALIDDATA);
			}
			snortRule.SetSid(ulong(lSid));
		}
		else if (0 == stricmp("pcre", iOp->name.c_str()))
		{
			CPcreOption pcreOpt;
			try
			{
				STRING strTmp(opValueBeg, opValueEnd);
				pcreOpt.FromPattern(CDllString(strTmp.c_str()));
				if (pcreOpt.HasFlags(CRuleOption::HASNOT))
				{
					snortRule.AddFlags(CSnortRule::HASNOT);
				}
			}
			catch (std::exception &e)
			{
				TTHROW(e.what());
			}
			catch (CTrace &e)
			{
				TTHROW(e.What());
			}
			snortRule.PushBack(&pcreOpt);
		}
		else if (0 == stricmp ("byte_jump", iOp->name.c_str()) ||
			0 == stricmp("byte_test", iOp->name.c_str()))
		{
			snortRule.AddFlags(CSnortRule::HASBYTE);
		}
		else if (0 == stricmp("content", iOp->name.c_str()) ||
			0 == stricmp("uricontent", iOp->name.c_str()))
		{
			CContentOption contOpt;
			try
			{
				STRING strTmp(opValueBeg, opValueEnd);
				contOpt.FromPattern(CDllString(strTmp.c_str()));
				if (contOpt.HasFlags(CRuleOption::HASNOT))
				{
					snortRule.AddFlags(CSnortRule::HASNOT);
				}
			}
			catch (std::exception &e)
			{
				TTHROW(e.what());
			}
			catch (CTrace &e)
			{
				TTHROW(e.What());
			}
			snortRule.PushBack(&contOpt);
		}
		else if (0 == stricmp("nocase", iOp->name.c_str()))
		{
			if (snortRule.Size() == 0)
			{
				TTHROW(TI_INVALIDDATA);
			}
			CContentOption *pCont = dynamic_cast<CContentOption*>(snortRule.Back());
			if (null == pCont)
			{
				TTHROW(TI_INVALIDDATA);
			}
			pCont->AddFlags(CContentOption::NOCASE);
		}
		else if (0 == stricmp("offset", iOp->name.c_str()))
		{
			if (snortRule.Size() == 0)
			{
				TTHROW(TI_INVALIDDATA);
			}
			CContentOption *pCont = dynamic_cast<CContentOption*>(snortRule.Back());
			if (null == pCont)
			{
				TTHROW(TI_INVALIDDATA);
			}
			pCont->m_nOffset = atoi(&*opValueBeg);
			pCont->AddFlags(CContentOption::OFFSET);
		}
		else if (0 == stricmp("depth", iOp->name.c_str()))
		{
			if (snortRule.Size() == 0)
			{
				TTHROW(TI_INVALIDDATA);
			}
			CContentOption *pCont = dynamic_cast<CContentOption*>(snortRule.Back());
			if (NULL == pCont)
			{
				TTHROW(TI_INVALIDDATA);
			}
			pCont->m_nDepth = atoi(&*opValueBeg);
			pCont->AddFlags(CContentOption::DEPTH);
		}
		else if (0 == stricmp("distance", iOp->name.c_str()))
		{
			if (snortRule.Size() == 0)
			{
				TTHROW(TI_INVALIDDATA);
			}
			CContentOption *pCont = dynamic_cast<CContentOption*>(snortRule.Back());
			if (NULL == pCont)
			{
				TTHROW(TI_INVALIDDATA);
			}
			pCont->m_nDistance = atoi(&*opValueBeg);
			pCont->AddFlags(CContentOption::DISTANCE);
		}
		else if (0 == stricmp("within", iOp->name.c_str()))
		{
			if (snortRule.Size() == 0)
			{
				TTHROW(TI_INVALIDDATA);
			}
			CContentOption *pCont = dynamic_cast<CContentOption*>(snortRule.Back());
			if (NULL == pCont)
			{
				TTHROW(TI_INVALIDDATA);
			}
			pCont->m_nWithin = atoi(&*opValueBeg);
			pCont->AddFlags(CContentOption::WITHIN);
		}
	}
}

/*!
* NAME
*  Rule2PcreList::
*/
/*!*
* This function converts a CSnortRule to a CRegRule and extract signatures from content option
**
* According to the constraints of rule options, we split a snort rule into some option chains.
* For every option chain, the datapacket matchs from the first byte.
* Then we transfrom every option into pcre.
**
* @param rule		 a CSnortRule object which contains the original information
* 				of a snort rule. 
* @param regrule	 the transformed CRegRule object which makes up of a number of pcre lists
* 					and the signatures in every pcre list.
**
* @return integer
**
* @retval  0 function successful
* @retval <>0 fatal error
*/

void Rule2RegRule(const CSnortRule &rule, CRegRule &regRule)
{
	regRule.Reserve(SC_CHAINRESERV);
	regRule.PushBack(CPcreChain());
	CPcreChain *pCurChain = &regRule.Back();
	CPcreOption pcreOpt;

	for(ulong i = 0; i < rule.Size(); ++i)
	{
		CPcreOption *pPcre = dynamic_cast<CPcreOption*>(rule[i]);
		if (pPcre == null)
		{
			CContentOption *pCont = dynamic_cast<CContentOption*>(rule[i]);
			TASSERT(pCont != NULL);
			pCont->ToPcre(pcreOpt);
			pPcre = &pcreOpt;
		}
		if(!pPcre->HasFlags(CPcreOption::PF_R) && pCurChain->Size() != 0)
		{
			regRule.PushBack(CPcreChain());
			pCurChain = &regRule.Back();
		}
		pCurChain->PushBack(*pPcre);
	}

	if (regRule.Back().Size() == 0)
	{
		regRule.PopBack();
	}
}


/*! assign all the signatures of each rule to all its option list

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

bool SeqIncBy(const CRegRule &regRule, const RULESEQUENCE &ruleSeq, ulong ulIdx)
{
	TASSERT(ulIdx < ruleSeq.size());
	TASSERT(ruleSeq[ulIdx].size() == 1);
	TASSERT(ruleSeq[ulIdx].front().size() == 1);

	const CByteArray &seq = ruleSeq[ulIdx].front().front();
	for (ulong i = 0; i < ruleSeq.size(); ++i)
	{
		if (ulIdx != i)
		{
			const CHAINSEQUENCE &curChainSeq = ruleSeq[i];
			for (ulong j = 0; j < curChainSeq.size(); ++j)
			{
				const PCRESEQUENCE &curPcreSeq = curChainSeq[j];
				bool bCaseless = true;
				// as long as the content has nocase mod
				// a caseless comparation can be executed.
				if (!regRule[ulIdx][0].HasFlags(CPcreOption::PF_i))
				{
					// if the content is case-sensitive and the
					// pcre is case-less, this content can't be erased
					if (regRule[i][j].HasFlags(CPcreOption::PF_i))
					{
						continue;
					}
					// if the pcre is case-sensitive, a case-sensitive
					// comparation shoud be executed.
					bCaseless = false;
				}

				// if the content has no '^', comparation should be excuted
				if (regRule[ulIdx][0].HasFlags(CPcreOption::PF_A))
				{
					// if the content has a '^', and the pcre has no '^',
					// this content can't be erased.
					if (!regRule[i][j].HasFlags(CPcreOption::PF_A))
					{
						continue;
					}
					// if the pcre has '^', but also has a '/m' mod,
					// this content can't be erased.
					if (regRule[i][j].HasFlags(CPcreOption::PF_m))
					{
						continue;
					}
				}
				if (std::find_if(curPcreSeq.cbegin(), curPcreSeq.cend(),
					INCLUDESEQUENCE(seq, bCaseless)) != curPcreSeq.cend())
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool IsOneContentChain(const CPcreChain &chain, const CHAINSEQUENCE &chainSeq)
{
	if (chain.Size() != 1)
	{
		return false;
	}
	if (chainSeq.size() != 1)
	{
		return false;
	}
	if (!chain[0].HasFlags(CPcreOption::PF_F))
	{
		return false;
	}
	if (chainSeq[0].size() != 1)
	{
		return false;
	}
	return true;
}

void PreCompileRule(const CRegRule &regRule,
					RULESEQUENCE &ruleSeq,
					RULECOMPDATA &ruleCompData)
{
	for (ulong i = 0; i < regRule.Size(); ++i)
	{
		ruleSeq.push_back(CHAINSEQUENCE());
		ruleCompData.push_back(CHAINCOMPDATA());

		CHAINSEQUENCE &chainSeq			= ruleSeq.back();
		CHAINCOMPDATA &chainCompData	= ruleCompData.back();

		const CPcreChain &curPcreChain = regRule[i];

		for (ulong j = 0; j < curPcreChain.Size(); ++j)
		{
			chainCompData.push_back(CByteArray());
			chainSeq.push_back(PCRESEQUENCE());
			curPcreChain[j].Precompile(chainCompData.back());
			ExtractSequence(chainCompData.back(), chainSeq.back());
		}
	}
}

void ProcessRule(CRegRule &regRule, RULECOMPDATA &result)
{
	RULESEQUENCE ruleSeq;
	PreCompileRule(regRule, ruleSeq, result);
	for (ulong i = 0; i < regRule.Size();)
	{
		CPcreChain &curChain = regRule[i];
		CHAINSEQUENCE &curChainSeq = ruleSeq[i];

		bool bErased = false;
		if (IsOneContentChain(curChain, curChainSeq))
		{
			if (SeqIncBy(regRule, ruleSeq, i))
			{
				regRule.Erase(i);
				result.erase(result.begin() + i);
				ruleSeq.erase(ruleSeq.begin() + i);
				bErased = true;
				g_log << "Erased one chain" << g_log.nl;
			}
		}
		if (!bErased)
		{
			for (ulong j = 0; j < curChainSeq.size(); ++j)
			{
				PCRESEQUENCE &curPcreSeq = curChainSeq[j];
				for (ulong k = 0; k < curPcreSeq.size(); ++k)
				{
					ExtractSignatures(curPcreSeq[k], regRule[i].GetSigs());
				}
			}
			regRule[i].GetSigs().Unique();
			++i;
		}
	}
}

/*! complie one rule to several dfas

Arguments:
  rule		the snort rule
  result		the compile result
  ruleResult  the relationship between sid and dfa ids

Returns:		nothing

*/
void Rule2Dfas(const CRegRule &rule, CCompileResults &result)
{
	CRegRule regRule = rule;

	COMPILEDINFO &ruleResult = result.GetSidDfaIds().Back();

	RULECOMPDATA ruleCompData;
	try
	{
		ProcessRule(regRule, ruleCompData);
	}
	catch (CTrace &)
	{
		ruleResult.m_nResult |= COMPILEDINFO::RES_PCREERROR;
		return;
	}

	const ulong nOldDfaSize = result.GetDfaTable().Size();

	const ulong nCurRuleSize = regRule.Size();
	result.GetDfaTable().Resize(nOldDfaSize + nCurRuleSize);

	const ulong nOldRegexSize = result.GetRegexTbl().Size();
	result.GetRegexTbl().Resize(nOldRegexSize + nCurRuleSize);

	bool bHasSigs = false;
	static CNfa nfa;
	nfa.Reserve(5000);
	for (ulong i = 0; i < nCurRuleSize; ++i)
	{
		CPcreChain &curPcreChain = regRule[i];

		nfa.Clear();
		for (ulong j = 0; j < curPcreChain.Size(); ++j)
		{
			const CPcreOption &curPcre = curPcreChain[j];
			try
			{
				CodeToNFA(ruleCompData[i][j],
					curPcre.HasFlags(CPcreOption::PF_A), nfa);
			}
			catch (CTrace &e)
			{
				e;
				ruleResult.m_nResult |= COMPILEDINFO::RES_PCREERROR;
				nfa.Clear();
				break;
			}
		}
		if (ruleResult.m_nResult != COMPILEDINFO::RES_SUCCESS)
		{
			break;
		}
		if (regRule[i].GetSigs().Size() > 0)
		{
			bHasSigs = true;
		}

		ulong nDfaId = nOldDfaSize + i;
		ulong nChainId = nOldRegexSize + i;

		CDfa &dfa = result.GetDfaTable()[nDfaId];

		dfa.SetId(nDfaId);
		ulong nToDFAFlag = dfa.FromNFA(nfa);

		if (nToDFAFlag == -1)
		{
			ruleResult.m_nResult |= COMPILEDINFO::RES_EXCEEDLIMIT;
			dfa.Clear();
			break;
		}

		TASSERT(dfa.GetFinalStates().Size() != 0);

		ulong nr = dfa.Minimize();
		if (0 != nr || dfa.Size() > SC_MAXDFASIZE)
		{
			ruleResult.m_nResult |= COMPILEDINFO::RES_EXCEEDLIMIT;
			dfa.Clear();
			break;
		}

		ruleResult.m_dfaIds.PushBack(nDfaId);
		result.GetRegexTbl()[nChainId] = regRule[i];
	}

	if (!bHasSigs)
	{
		ruleResult.m_nResult |= COMPILEDINFO::RES_HASNOSIG;
	}

	if (ruleResult.m_nResult == COMPILEDINFO::RES_SUCCESS)
	{
		AssignSig(result, nOldRegexSize, nOldRegexSize + nCurRuleSize);
	}
	else
	{
		ruleResult.m_dfaIds.Clear();
		result.GetDfaTable().Resize(nOldDfaSize);
		result.GetRegexTbl().Resize(nOldRegexSize);
	}
}
