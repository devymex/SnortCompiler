#include "stdafx.h"

#include <hwprj\dfacompress.h>

#define STATEMAX 500

DFACOMPRESS void DfaCompress(CDfa &olddfa)
{
	std::vector<CUnsignedArray> clusterVec;//��ʾ�أ�ÿ��Ԫ��ÿ�����е�״̬���
	STATEID stateCluster[STATEMAX];//���ÿ��״̬�����ĸ���,�������500��״̬
	std::fill(stateCluster, stateCluster + STATEMAX, STATEMAX);

	GetDfaCluster(olddfa, clusterVec, stateCluster);
}

//���dfa�Ĵ���Ϣ
void GetDfaCluster(CDfa &olddfa, std::vector<CUnsignedArray> &clusterVec, STATEID *stateCluster)
{
}
