#include "stdafx.h"
#include "NCreDfa.h"
#include "dfanew.h"
#include "md5.h"

struct MD5VAL
{
	size_t val[4];
};

/* Following codes is a testing for unordered_map */
/* DONT REMOVE */
//std::vector<size_t> v1, v2, v3;
//v1.push_back(0);
//v2.push_back(1);
////v2.push_back(2);

//int n = ssh.size();
//ssh.insert(std::make_pair(v1, 20));
//n = ssh.size();
//ssh.insert(std::make_pair(v2, 30));
//n = ssh.size();

//STATESETHASH::iterator xx1 = ssh.find(v1);
//n = xx1->second;
//STATESETHASH::iterator xx2 = ssh.find(v2);
//n = xx2->second;

bool NColumnEqual(std::vector<CStateSet*> &c1, std::vector<CStateSet*>&c2)
{
	if (c1.size() != c2.size())
	{
		return false;
	}
	for (size_t i = 0; i < c1.size(); ++i)
	{
		if (c1[i]->Size() != c2[i]->Size())
		{
			return false;
		}
		if (!(*c1[i] == *c2[i]))
		{
			return false;
		}
	}
	return true;
}


struct NFACOL_HASH
{
	size_t operator()(const MD5VAL &md5val)
	{
		const size_t _FNV_offset_basis = 2166136261U;
		const size_t _FNV_prime = 16777619U;

		size_t _Val = _FNV_offset_basis;
		for (size_t _Next = 0; _Next < 4; ++_Next)
			{	
				_Val ^= (size_t)md5val.val[_Next];
				_Val *= _FNV_prime;
			}

		return (_Val);
	}
};

bool operator == (const MD5VAL &md5val1, const MD5VAL &md5val2)
{
	for(size_t i = 0; i < 4; ++i)
	{
		if(md5val1.val[i] != md5val2.val[i])
		{
			return false;
		}
	}
	return true;
}

//对每一列求md5值，然后将此值进行hash,正确，与用string速度一样
//void NAvaiEdges(CNfa &oneNfaTab, STATEID *group)
//{
//	typedef std::unordered_map<MD5VAL, STATEID, NFACOL_HASH> GROUPHASH;//使用md5值进行hash
//	GROUPHASH md5Hash;
//
//	//std::vector<MD5VAL> colMd5Vec(DFACOLSIZE);
//	std::vector<MD5VAL> colMd5Vec;
//	typedef std::vector<std::vector<size_t> > COLUMN;
//	COLUMN nfaCol(DFACOLSIZE);
//
//	//将列取出，使用-1间隔
//	for(size_t i = 0; i < oneNfaTab.Size(); ++i)
//	{
//		for(size_t c = 0; c < DFACOLSIZE; ++c)
//		{
//			CStateSet &elems = oneNfaTab[i][c];
//			if(elems.Size() != 0)
//			{
//				for(size_t j = 0; j < elems.Size(); ++j)
//				{
//					if(elems[j] > oneNfaTab.Size())
//					{
//						std::cout << "overflow" << std::endl;
//						return;
//					}
//					nfaCol[c].push_back(elems[j]);
//				}
//			}
//			nfaCol[c].push_back(-1);
//		}
//	}
//
//	//求每列的md5值
//	MD5 md5;
//	for(size_t c = 0; c < DFACOLSIZE; ++c)
//	{
//		MD5VAL md5val;
//		md5.reset();
//		md5.update((const void*)&nfaCol[c][0], nfaCol[c].size() * sizeof(size_t));
//		memcpy(md5val.val, md5.digest(), 16);
//		colMd5Vec.push_back(md5val);
//	}
//
//	STATEID curId = 0;
//	for(size_t c = 0; c < DFACOLSIZE; ++c)
//	{
//		GROUPHASH::iterator it = md5Hash.find(colMd5Vec[c]);
//		if(it == md5Hash.end())
//		{
//			md5Hash[colMd5Vec[c]] = curId;
//			group[c] = curId;
//			++curId;
//		}
//		else
//		{
//			group[c] = it->second;
//		}
//	}
//}

struct KEY
{
	size_t* pAddr;
	size_t nSize;
};

