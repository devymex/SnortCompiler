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
	//std::vector<SIGNATURE> sigs;
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

//struct MYRESULT
//{
//	TRAITSMAP myMap;
//	std::vector<CSnortRule> mySet;
//};
//
//size_t LongestCommonString(const std::string &str1, const std::string &str2)
//{
//	size_t nRow = str1.size(), nCol = str2.size();
//	size_t nMatSize = (nRow + 1) * (nCol + 1);
//	static size_t pMatrix[1000000];
//	ZeroMemory(pMatrix, nMatSize * sizeof(size_t));
//	size_t iMax = 0;
//	for (size_t i = 0; i < nRow; ++i)
//	{
//		for (size_t j = 0; j < nCol; ++j)
//		{
//			size_t nSrc = i * nCol + j;
//			size_t nDest = nSrc + nCol + 1;
//			pMatrix[nDest] = pMatrix[nSrc] + (str1[i] == str2[j]);
//			if (pMatrix[nDest] > pMatrix[iMax])
//			{
//				iMax = nDest;
//			}
//		}
//	}
//	return pMatrix[iMax];
//}
//
//template<typename _RECVFUNC>
//size_t ExtractTraits(const RULEPATTERN &pat, _RECVFUNC recv)
//{
//	size_t nTraits = 0;
//	size_t nLen = pat.strPat.length();
//	if (nLen > RULETRAITS::TRAITS_LEN)
//	{
//		RULETRAITS rt;
//		for (size_t i = RULETRAITS::TRAITS_LEN; i < nLen; ++i, ++nTraits)
//		{
//			CopyMemory(rt.szTraits, &pat.strPat[i - RULETRAITS::TRAITS_LEN], RULETRAITS::TRAITS_LEN);
//			recv(rt);
//		}
//	}
//	return nTraits;
//}
//
//struct TRAITSRECEIVER
//{
//	std::vector<RULETRAITS> &m_Traits;
//	TRAITSRECEIVER(std::vector<RULETRAITS> &traits) : m_Traits(traits){}
//	void operator()(const RULETRAITS &rt)
//	{
//		m_Traits.push_back(rt);
//	}
//};
//
//typedef std::vector<size_t> VECSIZET;
//size_t RuleSimilarity(size_t *pPatMat, size_t nPatDem, const VECSIZET &rule1, const VECSIZET &rule2)
//{
//	const VECSIZET *pPatSet1 = &rule1;
//	const VECSIZET *pPatSet2 = &rule2;
//	if (pPatSet1->size() > pPatSet2->size())
//	{
//		std::swap(pPatSet1, pPatSet2);
//	}
//
//	// make size of pPatSet1 less than pPatSet2;
//
//	size_t nDpMat[100000], nPatNum1 = pPatSet1->size(), nPatNum2 = pPatSet2->size();
//	size_t nMax = 0;
//	for (size_t i = 0; i < nPatNum1; ++i)
//	{
//		for (size_t j = 0; j < nPatNum2; ++j)
//		{
//			size_t nCurPos = i * nPatNum2 + j;
//			nDpMat[nCurPos] = pPatMat[(*pPatSet1)[i] * nPatDem + (*pPatSet2)[j]];
//			if (i > 0)
//			{
//				size_t nMax = 0;
//				for (size_t k = 0; k < nPatNum2; ++k)
//				{
//					if (k != j && nDpMat[nCurPos - nPatNum2] > nMax)
//					{
//						nMax = nDpMat[nCurPos - nPatNum2];
//					}
//				}
//				nDpMat[nCurPos] += nMax;
//			}
//			if (nDpMat[nCurPos] > nMax)
//			{
//				nMax = nDpMat[nCurPos];
//			}
//		}
//	}
//	return nMax;
//}
//
//struct SPARSEELEM
//{
//	size_t row;
//	size_t col;
//	size_t val;
//};

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
		//for (size_t j = 0; j < res.GetRegexTbl())
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
				//if (std::find_first_of)
				pListMat[y * nListMatDem + x] = LongestCommonString(listSet[x].strList, listSet[y].strList);
				++nNonzeros;
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

	////Building a set for rule number to SID
	//std::cout << "Building a set of rule SID...";
	//std::vector<size_t> ruleSids;
	//for (std::vector<RULEPATTERN>::iterator i = patSet.begin(); i != patSet.end(); ++i)
	//{
	//	ruleSids.push_back(i->nSid);
	//}
	//std::sort(ruleSids.begin(), ruleSids.end());
	//ruleSids.erase(std::unique(ruleSids.begin(), ruleSids.end()), ruleSids.end());
	//std::cout << "Completed in " << t1.Reset() << " Sec. SID members: " << nNonzeros << std::endl << std::endl;

	//// Building a map for SID to rule number;
	//std::map<size_t, size_t> ruleTbl;
	//std::cout << "Building a map for SID to rule number...";
	//for (std::vector<size_t>::iterator i = ruleSids.begin(); i != ruleSids.end(); ++i)
	//{
	//	ruleTbl[*i] = i - ruleSids.begin();
	//}
	//std::cout << "Completed in " << t1.Reset() << " Sec. Map size: " << ruleTbl.size() << std::endl << std::endl;

	//// Alloc memory for similar matrix of patterns
	//std::cout << "Allocating a simillar matrix of patterns (SMR)..." << std::endl;
	//size_t nRuleMatDem = ruleSids.size();
	//size_t nRuleMatSize = nRuleMatDem * nRuleMatDem;
	//size_t *pRuleMat = new size_t[nRuleMatSize];
	//ZeroMemory(pRuleMat, nRuleMatSize * sizeof(size_t));
	//std::cout << "Completed in " << t1.Reset() << " Sec. SMR size (bytes): " << nRuleMatSize * sizeof(size_t) << std::endl << std::endl;

	//std::cout << "Computer similarity of every pair of correlative rules..." << std::endl;
	//nNonzeros = 0;
	//for (size_t iPat1 = 0; iPat1 < nPatMatDem; ++iPat1)
	//{
	//	for (size_t iPat2 = iPat1 + 1; iPat2 < nPatMatDem; ++iPat2)
	//	{
	//		if (pPatMat[iPat1 * nPatMatDem + iPat2] != 0)
	//		{

	//			size_t nSid1 = patSet[iPat1].nSid, nSid2 = patSet[iPat2].nSid;
	//			size_t nRule1 = ruleTbl[nSid1], nRule2 = ruleTbl[nSid2];

	//			if (pRuleMat[nRule1 * nRuleMatDem + nRule2] == 0)
	//			{
	//				VECSIZET &patSet1 = patMap[nSid1];
	//				VECSIZET &patSet2 = patMap[nSid2];

	//				size_t nRuleSim = RuleSimilarity(pPatMat, nPatMatDem, patSet1, patSet2);
	//				if (nRuleSim > 0)
	//				{
	//					pRuleMat[nRule1 * nRuleMatDem + nRule2] = nRuleSim;
	//					pRuleMat[nRule1 * nRuleMatDem + nRule2] = nRuleSim;
	//					nNonzeros += 2;
	//				}
	//			}
	//		}
	//	}
	//	if (iPat1 % 1000 == 0 && iPat1 != 0)
	//	{
	//		std::cout << iPat1 << std::endl;
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
