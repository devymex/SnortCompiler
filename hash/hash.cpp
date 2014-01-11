#include <iostream>

#include <hwprj\compres.h>
#include <hwprj\groupres.h>
#include <hwprj\buildhash.h>

typedef std::vector<int> MYDFAID;
typedef std::vector<MYDFAID> HASHTABLE;
typedef std::vector<HASHTABLE> ALLHASH;

typedef struct
{
	int dfaSize;
	int hashDeepth;

	ALLHASH allHash;
}	MYHASH;


void establishHashTable(MYHASH * myhash, CGroupRes * groupRes, const int deepth, MYDFAID *min)
{
	myhash->hashDeepth = deepth;
	myhash->dfaSize = groupRes->GetGroups().Size();
	myhash->allHash.resize(myhash->hashDeepth);

	
	for(int i = 0; i < myhash->hashDeepth; ++i)
	{
		myhash->allHash[i].resize(groupRes->GetBucketCnt());
	}

	for(int i = 0; i < myhash->dfaSize; ++i)
	{
		for(int j = 0; j < groupRes->GetGroups()[i].ComSigs.Size() && j < deepth; ++j)
		{
			myhash->allHash[j][hash(groupRes->GetGroups()[i].ComSigs[j])].push_back(groupRes->GetGroups()[i].mergeDfaId);
		}
		
		if(deepth <= groupRes->GetGroups()[i].ComSigs.Size())
		{
			min->push_back(deepth);
		}
		else
		{
			min->push_back(groupRes->GetGroups()[i].ComSigs.Size());
		}
	}

}
int main()
{
	CGroupRes groupRes;
	groupRes.ReadFromFile("FinalResult.cdt");
	hash.nBucketCnt = groupRes.GetBucketCnt();

	
	MYHASH myHash;
	myHash.dfaSize = groupRes.GetGroups().Size();
	myHash.hashDeepth = 3;
	MYDFAID min;
	establishHashTable(&myHash, &groupRes, 3, &min);

	return 0;

}