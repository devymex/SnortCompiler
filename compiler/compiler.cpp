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

int count = 0;

/*
* read rules from a file
* then process the rules to CSnortRule
* callback function RECIEVER to handle CSnortRule
*/
COMPILERHDR void ParseRuleFile(const char *pFileName, RECIEVER recv, void *lpUser)
{
	typedef std::vector<std::string>	STRINGVEC;
	typedef STRINGVEC::iterator			STRINGVEC_ITER;
	if(recv == NULL)
	{
		throw 0;
	}
	STRINGVEC rules;
	if(0 == LoadFile(pFileName, rules))
	{
		if(!rules.empty())
		{
			for(STRINGVEC_ITER i = rules.begin(); i != rules.end(); ++i)
			{
				g_log << "Compiling: " << i - rules.begin() + 1 << g_log.nl;
				i->erase(i->begin(), find(i->begin(), i->end(), '(') + 1);
				i->erase(find(i->rbegin(), i->rend(), ')').base() - 1, i->end());

				CSnortRule snortRule;
				try
				{
					ParseOptions(*i, snortRule);
				}
				catch (CTrace &e)
				{
					g_log << "ParseOptions error: " << e.What() << g_log.nl;
					throw;
				}
				PARSERESULT pr;
				pr.ulSid = snortRule.GetSid();
				pr.ulFlag = PARSEFLAG::PARSE_DEFAULT;
				try
				{
					Rule2RegRule(snortRule, pr.regRule);
				}
				catch (CTrace &e)
				{
					pr.ulFlag |= PARSEFLAG::PARSE_ERROR;
					g_log << "Rule2RegRule error: " << e.What() << g_log.nl;
				}
				if (snortRule.GetFlags() & CSnortRule::HASBYTE)
				{
					pr.ulFlag |= PARSEFLAG::PARSE_HASBYTE;
				}
				if (snortRule.GetFlags() & CSnortRule::HASNOT)
				{
					pr.ulFlag |= PARSEFLAG::PARSE_HASNOT;
				}
				if (snortRule.Size() == 0)
				{
					pr.ulFlag |= PARSEFLAG::PARSE_EMPTY;
				}
				recv(pr, lpUser);
			}
		}
	}
}

COMPILERHDR void CompileRuleFile(const char *pFileName, CCompileResults &compRes)
{
	ParseRuleFile(pFileName, CompileCallback, &compRes);
	std::cout << count << std::endl;
}