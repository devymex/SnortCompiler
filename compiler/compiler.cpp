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

/* complie one rule

Arguments:
  rule		the snort rule
  lpVoid		the compile result

Returns:		nothing

*/
void __stdcall CompileResult(const PARSERESULT &parseRes, void *lpVoid)
{
	CCompileResults &result = *(CCompileResults*)lpVoid;
	ulong nFlag = parseRes.ulFlags;
	ulong nNewSize = result.GetSidDfaIds().Size() + 1;
	result.GetSidDfaIds().Resize(nNewSize);
	COMPILEDRULE &ruleResult = result.GetSidDfaIds().Back();
	ruleResult.m_nSid = parseRes.ulSid;
	if (parseRes.ulRet == SC_ERROR)
	{
		ruleResult.m_nResult = COMPILEDRULE::RES_ERROR;
	}
	else if (parseRes.regRule.Size() == 0)
	{
		ruleResult.m_nResult = COMPILEDRULE::RES_EMPTY;
	}
	else if (nFlag & CSnortRule::RULE_HASNOT)
	{
		ruleResult.m_nResult = COMPILEDRULE::RES_HASNOT;
	}
	else if (nFlag & CSnortRule::RULE_HASBYTE)
	{
		ruleResult.m_nResult = COMPILEDRULE::RES_HASBYTE;
	}
	else
	{
		Rule2Dfas(parseRes.regRule, result, ruleResult);
	}
}

/*
* read rules from a file
* then process the rules to CSnortRule
* callback function RECIEVER to handle CSnortRule
*/
COMPILERHDR ulong ParseRuleFile(const char *pFileName, RECIEVER recv, void *lpUser)
{
	typedef std::vector<std::string>	STRINGVEC;
	typedef STRINGVEC::iterator			STRINGVEC_ITER;
	if(recv == NULL)
	{
		return (ulong)-1;
	}
	STRINGVEC rules;
	if(0 == LoadFile(pFileName, rules))
	{
		if(!rules.empty())
		{
			for(STRINGVEC_ITER i = rules.begin(); i != rules.end(); ++i)
			{
				std::cout << i - rules.begin() + 1 << std::endl;
				//std::cout << ": " << g_dTimer << std::endl;
				i->erase(i->begin(), find(i->begin(), i->end(), '(') + 1);
				i->erase(find(i->rbegin(), i->rend(), ')').base() - 1, i->end());

				CSnortRule snortRule;
				if (0 == ProcessOption(*i, snortRule))
				{
					PARSERESULT pr;
					pr.ulRet = Rule2PcreList(snortRule, pr.regRule);
					pr.ulFlags = snortRule.GetFlag();
					pr.ulSid = snortRule.GetSid();
					recv(pr, lpUser);
				}
			}
		}
	}
	return 0;
}

