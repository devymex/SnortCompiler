#include "stdafx.h"
#include "../compilernew/compilernew.h"

struct LISTTRAIT
{
	enum {TRAIT_LEN = 32};
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
	
	// Compute the LCS of every pair of correlative lists;
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
					pListMat[x * nListMatDem + y] = pListMat[y * nListMatDem + x] = LongestCommonString(listSet[x].strList, listSet[y].strList);
					nNonzeros += 2;
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

	//Analyze the data of the similar matrix
	std::cout << "Analyze the data of the similar matrix..." << std::endl;
	std::cout << "The number of the list: " << nListMatDem << std::endl;
	std::cout << "The element of the matrix: " << nListMatSize << std::endl;
	std::cout << "The matrix sparesity: " << nNonzeros / double(nListMatSize) << std::endl;
	std::vector<size_t> vecRowNonZeros;
	size_t nAllZeros = 0;
	std::vector<std::string> vecNotSimilar;
	std::vector<size_t> vecdfaSize;
	std::vector<std::string> vecVerySimiliar;
	for (size_t i = 0; i < nListMatDem; ++i)
	{
		size_t nRowNonZeros = 0;
		for (size_t j = 0; j < nListMatDem; ++j)
		{
			if (pListMat[i * nListMatDem + j] != 0)
			{
				++nRowNonZeros;
			}
		}
		if (nRowNonZeros == 0)
		{
			vecNotSimilar.push_back(listSet[i].strList);
			vecdfaSize.push_back(res.GetDfaTable()[i].Size());
			++nAllZeros;
		}
		else
		{
			vecVerySimiliar.push_back(listSet[i].strList);
		}
		vecRowNonZeros.push_back(nRowNonZeros);
	}
	//std::ofstream fout("..\\..\\output\\NotSimilar.txt");
	//for (size_t i = 0; i < nAllZeros; ++i)
	//{
	//	fout << vecNotSimilar[i] << "\t" << vecdfaSize[i] << std::endl;
	//}
	//fout.close();
	//fout.clear();
	//std::ofstream ff("..\\..\\output\\VerySimilar.txt");
	//for (std::vector<std::string>::iterator i = vecVerySimiliar.begin(); i != vecVerySimiliar.end(); ++i)
	//{
	//	ff << *i << std::endl;
	//}
	//ff.close();
	//ff.clear();
	std::cout << "The max of number of element who is nonzero in row: " << *std::max_element(vecRowNonZeros.begin(), vecRowNonZeros.end()) << std::endl;
	std::cout << "the number of row whose element is all zero: " << nAllZeros << std::endl;
	std::cout << "the average of number of element who is nonzero in each row: " << std::accumulate(vecRowNonZeros.begin(), vecRowNonZeros.end(), 0) / double(nListMatDem) << std::endl;
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//Start grouping...
	std::vector<std::vector<size_t>> vecGroups;
	std::cout << "Starting grouping..." << std::endl;
	for (size_t i = 0; i < nListMatDem; ++i)
	{
		vecGroups.push_back(std::vector<size_t>());
		std::vector<size_t> &vecGroup = vecGroups.back();
		vecGroup.push_back(i);
		for (size_t j = 0; j < nListMatDem; ++j)
		{
			if (pListMat[i * nListMatDem + j] != 0)
			{
				vecGroup.push_back(j);
			}
		}
		if (vecGroup.size() == 1)
		{
			vecGroups.pop_back();
		}
		if (vecGroup.size() > 1)
		{
			for (size_t j = 1; j < vecGroup.size(); ++j)
			{
				for (size_t k = 0; k < nListMatDem; ++k)
				{
					pListMat[vecGroup[j] * nListMatDem + k] = 0;
					pListMat[k * nListMatDem + vecGroup[j]] = 0;
				}
			}
		}
	}
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	//output grouping result...
	std::cout << "output grouping result..." << std::endl;
	std::ofstream fout("..\\..\\output\\GroupRes.txt");
	for (std::vector<std::vector<size_t>>::iterator i = vecGroups.begin(); i != vecGroups.end(); ++i)
	{
		for (std::vector<size_t>::iterator j = i->begin(); j != i->end(); ++j)
		{
			fout << listSet[*j].strList << std::endl;
		}
		fout << std::endl << std::endl;
	}
	fout.close();
	fout.clear();
	std::cout << "Completed in " << t1.Reset() << " Sec." << std::endl << std::endl;

	std::cout << "Total time: " << tAll.Reset() << " Sec." << std::endl;

	system("pause");
	return 0;
}
