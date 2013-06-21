#include "stdafx.h"
#include "MatchPkt.h"

typedef std::vector<BYTEARY>			CHAINCOMPDATA;
typedef std::vector<CHAINCOMPDATA>		RULECOMPDATA;

void __stdcall MyProcess(const PARSERESULT &parseRes, void *lpParam)
{
	REGRULESMAP &rulesmap = *(REGRULESMAP*)lpParam;
	
	if(parseRes.ulFlag == PARSEFLAG::PARSE_DEFAULT)
	{
		RULECOMPDATA1 ruleCompData;
		rulesmap.result.resize(rulesmap.result.size() + 1);
		rulesmap.result.back().m_nSid = parseRes.ulSid;
		rulesmap.result.back().regrule = parseRes.regRule;
		ProcessRule(rulesmap.result.back().regrule, ruleCompData);
	}
}


MATCHPKT void MchCompile(const char* filename, LPVOID lpVoid)
{
	//int sids[4653];
	//std::memset(sids, 0, sizeof(sids));
	std::vector<SIGNATURE> sigvec;
	ParseRuleFile(filename, MyProcess, lpVoid);
	REGRULESMAP &rulesmap = *(REGRULESMAP*)lpVoid;
	std::vector<REGRULES>::iterator mapbegin = rulesmap.result.begin();
	for (std::vector<REGRULES>::iterator iter = mapbegin; iter != rulesmap.result.end(); ++iter)
	{
		SIGNATURE tempsig = (SIGNATURE)-1;
		ulong flag = 0;
        ulong pos = iter - mapbegin;
		for (ulong chainsize = 0; chainsize < iter->regrule.Size(); ++chainsize)
		{
			SIGNATURE sig;
			ulong sigcnt = (iter->regrule)[chainsize].GetSigs().Size();

			if (sigcnt > 0)
			{
				flag |= (1 << 1);
				ulong random = rand() % sigcnt;
				tempsig = (iter->regrule)[chainsize].GetSigs()[random];

				for (ulong cursig = 0; cursig < sigcnt; ++cursig)
				{
					sig = (iter->regrule)[chainsize].GetSigs()[cursig];
					char *csig = (char*)&sig;

					if (rulesmap.sigmap.count(sig) != 0)
					{
						continue;
					}
					else
					{
						SIGSMAP &temp = rulesmap.sigmap;
						temp[sig].resize(temp[sig].size() + 1);
						temp[sig].back() = pos;
						//sigvec.push_back(sig);
						//sids[pos] = 1;
						flag |= 1;
						goto END;
					}
				}
			}
		}
END:	if(((flag & 1) == 0) && ((flag & 1 << 1) != 0))
		{
			SIGSMAP &temp = rulesmap.sigmap;
			temp[tempsig].resize(temp[tempsig].size() + 1);
			temp[tempsig].back() = pos;
			//sigvec.push_back(tempsig);
			//sids[pos] = 1;

		}
	}
	//std::sort(sigvec.begin(), sigvec.end());
	//sigvec.erase(std::unique(sigvec.begin(),sigvec.end()), sigvec.end());

	//std::vector<ulong> sidVec;
	//for(int i = 0; i < 4653; ++i)
	//{
	//	if(0 == sids[i])
	//	{
	//		//sidVec.push_back(rulesmap.result[i].m_nSid);
	//		std::cout << rulesmap.result[i].m_nSid << std::endl;
	//	}
	//}
	ulong e0 = 0, e1 = 0, e2 = 0;
	SIGSMAP &temp = rulesmap.sigmap;
	for(SIGSMAP::iterator iter = temp.begin(); iter != temp.end(); ++ iter)
	{
		ulong size = iter->second.size();
		if (size == 0)
		{
			++e0;
		}
		else if(size == 1)
			++e1;
		else 
			++e2;
	}
	std::cout << std::endl;
}
