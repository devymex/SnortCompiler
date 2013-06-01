#include "stdafx.h"
#include "PMatchPkt.h"

void CALLBACK MyProcess(const CSnortRule &rule, LPVOID lpVoid)
{
	REGRULESMAP &rulesmap = *(REGRULESMAP*)lpVoid;
	rulesmap.result.resize(rulesmap.result.size() + 1);
	rulesmap.result.back().m_nSid = rule.GetSid();

	Rule2PcreList(rule, rulesmap.result.back().regrule);
}

PMATCHPKT void MchCompile(LPCTSTR filename, LPVOID lpVoid)
{
	//std::vector<SIGNATURE> sigvec;
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
						rulesmap.sigmap[sig] = pos;
						//sigvec.push_back(sig);
						flag |= 1;
						goto END;
					}
				}
			}
		}
END:	if(((flag & 1) == 0) && ((flag & 1 << 1) != 0))
		{
			rulesmap.sigmap[tempsig] = pos;
			//sigvec.push_back(tempsig);
		}
	}
	//std::sort(sigvec.begin(), sigvec.end());
	//sigvec.erase(std::unique(sigvec.begin(),sigvec.end()), sigvec.end());
	std::cout << std::endl;
}
