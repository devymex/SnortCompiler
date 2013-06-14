/**
**	@file		compilernew.cpp
**
**	@author		Lab 435, Xidian University
**
**	@brief		Support functions for compile rule to dfa
**
**	This implements compile rule to dfa algorithm
**
*/

#include "stdafx.h"
#include <hwprj\snortrule.h>
#include "comprule.h"
#include <hwprj\compiler.h>

#define nfaTreeReserve 100


/* complie one rule

Arguments:
  rule		the snort rule
  lpVoid		the compile result

Returns:		nothing

*/
void __stdcall Process(const CSnortRule &rule, void *lpVoid)
{
	CCompileResults &result = *(CCompileResults*)lpVoid;
	ulong nFlag = rule.GetFlag();
	ulong nNewSize = result.GetSidDfaIds().Size() + 1;
	result.GetSidDfaIds().Resize(nNewSize);
	COMPILEDRULE &ruleResult = result.GetSidDfaIds().Back();
	ruleResult.m_nSid = rule.GetSid();
	if (rule.Size() == 0)
	{
		ruleResult.m_nResult = COMPILEDRULE::RES_EMPTY;
		return;
	}
	else if (nFlag & CSnortRule::RULE_HASNOT)
	{
		ruleResult.m_nResult = COMPILEDRULE::RES_HASNOT;
		return;
	}
	else if (nFlag & CSnortRule::RULE_HASBYTE)
	{
		ruleResult.m_nResult = COMPILEDRULE::RES_HASBYTE;
		return;
	}
	else
	{
		Rule2Dfas(rule, result, ruleResult);
	}
}

/* complie all rules

Arguments:
  filename	 path of the file where all rules are saved in
  result		the compile result

Returns:		nothing

*/

COMPILERHDR void CompileRuleSet(const char *filename, CCompileResults &result)
{
	CompileFile(filename, Process, &result);

	std::cout << "rule2pcretime:" << rule2pcretime << std::endl;
	std::cout << "pcre2nfatime:" << pcre2nfatime << std::endl;
	std::cout << "nfa2dfatime:" << nfa2dfatime << std::endl;
	std::cout << "dfamintimetime:" << dfamintimetime << std::endl;
}

