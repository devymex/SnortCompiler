#include "stdafx.h"
#include "../compilernew/compilernew.h"

struct LISTTRAIT
{
	enum {TRAIT_LEN = 16};
	char szTrait[TRAIT_LEN];
};

bool operator == (const LISTTRAIT &r1, const LISTTRAIT &r2)
{
	__int64 *p1 = (__int64*)r1.szTrait;
	__int64 *p2 = (__int64*)r2.szTrait;
	for (size_t i = 0; i < LISTTRAIT::TRAIT_LEN / sizeof(__int64); ++i)
	{
		if (p1[i] != p2[i])
		{
			return false;
		}
	}
	return true;
}

struct LISTTRAITHASH
{
	enum {HASH_SIZE = 1000000};
	size_t operator()(const LISTTRAIT &rt)
	{
		const size_t *p = (size_t*)rt.szTrait;
		size_t n = 1;
		for (size_t i = 0; i < LISTTRAIT::TRAIT_LEN / sizeof(size_t); ++i)
		{
			n *= p[i];
		}

		return n % HASH_SIZE;
	}
};

typedef std::unordered_map<LISTTRAIT, std::vector<size_t>, LISTTRAITHASH> TRAITMAP;

struct RULELIST
{
	std::string strList;
	std::vector<SIGNATURE> sigs;
};

bool ExtractTraits(const RULELIST &rulelist, LISTTRAIT &trait)
{
	bool flag = false;
	size_t nLen = rulelist.strList.length();
	if (nLen > LISTTRAIT::TRAIT_LEN)
	{
		flag = true;
		CopyMemory(trait.szTrait, &rulelist.strList[0], LISTTRAIT::TRAIT_LEN);
	}
	return flag;
}

size_t LongestCommonString(const std::string &str1, const std::string &str2)
{
	size_t count = 0;
	size_t min = str1.length() < str2.length() ? str1.length() : str2.length();
	while (count < min && str1[count] == str2[count])
	{
		++count;
	}
	return count;
}

