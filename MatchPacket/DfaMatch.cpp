#include "stdafx.h"
#include "MatchPkt.h"

void FindSig(size_t sig, std::map<size_t, std::vector<SIGSID>> &hashtable, std::vector<size_t> &matchSids)
{
	if (hashtable.count(sig))
	{
		std::vector<SIGSID> &onevec = hashtable[sig];
		if (!onevec.empty())
		{
			for(std::vector<SIGSID>::iterator iter = onevec.begin(); iter != onevec.end(); ++iter)
			{
				if (sig == iter->sig)
				{
					matchSids.resize(matchSids.size() + 1);
					matchSids.back() = iter->sig;
				}
			}
		}
	}
}

//���� 0 ��ʾû��ƥ���ϣ����� 1 ��ʾƥ����
size_t MatchOnedfa(std::vector<u_char> &onepkt, CDfaNew &dfa, std::vector<size_t> &matchedDids)
{
	std::unordered_map<size_t, std::vector<size_t>> dfaids;
	for (size_t i = 0; i < dfa.GetTermCnt(); ++i)
	{
		TERMSET &term = dfa.GetTerm(i);
		if (dfaids.count(term.dfaSta))
		{
			dfaids[term.dfaSta].push_back(term.dfaId);
		}
		else
		{
			dfaids[term.dfaSta].resize(1);
			dfaids[term.dfaSta].push_back(term.dfaId);
		}
	}

	STATEID curSta = dfa.GetStartId();
	for (std::vector<u_char>::iterator edgeiter = onepkt.begin(); edgeiter != onepkt.end(); ++edgeiter)
	{
		BYTE group = dfa.GetOneGroup(*edgeiter);

		if (0 == (dfa[curSta].GetFlag() & CDfaRow::TERMINAL))
		{
			if (dfa[curSta][group] != -1)
			{
				curSta = dfa[curSta][group];
			}
			else
			{
				return 0;
			}
		}
		else
		{

		}
	}
}

MATCHPKT void DfaMatchPkt(std::vector<std::vector<u_char>> &allPkt, std::map<size_t, std::vector<SIGSID>> &hashtable, std::vector<CDfaNew> &alldfas, std::vector<MATCHRESULT> &matchresult)
{
	for (std::vector<std::vector<u_char>>::iterator allPktIter = allPkt.begin(); allPktIter != allPkt.end(); ++allPktIter)
	{
		SIGNATURE onesig = -1;
		std::vector<size_t> matchDids;
		std::vector<size_t> matchedSids;

		for (std::vector<u_char>::iterator pktIter = allPktIter->begin(); pktIter + 3 != allPktIter->end(); ++pktIter)
		{
			onesig = *(SIGNATURE *)&(*pktIter);
			FindSig(HashFcn(onesig), hashtable, matchDids);
		}

		if (!matchDids.empty())
		{
			for(std::vector<size_t>::iterator idIter = matchDids.begin(); idIter != matchDids.end(); ++idIter)
			{
				MatchOnedfa(*allPktIter, alldfas[*idIter], matchedSids);
			}
		}

		if(!matchedSids.empty())
		{
			matchresult.resize(matchresult.size() + 1);
			matchresult.back().pktnum = allPktIter - allPkt.begin();
			matchresult.back().matchedSids = matchedSids;
		}
	}
}