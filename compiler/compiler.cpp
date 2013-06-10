/**
**  @file        compilernew.cpp
**
**  @author      Lab 435, Xidian University
**
**  @brief       Support functions for compile rule to dfa
**
**  This implements compile rule to dfa algorithm
**
*/

#include "stdafx.h"
#include <hwprj\snortrule.h>
#include <hwprj\rule2nfa.h>

#include <hwprj\compiler.h>

/* assign all the signatures of each rule to all its option list

Arguments:
  result      the compile result
  BegIdx      the index of the first regex which belongs to this rule
  EndIdx      the next index of the last regex which belongs to this rule

Returns:      nothing

*/
void AssignSig(CCompileResults &result, ULONG BegIdx, ULONG EndIdx)
{
	//all the signatures of this rule
	std::vector<SIGNATURE> vecRuleSigs;
	for (ULONG i = BegIdx; i < EndIdx; ++i)
	{
		for (ULONG j = 0; j < result.GetRegexTbl()[i].GetSigs().Size(); ++j)
		{
			if (std::find(vecRuleSigs.begin(), vecRuleSigs.end(),
				result.GetRegexTbl()[i].GetSigs()[j]) == vecRuleSigs.end())
			{
				vecRuleSigs.push_back(result.GetRegexTbl()[i].GetSigs()[j]);
			}
		}
	}

	//assign all the signatures of each rule to all its option list
	for (ULONG i = BegIdx; i < EndIdx; ++i)
	{
		result.GetRegexTbl()[i].GetSigs().Clear();
		for (ULONG j = 0; j < vecRuleSigs.size(); ++j)
		{
			result.GetRegexTbl()[i].GetSigs().PushBack(vecRuleSigs[j]);
		}
	}
}

double rule2pcretime = 0.0;
double pcre2nfatime = 0.0;
double nfa2dfatime = 0.0;
double dfamintimetime = 0.0;

/* complie one rule to several dfas

Arguments:
  rule        the snort rule
  result      the compile result
  ruleResult  the relationship between sid and dfa ids

Returns:      nothing

*/

void Rule2Dfas(const CSnortRule &rule, CCompileResults &result, COMPILEDRULENEW &ruleResult)
{
	CRegRule regrule;
	CTimer ctime;//for test
	ctime.Reset();//for test

	ULONG flag = Rule2PcreList(rule, regrule);
	rule2pcretime += ctime.Reset();//for test

	if (flag == SC_ERROR)
	{
		ruleResult.m_nResult = COMPILEDRULENEW::RES_ERROR;
		return;
	}
	else
	{
		ruleResult.m_nResult = COMPILEDRULENEW::RES_SUCCESS;
		const ULONG nDfaTblSize = result.GetDfaTable().Size();
		const ULONG nIncrement = regrule.Size();
		result.GetDfaTable().Resize(nDfaTblSize + nIncrement);
		const ULONG nRegexTblSize = result.GetRegexTbl().Size();
		result.GetRegexTbl().Resize(nRegexTblSize + nIncrement);
		ULONG nDfaId;
		ULONG nChainId;
		bool bHasSigs = false;
		for (ULONG i = 0; i < nIncrement; ++i)
		{
			CNfa nfa;

			ctime.Reset();//for test
			ULONG nToNFAFlag = CRegChainToNFA(regrule[i], nfa);
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
				ruleResult.m_nResult = COMPILEDRULENEW::RES_ERROR;
				ruleResult.m_dfaIds.Clear();
				result.GetDfaTable().Resize(nDfaTblSize);
				result.GetRegexTbl().Resize(nRegexTblSize);
				return;
			}
			else if (nToNFAFlag == SC_EXCEED)
			{
				ruleResult.m_nResult = COMPILEDRULENEW::RES_EXCEED;
			}
			else
			{
				ctime.Reset();//for test
				dfa.SetId(nDfaId);
				ULONG nToDFAFlag = dfa.FromNFA(nfa);
				nfa2dfatime += ctime.Reset();//for test

				if (nToDFAFlag == -1)
				{
					ruleResult.m_nResult = COMPILEDRULENEW::RES_EXCEEDLIMIT;
					dfa.Clear();
				}
				else
				{
					ctime.Reset();//for test
					ULONG nr = dfa.Minimize();
					if (dfa.GetFinalState().Size() == 0)
					{
						system("pause");
					}
					dfamintimetime += ctime.Reset();//for test
					if (0 != nr || dfa.Size() > SC_MAXDFASIZE)
					{
						ruleResult.m_nResult = COMPILEDRULENEW::RES_EXCEEDLIMIT;
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
			ruleResult.m_nResult = COMPILEDRULENEW::RES_HASNOSIG;
			ruleResult.m_dfaIds.Clear();
			result.GetDfaTable().Resize(nDfaTblSize);
			result.GetRegexTbl().Resize(nRegexTblSize);
			return;
		}

		if (ruleResult.m_nResult != COMPILEDRULENEW::RES_ERROR)
		{
			AssignSig(result, nRegexTblSize, nRegexTblSize + nIncrement);
		}
	}
}

/* complie one rule

Arguments:
  rule        the snort rule
  lpVoid      the compile result

Returns:      nothing

*/

void CALLBACK Process(const CSnortRule &rule, LPVOID lpVoid)
{
	CCompileResults &result = *(CCompileResults*)lpVoid;
	ULONG nFlag = rule.GetFlag();
	ULONG nNewSize = result.GetSidDfaIds().Size() + 1;
	result.GetSidDfaIds().Resize(nNewSize);
	COMPILEDRULENEW &ruleResult = result.GetSidDfaIds().Back();
	ruleResult.m_nSid = rule.GetSid();
	if (rule.Size() == 0)
	{
		ruleResult.m_nResult = COMPILEDRULENEW::RES_EMPTY;
		return;
	}
	else if (nFlag & CSnortRule::RULE_HASNOT)
	{
		ruleResult.m_nResult = COMPILEDRULENEW::RES_HASNOT;
		return;
	}
	else if (nFlag & CSnortRule::RULE_HASBYTE)
	{
		ruleResult.m_nResult = COMPILEDRULENEW::RES_HASBYTE;
		return;
	}
	else
	{
		Rule2Dfas(rule, result, ruleResult);
	}
}

/* complie all rules

Arguments:
  filename    path of the file where all rules are saved in
  result      the compile result

Returns:      nothing

*/

COMPILERNEW void CompileRuleSet(LPCTSTR filename, CCompileResults &result)
{
	CompileFile(filename, Process, &result);

	std::cout << "rule2pcretime:" << rule2pcretime << std::endl;
	std::cout << "pcre2nfatime:" << pcre2nfatime << std::endl;
	std::cout << "nfa2dfatime:" << nfa2dfatime << std::endl;
	std::cout << "dfamintimetime:" << dfamintimetime << std::endl;
}