int main(void)
{
	//Load all rules lists from file
	CTimer t1, tAll;

	std::cout << "Loading results from file..." << std::endl;
	CResNew res;
	res.ReadFromFile(_T("..\\..\\output\\result.cdt"));
	std::cout << "Completed in " << t1.Reset() << " Sec. lists: " << res.GetRegexTbl().Size() << std::endl << std::endl;

	std::vector<RULELIST> listSet;
	listSet.reserve(10000);
	for (size_t i = 0; i < res.GetRegexTbl().Size(); ++i)
	{
		listSet.push_back(RULELIST());
		RULELIST &cr = listSet.back();
		for (size_t j = 0; j < res.GetRegexTbl()[i].Size(); ++j)
		{
			cr.strList += res.GetRegexTbl()[i][j].C_Str();
		}
		for (size_t j = 0; j < res.GetRegexTbl()[i].GetSigCnt(); ++j)
		{
			cr.sigs.push_back(res.GetRegexTbl()[i].GetSig(j));
		}
	}

	// Build list traits map
	std::cout << "Building a trait map for a trait to a vector of list index numbers..." << std::endl;
	TRAITMAP traitMap;
	traitMap.rehash(LISTTRAITHASH::HASH_SIZE);
	for (std::vector<RULELIST>::iterator i = listSet.begin(); i != listSet.end(); ++i)
	{
		LISTTRAIT trait;
		if (ExtractTraits(*i, trait))
		{
			TRAITMAP::iterator irt = traitMap.find(trait);
			if (irt == traitMap.end())
			{
				std::vector<size_t> newVec;
				newVec.push_back(i - listSet.begin());
				traitMap.insert(std::make_pair(trait, newVec));
			}
			else
			{
				irt->second.push_back(i - listSet.begin());
			}
		}
	}
	std::cout << "Completed in " << t1.Reset() << " Sec. Traits: " << traitMap.size() << std::endl << std::endl;

	// Alloc memory for list similar matrix
	std::cout << "Allocating a simillar matrix of lists (SML)." << std::endl;
	size_t nListMatDem = listSet.size();
	size_t nListMatSize = nListMatDem * nListMatDem;
	size_t *pListMat = new size_t[nListMatSize];
	ZeroMemory(pListMat, nListMatSize * sizeof(size_t));
	std::cout << "Completed in " << t1.Reset() << " Sec. SML size (bytes): " << nListMatSize * sizeof(size_t) << std::endl << std::endl;
	
	// Compute the LCS of every pair of correlative patterns;
	std::cout << "Computer similarity of every pair of correlative lists..." << std::endl;
	size_t nNonzeros = 0;
	for (std::vector<RULELIST>::iterator i = listSet.begin(); i != listSet.end(); ++i)
	{
		LISTTRAIT trait;
		if (!ExtractTraits(*i, trait))
		{
			continue;
		}

		// Compute LCS of every pair of correlative lists
		TRAITMAP::iterator irt = traitMap.find(trait);
		for (std::vector<size_t>::iterator j = irt->second.begin(); j != irt->second.end(); ++j)
		{
			size_t y = i - listSet.begin(), x = *j;
			if (y < x)
			{
				if (std::find_first_of(i->sigs.begin(), i->sigs.end(), listSet[*j].sigs.begin(), listSet[*j].sigs.end()) != i->sigs.end())
				{
					pListMat[y * nListMatDem + x] = LongestCommonString(listSet[x].strList, listSet[y].strList);
					++nNonzeros;
				}
			}
		}
		size_t iCur = i - listSet.begin();
		if (iCur % 1000 == 0 && iCur != 0)
		{
			std::cout << iCur << std::endl;
		}
	}
	std::cout << "Completed in " << t1.Reset() << " Sec. Non-zero members: " << nNonzeros << std::endl << std::endl;

	//const size_t nThreshould = 3;
	//nNonzeros = 0;
	//std::cout << "Threshould every elem of SMP to " << nThreshould << std::endl;
	//for (size_t i = 0; i < nPatMatDem; ++i)
	//{
	//	for (size_t j = i + 1; j < nPatMatDem; ++j)
	//	{
	//		if (pPatMat[i * nPatMatDem + j] > 0)
	//		{
	//			if (pPatMat[i * nPatMatDem + j] <= nThreshould)
	//			{
	//				pPatMat[i * nPatMatDem + j] = 0;
	//			}
	//			else
	//			{
	//				pPatMat[j * nPatMatDem + i] = pPatMat[i * nPatMatDem + j];
	//				nNonzeros += 2;
	//			}
	//		}
	//	}
	//}
	//std::cout << "Completed in " << t1.Reset() << " Sec. Non-zero members: " << nNonzeros << std::endl << std::endl;

	//std::ofstream ff("..\\out.tmp");
	//std::vector<SPARSEELEM> sparseMat;
	//sparseMat.reserve(nNonzeros / 2);
	//for (size_t iRule1 = 0; iRule1 < nRuleMatDem; ++iRule1)
	//{
	//	for (size_t iRule2 = iRule1 + 1; iRule2 < nRuleMatDem; ++iRule2)
	//	{
	//		SPARSEELEM se;
	//		se.val = pRuleMat[iRule1 * nRuleMatDem + iRule2];
	//		if (se.val > 0)
	//		{
	//			se.row = ruleSids[iRule1];
	//			se.col = ruleSids[iRule2];
	//			sparseMat.push_back(se);
	//			ff << se.row << " " << se.col << " " << se.val << std::endl;
	//		}
	//	}
	//}

	//for (std::vector<SPARSEELEM>::iterator i = sparseMat.begin(); i != sparseMat.end(); ++i)
	//{
	//	for (std::vector<SPARSEELEM>::iterator j = i + 1; i != sparseMat.end(); ++i)
	//	{

	//	}
	//}
	//std::cout << "Total time: " << tAll.Reset() << " Sec." << std::endl;


	system("pause");
	return 0;
}
