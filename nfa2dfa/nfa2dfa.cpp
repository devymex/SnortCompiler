#include "stdafx.h"
#include "CreDfa.h"
#include "nfa2dfa.h"

CREDFA void NfaToDfa(CNfa &oneNfaTab, CDfa &dfaTab)
{
	std::vector<std::vector<size_t>> charGroups;
	AvaiEdges(oneNfaTab, charGroups);

	//printNfa(oneNfaTab);
	//std::vector<size_t> hashTable[HASHMODULO];
	typedef std::unordered_map<std::vector<size_t>, size_t, STATESET_HASH> STATESETHASH;

	STATESETHASH ssh;
	ssh.rehash(STATESET_HASH::MAX_SIZE);

	//std::stack<std::vector<size_t>> nfaStasStack;
	std::vector<std::vector<size_t>> nfaSta;
	nfaSta.reserve(SC_STATELIMIT + 1);
	size_t nNfaCursize = nfaSta.size();
	nfaSta.resize(nNfaCursize + 1);
	std::vector<size_t> &startEVec = nfaSta.back();
	std::vector<size_t> startVec;

	char finFlag = 0;
	startVec.push_back(0);
	EClosure(oneNfaTab, startVec, startEVec, finFlag);


	//nfaStasStack.push(startEVec);
	//ssh.insert(std::make_pair(startEVec, ssh.size()));
	ssh[startEVec] = ssh.size();

	size_t nCursize = dfaTab.Size();
	dfaTab.Resize(nCursize + 1);

	dfaTab.Back().SetFlag(dfaTab.Back().GetFlag() | dfaTab.Back().START);

	if(finFlag == 1)
	{
		dfaTab.Back().SetFlag(dfaTab.Back().GetFlag() | dfaTab.Back().TERMINAL);
		finFlag = 0;
	}
	//std::vector<size_t> curNfaVec;

	//while(nfaStasStack.size() > 0)
	while (nfaSta.size() > 0)
	{
		int curStaNum;
		//curNfaVec = nfaStasStack.top();
		//nfaStasStack.pop();
		std::vector<size_t> &curNfaVec = nfaSta.back();

		for(std::vector<std::vector<size_t>>::iterator group = charGroups.begin();
			group != charGroups.end(); ++group)
		{
			if( dfaTab.Size() > SC_STATELIMIT)
			{
				return;
			}
			size_t nCurChar = group->front();

			STATESETHASH::iterator ir = ssh.find(curNfaVec);
			if (ir == ssh.end())
			{
				std::cout << "Fatal Error!" << std::endl;
				break;
			}
			curStaNum = ir->second;

			//std::vector<size_t> nextNfaVec;
			nNfaCursize = nfaSta.size();
			nfaSta.resize(nNfaCursize + 1);
			std::vector<size_t> &nextNfaVec = nfaSta.back();
			NextNfaSet(oneNfaTab, curNfaVec, nCurChar, nextNfaVec, finFlag);

			if(!nextNfaVec.empty())
			{
				if(ssh.count(nextNfaVec) == 0)
				{
					size_t nextSta = ssh.size();
					ssh[nextNfaVec] = nextSta;

					size_t nCursize = dfaTab.Size();
					dfaTab.Resize(nCursize + 1);
					for(std::vector<size_t>::iterator iter = group->begin(); iter != group->end(); ++iter )
					{
						dfaTab[curStaNum][*iter] = nextSta;
					}
					if(finFlag == 1)
					{
						dfaTab.Back().SetFlag(dfaTab.Back().GetFlag() | dfaTab.Back().TERMINAL);
						finFlag = 0;
					}
					//nfaStasStack.push(nextNfaVec);
				}
				else
				{
					for(std::vector<size_t>::iterator iter = group->begin(); iter != group->end(); ++iter )
					{
						dfaTab[curStaNum][*iter] = ssh[nextNfaVec];
					}
					nfaSta.pop_back();
				}
			}
			else
			{
				nfaSta.pop_back();
			}
		}
		nfaSta.pop_back();
	}
}