struct GROUPSET_HASH
{
	size_t operator()(const std::vector<KEY> &set)
	{
		const size_t _FNV_offset_basis = 2166136261U;
		const size_t _FNV_prime = 16777619U;

		size_t _Val = _FNV_offset_basis;
		for (size_t _Next = 0; _Next < set.size(); ++_Next)
			{	
				if(set[_Next].pAddr != NULL)
				{
					for(size_t j = 0; j < set[_Next].nSize; ++j)
					{
						size_t elemt = set[_Next].pAddr[j];
						_Val ^= elemt;
						_Val *= _FNV_prime;

					}
				}
				_Val ^= (size_t)-1;
				_Val *= _FNV_prime;
			}

		return (_Val);
	}
};

bool operator == (const std::vector<KEY> &set1, const std::vector<KEY> &set2)
{
	size_t nSize = set1.size();
	if(nSize != set2.size())
	{
		return false;
	}
	for(size_t i = 0; i < nSize; ++i)
	{
		if((set1[i].pAddr == NULL && set2[i].pAddr != NULL) || (set1[i].pAddr != NULL && set2[i].pAddr == NULL))
		{
			return false;
		}
		if(set1[i].pAddr != NULL && set2[i].pAddr != NULL)
		{
			size_t iSize1 = set1[i].nSize;
			size_t iSize2 = set2[i].nSize;
			if(iSize1 != iSize2)
			{
				return false;
			}
			if(memcmp(set1[i].pAddr, set2[i].pAddr, iSize1 * sizeof(size_t)) != 0)
			{
				return false;
			}
		}

	}
	return true;
}


//使用std::vector<size_t*>进行hash, 正确，快
void NAvaiEdges(CNfa &oneNfaTab, STATEID *group)
{
	typedef std::unordered_map<std::vector<KEY>, STATEID, GROUPSET_HASH> GROUPHASH;
	GROUPHASH ghash;
	STATEID curId = 0;

	typedef std::vector<std::vector<KEY> > COLUMN;
	COLUMN nfaCol(DFACOLSIZE);

	size_t nSize = oneNfaTab.Size();
	for (size_t i = 0; i < nSize; ++i)
	{
		oneNfaTab[i].SortAllDest();
	}

	for(size_t c = 0; c < DFACOLSIZE; ++c)
	{
		nfaCol[c].resize(oneNfaTab.Size());
		for(size_t i = 0; i < oneNfaTab.Size(); ++i)
		{
			nfaCol[c][i].pAddr = oneNfaTab[i].GetCol(c);
			nfaCol[c][i].nSize = oneNfaTab[i].DestCnt(c);
		}

		GROUPHASH::iterator colIt = ghash.find(nfaCol[c]);
		if(colIt == ghash.end())
		{
			ghash[nfaCol[c]] = curId;
			group[c] = curId;
			++curId;
		}
		else
		{
			group[c] = colIt->second;
		}
	}

}

//string 正确 快，先取出列
//void NAvaiEdges(CNfa &oneNfaTab, STATEID *group)
//{
//	typedef std::unordered_map<std::string, STATEID> GROUPHASH;//将每一列表示成string形式，每行之间用'u'间隔,元素与元素之间用','间隔
//	GROUPHASH ghash;
//	STATEID curId = 0;
//	std::vector<std::string> colVec(DFACOLSIZE);
//	for(size_t i = 0; i < oneNfaTab.Size(); ++i)
//	{
//		for(size_t c = 0; c < DFACOLSIZE; ++c)
//		{
//			CStateSet &elems = oneNfaTab[i][c];
//			if(elems.Size() != 0)
//			{
//				elems.Sort();
//				for(size_t j = 0; j < elems.Size(); ++j)
//				{
//					if(elems[j] > oneNfaTab.Size())
//					{
//						std::cout << "overflow" << std::endl;
//						return;
//					}
//					colVec[c].push_back(elems[j]);
//				}
//			}
//			colVec[c].push_back('u');
//		}
//	}
//	for(size_t c = 0; c < DFACOLSIZE; ++c)
//	{
//		GROUPHASH::iterator colIt = ghash.find(colVec[c]);
//		if(colIt == ghash.end())
//		{
//			ghash[colVec[c]] = curId;
//			group[c] = curId;
//			++curId;
//		}
//		else
//		{
//			group[c] = colIt->second;
//		}
//	}
//}

