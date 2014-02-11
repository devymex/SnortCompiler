
#include <hwprj\MultiLevelHash.h>

// ºÏ—ÈhashDeepth
#include <iostream>

CMultiLevelHash::CMultiLevelHash(){}


CMultiLevelHash::~CMultiLevelHash(){}


void CMultiLevelHash::Init(const CGroupRes &groupRes)
{
	hash.nBucketCnt = groupRes.GetBucketCnt();
	dfaSize = groupRes.GetGroups().Size();
	mLevelHash.resize(hashDeepth);

	// ºÏ—ÈhashDeepth
	std::cout << hashDeepth << std::endl;

	minSig.Resize(dfaSize);

	for (int i = 0; i < hashDeepth; ++i)
	{
		mLevelHash[i].resize(hash.nBucketCnt);
	}

	for (int i = 0; i < dfaSize; ++i)
	{

		int nMinSigCnt = std::min(groupRes.GetGroups()[i].ComSigs.Size(), hashDeepth);
		for (int j = 0; j < nMinSigCnt; ++j)
		{
			
			SIGDFAID sigId;
			sigId.sig = groupRes.GetGroups()[i].ComSigs[j];
			sigId.dfaId = groupRes.GetGroups()[i].mergeDfaId;

			ulong hashValue = hash(sigId.sig);
			mLevelHash[j][hashValue].push_back(sigId);
		}
		minSig[i] = nMinSigCnt;
	}
}


void CMultiLevelHash::MatchOneSig(const ulong &sig, CUnsignedArray &matchResultArr)
{
	ulong hashValue = hash(sig);
	for (int i = 0; i < hashDeepth; ++i)
	{
		ulong sigDfaSz = mLevelHash[i][hashValue].size();
		for (int j = 0; j < sigDfaSz; ++j)
		{
			if (mLevelHash[i][hashValue][j].sig == sig)
			{
				++matchResultArr[mLevelHash[i][hashValue][j].dfaId];
				//std::cout << mLevelHash[i][hashValue][j].dfaId << std::endl;
			}
		}
	}
}


int CMultiLevelHash::GetMin(int i)
{
	return minSig[i];
}
