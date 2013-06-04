#include "stdafx.h"
#include "PMatchPkt.h"

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

PMATCHPKT void MchCompile(LPCTSTR filename, LPVOID lpVoid)
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

//调用pcre库进行规则匹配
bool TradithinalMatch(std::vector<u_char> &dataSrc, CRegRule &regRule)
{
	for(size_t i = 0; i < regRule.Size(); ++i)
	{
		//从数据包头开始匹配
		std::string pData(dataSrc.begin(), dataSrc.end());
		size_t dataSize = dataSrc.size();
		for(size_t j = 0; j < regRule[i].Size(); ++j)
		{
			//对规则选项进行匹配
			int Pos = -1;
			bool flag = match(pData.c_str(), dataSize, regRule[i][j].GetString(), Pos);
			if(!flag)
			{
				return false;
			}
			else if(flag && Pos < dataSize)
			{
				pData = pData.substr(Pos);
				dataSize -= Pos;
			}
		}
	}
	return true;
}