#include "stdafx.h"

#include <hwprj\dfacompress.h>

#define STATEMAX 500

DFACOMPRESS void DfaCompress(CDfa &olddfa)
{
	std::vector<CUnsignedArray> clusterVec;//表示簇，每个元素每个簇中的状态编号
	ulong stateCluster[STATEMAX];//标记每个状态属于哪个簇,假设最多500个状态
	std::fill(stateCluster, stateCluster + STATEMAX, STATEMAX);
	ulong codeMap[STATEMAX];
	std::fill(codeMap, codeMap + STATEMAX, STATEMAX);
	GetDfaCluster(olddfa, clusterVec, stateCluster, codeMap);
}

//获得dfa的簇信息
void GetDfaCluster(CDfa &olddfa, std::vector<CUnsignedArray> &clusterVec, ulong *stateCluster, ulong *codeMap)
{
	CUnsignedArray stateVec;
	ulong text = 0;
	ulong label[STATEMAX] = {0};
	std::stack<CUnsignedArray> stateStack;
	STATEID sstate = olddfa.GetStartState();
	label[sstate] =1;
	codeMap[sstate] = 0;
	CUnsignedArray cua;
	cua.PushBack(sstate);
	clusterVec.push_back(cua);
	stateStack.push(cua);
	cua.Clear();
	stateCluster[sstate] = 0;
	bool temp = false;
	while(!stateStack.empty())
	{
		stateVec=stateStack.top();   
		stateStack.pop();
		for(ulong k = 0; k < stateVec.Size(); k++)
		{
		
			for(ulong i = 0; i < olddfa[stateVec[k]].Size(); i++)
			{
				STATEID toState = olddfa[stateVec[k]][i];
				if(label[toState] != 1 && toState != (STATEID)-1)
				{
					cua.PushBack(toState);
					codeMap[toState] = ++text;
					label[toState] = 1;
					stateCluster[toState] = clusterVec.size();
					temp = true;
				}
				if(label[toState] != 1&&toState == (STATEID)-1)
				{
					label[256] = 1;
				}
			}
			if(temp)
			{
				stateStack.push(cua);
				clusterVec.push_back(cua);
				cua.Clear();
				temp = false;
			}
		}
	}
	if(label[256] == 1)
	{
		cua.PushBack((STATEID)256);
		clusterVec.push_back(cua);
		stateCluster[256] = clusterVec.size() - 1;
	}
}