/*
* read rules from a file
* then process the rules to CSnortRule
* callback function RECIEVER to handle CSnortRule
*/
COMPILERHDR ulong CompileFile(const char *fileName, RECIEVER recv, void *lpUser)
{
	if(recv == NULL)
	{
		return (ulong)-1;
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


/* complie one rule to several dfas

Arguments:
  rule		the snort rule
  result		the compile result
  ruleResult  the relationship between sid and dfa ids

Returns:		nothing

*/
COMPILERHDR void Rule2Dfas(const CSnortRule &rule, CCompileResults &result,
							COMPILEDRULE &ruleResult)
{
	CRegRule regrule;
	CTimer ctime;//for test
	ctime.Reset();//for test

	ulong flag = Rule2PcreList(rule, regrule);
	rule2pcretime += ctime.Reset();//for test

	if (flag == SC_ERROR)
	{
		ruleResult.m_nResult = COMPILEDRULE::RES_ERROR;
		return;
	}
	else
	{
		ruleResult.m_nResult = COMPILEDRULE::RES_SUCCESS;
		const ulong nDfaTblSize = result.GetDfaTable().Size();
		const ulong nIncrement = regrule.Size();
		result.GetDfaTable().Resize(nDfaTblSize + nIncrement);
		const ulong nRegexTblSize = result.GetRegexTbl().Size();
		result.GetRegexTbl().Resize(nRegexTblSize + nIncrement);
		ulong nDfaId;
		ulong nChainId;
		bool bHasSigs = false;
		for (ulong i = 0; i < nIncrement; ++i)
		{
			CNfa nfa;

			ctime.Reset();//for test
			ulong nToNFAFlag = CRegChainToNFA(regrule[i], nfa);
			pcre2nfatime += ctime.Reset();//for test

			if (regrule[i].GetSigs().Size() > 0)
			{
				bHasSigs = true;
			}

			nDfaId = nDfaTblSize + i;
			nChainId = nRegexTblSize + i;
			CDfa &dfa = result.GetDfaTable()[nDfaId];
			if (nToNFAFlag == SC_ERROR)
			{
				ruleResult.m_nResult = COMPILEDRULE::RES_ERROR;
				ruleResult.m_dfaIds.Clear();
				result.GetDfaTable().Resize(nDfaTblSize);
				result.GetRegexTbl().Resize(nRegexTblSize);
				return;
			}
			else
			{
				ctime.Reset();//for test
				dfa.SetId(nDfaId);
				ulong nToDFAFlag = dfa.FromNFA(nfa);
				nfa2dfatime += ctime.Reset();//for test

				if (nToDFAFlag == -1)
				{
					ruleResult.m_nResult = COMPILEDRULE::RES_EXCEEDLIMIT;
					dfa.Clear();
				}
				else
				{
					ctime.Reset();//for test
					ulong nr = dfa.Minimize();
					if (dfa.GetFinalState().Size() == 0)
					{
						system("pause");
					}
					dfamintimetime += ctime.Reset();//for test
					if (0 != nr || dfa.Size() > SC_MAXDFASIZE)
					{
						ruleResult.m_nResult = COMPILEDRULE::RES_EXCEEDLIMIT;
						dfa.Clear();
					}
				}
			}
			if (dfa.Size() == 0)
			{
				ruleResult.m_dfaIds.Clear();
				result.GetDfaTable().Resize(nDfaTblSize);
				result.GetRegexTbl().Resize(nRegexTblSize);
				return;
			}
			ruleResult.m_dfaIds.PushBack(nDfaId);
			result.GetRegexTbl()[nChainId] = regrule[i];
		}

		if (!bHasSigs)
		{
			ruleResult.m_nResult = COMPILEDRULE::RES_HASNOSIG;
			ruleResult.m_dfaIds.Clear();
			result.GetDfaTable().Resize(nDfaTblSize);
			result.GetRegexTbl().Resize(nRegexTblSize);
			return;
		}

		if (ruleResult.m_nResult != COMPILEDRULE::RES_ERROR)
		{
			AssignSig(result, nRegexTblSize, nRegexTblSize + nIncrement);
		}
	}
}

/*
* process one rule
*/
void CompileRule(LPCSTR rule, RECIEVER recv, LPVOID lpUser)
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
**	NAME
**	 Rule2PcreList::
*/
/**
**	This function converts a CSnortRule to a CRegRule and extract signatures from content option
**
**	According to the constraints of rule options, we split a snort rule into some option chains.
**	For every option chain, the datapacket matchs from the first byte.
**	Then we transfrom every option into pcre.
**
**	@param rule		 a CSnortRule object which contains the original information
**					of a snort rule. 
**	@param regrule	 the transformed CRegRule object which makes up of a number of pcre lists
**						and the signatures in every pcre list.
**
**	@return integer
**
**	@retval  0 function successful
**	@retval <>0 fatal error
*/

ulong Rule2PcreList(const CSnortRule &rule, CRegRule &regrule)
{
	regrule.Reserve(nfaTreeReserve);
	ulong regChain_size = 0;
	regrule.Resize(++regChain_size);
	int cFlag = 0;

	for(ulong i = 0; i < rule.Size(); ++i)
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
			CDllString conPcreStr;

			//transfrom content to pcre
			cFlag = content2Pcre(pContent, conPcreStr);
			if(cFlag != 0)
			{
				return cFlag;
			}
			if(pContent->vecconts.size() >= 4)
			{
				//extract signatures
				BYTEARY contentTmp;
				contentTmp.reserve(pContent->vecconts.size());
				for(BYTEARY_ITER itTmp = pContent->vecconts.begin();
					itTmp != pContent->vecconts.end(); ++itTmp)
				{
					byte c = *itTmp;
					if (c >= 'A' && c <= 'Z')
					{
						contentTmp.push_back(c - 'A' + 'a');
					}
					else
					{
						contentTmp.push_back(c);
					}
				}

				for(BYTEARY_ITER sigIt = contentTmp.begin();
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
			CDllString strPattern(tmpStr.c_str());
			regrule.Back().PushBack(strPattern);
		}
	}

	//regrule.Reserve(++regChain_size);
	for(ulong i = 0; i < regrule.Size(); ++i)
	{
		if(regrule[i].GetSigs().Size() > 1)
		{
			regrule[i].GetSigs().Unique();
		}
	}
	return 0;
}