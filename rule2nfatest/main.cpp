#include "../rule2nfa/rule2nfa.h"
#include <iostream>
#include <fstream>
#include <string>
#include <tchar.h>

struct SNORTIDDFAIDS
{
	size_t m_nSid;
	std::vector<size_t> m_dfaIds;
};

struct RES
{
	std::vector<CDfa> m_dfaTbl;
	std::vector<SNORTIDDFAIDS> m_sidDfaIds;
	std::vector<size_t> m_errorIds;
	std::vector<size_t> m_exceedIds;
	std::vector<size_t> m_hasbyteIds;
	std::vector<size_t> m_hasnotIds;
	std::vector<size_t> m_emptyIds;
};


void printNfa(CNfa &nfa)
{
	for (size_t j = 0; j < nfa.Size(); ++j)
		{
			std::cout << j << ": ";
			for (size_t k = 0; k < CHARSETSIZE; ++k)
			{
				for (size_t l = 0; l < nfa[j][k].Size(); ++l)
				{
					std::cout << "(" << k << "," << nfa[j][k][l] << ")";
				}
			}
			std::cout << std::endl;
		}
}

void CALLBACK Process(const CSnortRule &rule, LPVOID lpVoid)
{
	RES &result = *(RES*)lpVoid;
	size_t nSid = rule.GetSid();
	size_t nFlag = rule.GetFlag();
	if (nFlag & CSnortRule::RULE_HASBYTE)
	{
		result.m_hasbyteIds.push_back(nSid);
	}
	else if (nFlag & CSnortRule::RULE_HASNOT)
	{
		result.m_hasnotIds.push_back(nSid);
	}
	else if (rule.Size() == 0)
	{
		result.m_emptyIds.push_back(nSid);
	}
	else
	{
		CNfaTree nfatree;
		size_t flag = InterpretRule(rule, nfatree);
		if (flag == SC_ERROR)
		{
			result.m_errorIds.push_back(nSid);
			return;
		}
		else if (flag == SC_EXCEED)
		{
			result.m_exceedIds.push_back(nSid);
			return;
		}
		else
		{
			SNORTIDDFAIDS sidDfaIds;
			sidDfaIds.m_nSid = nSid;
			const size_t nCursize = result.m_dfaTbl.size();
			const size_t nIncrement = nfatree.Size();
			result.m_dfaTbl.resize(nCursize + nIncrement);
			size_t nId;
			for (size_t i = 0; i < nIncrement; ++i)
			{
				CNfa nfa;
				SerializeNfa(nfatree[i], nfa);
				nId = nCursize + i;
				//NfaToDfa(nfa, dfa);
				sidDfaIds.m_dfaIds.push_back(nId);
			}
		}
	}
}


void main()
{
	RES result;
	CompileRuleSet(_T("..\\test.txt"), Process, &result);
	system("pause");

}