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
				std::cout << i - rules.begin() + 1 << std::endl;
				//std::cout << ": " << g_dTimer << std::endl;
				i->erase(i->begin(), find(i->begin(), i->end(), '(') + 1);
				i->erase(find(i->rbegin(), i->rend(), ')').base() - 1, i->end());

				CSnortRule snortRule;
				try
				{
					ProcessOption(*i, snortRule);
				}
				catch (CTrace &e)
				{
					std::cout << "ProcessOption error: " << e.What() << std::endl;
					continue;
				}
				PARSERESULT pr;
				pr.ulSid = snortRule.GetSid();
				pr.ulFlag = PARSEFLAG::PARSE_DEFAULT;
				try
				{
					Rule2RegRule(snortRule, pr.regRule);
					pr.ulFlag |= snortRule.GetFlag();
				}
				catch (CTrace &e)
				{
					std::cout << e.What() << std::endl;
					pr.ulFlag |= PARSEFLAG::PARSE_ERROR;
					std::cout << "Rule2RegRule error: " << e.What() << std::endl;
				}
				recv(pr, lpUser);
			}
		}
	}
}

COMPILERHDR void CompileRuleFile(const char *pFileName, CCompileResults &compRes)
{
	ParseRuleFile(pFileName, CompileCallback, &compRes);
}