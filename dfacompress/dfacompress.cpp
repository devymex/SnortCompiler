#include "stdafx.h"

#include <hwprj\dfacompress.h>

#define STATEMAX 500

DFACOMPRESS void DfaCompress(CDfa &olddfa)
{
	std::vector<CUnsignedArray> clusterVec;//表示簇，每个元素每个簇中的状态编号
	STATEID stateCluster[STATEMAX];//标记每个状态属于哪个簇,假设最多500个状态
	std::fill(stateCluster, stateCluster + STATEMAX, STATEMAX);

	GetDfaCluster(olddfa, clusterVec, stateCluster);
}

//获得dfa的簇信息
void GetDfaCluster(CDfa &olddfa, std::vector<CUnsignedArray> &clusterVec, STATEID *stateCluster)
{
}
