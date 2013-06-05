#include "stdafx.h"
#include "MatchPkt.h"
void CALLBACK MyProcess(const CSnortRule &rule, LPVOID lpVoid)
{
	REGRULESMAP &rulesmap = *(REGRULESMAP*)lpVoid;
	size_t nFlag = rule.GetFlag();

	if (rule.Size() == 0)
	{
		return;
	}
	else if (nFlag & CSnortRule::RULE_HASNOT)
	{
		return;
	}
	else if (nFlag & CSnortRule::RULE_HASBYTE)
	{
		return;
	}

	else
	{
		rulesmap.result.resize(rulesmap.result.size() + 1);
		rulesmap.result.back().m_nSid = rule.GetSid();
		Rule2PcreList(rule, rulesmap.result.back().regrule);
	}
}

MATCHPKT void MchCompile(LPCTSTR filename, LPVOID lpVoid)
{
	//int sids[4653];
	//std::memset(sids, 0, sizeof(sids));
	std::vector<SIGNATURE> sigvec;
	CompileRuleSet(filename, MyProcess, lpVoid);
	REGRULESMAP &rulesmap = *(REGRULESMAP*)lpVoid;
	std::vector<REGRULES>::iterator mapbegin = rulesmap.result.begin();
	for (std::vector<REGRULES>::iterator iter = mapbegin; iter != rulesmap.result.end(); ++iter)
	{
		SIGNATURE tempsig = (SIGNATURE)-1;
		size_t flag = 0;
        size_t pos = iter - mapbegin;
		for (size_t chainsize = 0; chainsize < iter->regrule.Size(); ++chainsize)
		{
			SIGNATURE sig;
			size_t sigcnt = (iter->regrule)[chainsize].GetSigCnt();
			if (sigcnt > 0)
			{
				flag |= (1 << 1);
				size_t random = rand() % sigcnt;
				tempsig = (iter->regrule)[chainsize].GetSig(random);

				for (size_t cursig = 0; cursig < sigcnt; ++cursig)
				{
					sig = (iter->regrule)[chainsize].GetSig(cursig);

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

	//std::vector<size_t> sidVec;
	//for(int i = 0; i < 4653; ++i)
	//{
	//	if(0 == sids[i])
	//	{
	//		//sidVec.push_back(rulesmap.result[i].m_nSid);
	//		std::cout << rulesmap.result[i].m_nSid << std::endl;
	//	}
	//}
	size_t e0 = 0, e1 = 0, e2 = 0;
	SIGSMAP &temp = rulesmap.sigmap;
	for(SIGSMAP::iterator iter = temp.begin(); iter != temp.end(); ++ iter)
	{
		size_t size = iter->second.size();
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