//使用string进行hash，正确，快
//void NAvaiEdges(CNfa &oneNfaTab, STATEID *group)
//{
//	//typedef std::unordered_map<std::vector<CStateSet*>, STATEID, GROUPSET_HASH> GROUPHASH;
//	typedef std::unordered_map<std::string, STATEID> GROUPHASH;//将每一列表示成string形式，size=0的用'n'代表，每行之间用'u'间隔,元素与元素之间用','间隔
//	GROUPHASH ghash;
//	STATEID curId = 0;
//	std::stringstream ss;
//	size_t nSize = oneNfaTab.Size();
//	for (size_t i = 0; i < nSize; ++i)
//	{
//		oneNfaTab[i].SortAllDest();
//	}
//	std::string str;
//	str.reserve(10000);
//	for(size_t c = 0; c < DFACOLSIZE; ++c)
//	{
//		str.clear();
//		for(size_t i = 0; i < nSize; ++i)
//		{
//			CNfaRow &row = oneNfaTab[i];
//			//CStateSet &elems = oneNfaTab[i][c];
//			size_t nCnt = row.DestCnt(c);
//			for(size_t j = 0; j < nCnt; ++j)
//			{
//				size_t nSta = row.GetDest(c, j);
//				if(nSta > nSize)
//				{
//					std::cout << "overflow" << std::endl;
//					return;
//				}
//				ss.str("");
//				ss << nSta;
//				str += ss.str();
//				//str += nSta;
//				str.push_back(',');
//			}
//			str.push_back('u');
//		}
//		GROUPHASH::iterator it = ghash.find(str);
//		if(it == ghash.end())
//		{
//			ghash[str] = curId;
//			group[c] = curId;
//			++curId;
//		}
//		else
//		{
//			group[c] = it->second;
//		}
//	}
//}
//
//原始代码，正确，慢
//void NAvaiEdges(CNfa &oneNfaTab, STATEID *group)
//{
//	std::vector<std::vector<size_t>> charGroups;
//	std::vector<CStateSet*> column[DFACOLSIZE];
//	for(size_t charNum = 0; charNum < DFACOLSIZE; ++charNum)
//	{
//		column[charNum].reserve(20000);
//		for(size_t i = 0; i < oneNfaTab.Size(); ++i)
//		{
//			CStateSet &elem = oneNfaTab[i][charNum];
//			elem.Sort();
//			column[charNum].push_back(&elem);
//			for (size_t j = 0; j < elem.Size(); ++j)
//			{
//				if(elem[j] > oneNfaTab.Size())
//				{
//					std::cout << "overflow" << std::endl;
//					return;
//				}
//			}
//		}
//	}
//
//	charGroups.clear();
//	std::vector<size_t> fullSet;
//
//	//std::cout << "columns complete" << std::endl;
//
//	for (size_t i = 0; i < DFACOLSIZE; ++i)
//	{
//		fullSet.push_back(i);
//	}
//
//	for (; !fullSet.empty();)
//	{
//		charGroups.push_back(std::vector<size_t>());
//		std::vector<size_t> &curGroup = charGroups.back();
//		curGroup.push_back(fullSet.front());
//		fullSet.erase(fullSet.begin());
//		for (std::vector<size_t>::iterator i = fullSet.begin(); i != fullSet.end() && !fullSet.empty();)
//		{
//			if (NColumnEqual(column[curGroup.front()], column[*i]))
//			{
//				curGroup.push_back(*i);
//				i = fullSet.erase(i);
//			}
//			else
//			{
//				++i;
//			}
//		}
//	}
//	//std::cout << "grouping complete" << std::endl;
//
//	for(STATEID i = 0; i < charGroups.size(); ++i)
//	{
//		for(STATEID j = 0; j < charGroups[i].size(); ++j)
//		{
//			group[charGroups[i][j]] = i;
//		}
//	}
//}
