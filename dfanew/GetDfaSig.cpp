#include "stdafx.h"
#include "dfanew.h"
#include "GetDfaSig.h"
#include "../common/common.h"



DFANEWSC void GetDfaSig(CDfanew &dfa)
{
	static STATEID serNum = 0;

	std::vector<STATEID> doms;
	//记录所有的终态
	std::vector<STATEID> termStas;
	//深度优先遍历DFA时标记已被访问过的状态
	size_t visited[DFACOLSIZE];
	std::memset((char*)visited, 0, sizeof(visited));

	//顺序记录深度优先搜索DFA时访问的状态
	STATEID deepSer[DFACOLSIZE];
	std::memset((char*)deepSer, BYTE(-1), sizeof(deepSer));

	//支配点矩阵
	INT64 domMax[4 * 256];
	std::memset(domMax, INT64(-1), sizeof(domMax));
	//UINT8 domMax[DFACOLSIZE][DFACOLSIZE];
	//std::memset(domMax, 1, sizeof(domMax));

	//记录所有点在支配点矩阵中的行号
	STATEID staRow[DFACOLSIZE];
	std::memset((char*)staRow, (STATEID)-1, sizeof(staRow));

	DeepSearch(dfa.GetStartId(), serNum, dfa, termStas, visited, deepSer, staRow);

	Dominates(dfa, termStas, domMax, deepSer, staRow, doms);
	std::vector<CCString> allStr;
	WFSDfa(dfa, doms, staRow, allStr);
	std::cout << std::endl;
}

//深度遍历dfa，填写deepser,terRow,staRow
void DeepSearch(STATEID startSta, STATEID &serNum, CDfanew &dfa, std::vector<STATEID> &termStas, size_t visited[], STATEID deepSer[], STATEID staRow[])
{
	if((dfa[startSta].GetFlag() & CDfaRow::TERMINAL) != 0)
	{
		termStas.push_back(startSta);
	}
	visited[startSta] = 1;
	std::vector<STATEID> stack;

	STATEID cursta = startSta;
	staRow[cursta] = serNum;
	deepSer[serNum] = cursta;
	++serNum;

	for(STATEID group = 0; group < dfa.GetGroupCount(); ++group)
	{
		STATEID nextsta = dfa[cursta][group];

		if((nextsta != (STATEID)-1) && (visited[nextsta] == 0))
		{
			visited[nextsta] = 1;
			stack.resize(stack.size() + 1);
			stack.back() = nextsta;
		}
	}

	while(!stack.empty())
	{
		cursta = stack.front();
		stack.erase(stack.begin());
		DeepSearch(cursta, serNum, dfa, termStas, visited, deepSer, staRow);
	}
}

void Dominates(CDfanew &dfa, std::vector<STATEID> termStas, INT64 domMax[], STATEID deepSer[], STATEID staRow[], std::vector<STATEID> &doms)
{
	std::vector<STATEID> inStas[DFACOLSIZE];
	InStas(dfa, inStas);
	//initial domMax first row
	domMax[0] &= (INT64)1 << 63;
	for(size_t i = 1; i < 4; ++i)
	{
		domMax[i] = 0;
	}

	bool chanFlag = true;
	while(chanFlag)
	{
		chanFlag = false;
		for(size_t i = 1; i < dfa.Size(); ++i)
		{
			INT64 temp[LENGTH];
			for(size_t t = 0; t < LENGTH; ++t)
			{
				temp[t] = domMax[i * 4 + t];
			}

			STATEID cursta = deepSer[i];
			for(size_t in = 0; in < inStas[cursta].size(); ++in)
			{
				size_t row = staRow[inStas[cursta][in]];
				RowAnd(&domMax[i * 4], &domMax[row * 4], &domMax[i * 4]);
			}
			//将第i行第i列置1
			size_t cur = i * 4 + i / 64;

			domMax[cur] |= (INT64)1 << (63 - i % 64);
	

			if(!chanFlag)
			{
				chanFlag =  Change(temp, &domMax[i * 4]);
			}
		}
	}

	//将多有终态的支配点取交集
	//for(size_t i = 0; i < termStas.size(); ++i)
	//{
	//	STATEID cur = staRow[termStas[i]];
	//	//只取矩阵中上三角的支配点
	//	for(size_t j = 0; j <= cur; ++j)
	//	{
	//		STATEID t = cur * 4 + j / 64;
	//		STATEID s = j % 64;
	//		if(j == 0)
	//		{
	//			if((domMax[t] & (INT64)1 << 63) != 0)
	//			{
	//				doms.push_back(deepSer[j]);
	//			}
	//		}
	//		else
	//		{
	//			if(s == 0)
	//			{
	//				if((domMax[t - 1] & (INT64)1) != 0)
	//				{
	//					doms.push_back(deepSer[j]);
	//				}
	//			}
	//			else
	//			{
	//				if((domMax[t] & ((INT64)1 << (63 - s))) != 0)
	//				{
	//					doms.push_back(deepSer[j]);
	//				}
	//			}
	//		}
	//	}
	//}
	size_t minDeep = DFACOLSIZE;
	INT64 termAnd[LENGTH];
	std::memset(termAnd, INT64(-1), sizeof(termAnd));
	for(size_t i = 0; i < termStas.size(); ++i)
	{
		STATEID row = staRow[termStas[i]];
		if(row < minDeep)
		{
			minDeep = row;
		}
		RowAnd(termAnd, &domMax[row * 4], termAnd);
	}

		for(size_t j = 0; j <= minDeep; ++j)
		{

			STATEID t = j / 64 ; 
			STATEID s = j % 64;
			if((termAnd[t] & (INT64)1 << (63 - s)) != (INT64)0)
			{
				doms.push_back(deepSer[j]);
			}
		}
}


void RowAnd(INT64* firRow, INT64* secRow, INT64* lastRow)
{
	for(size_t i = 0; i < LENGTH; ++i)
	{
		lastRow[i] = firRow[i] & secRow[i];
	}
}

//pre states of each state of dfa
void InStas(CDfanew &dfa, std::vector<STATEID> *inStas)
{
	UINT8 visited[DFACOLSIZE][DFACOLSIZE];
	std::memset(visited, 0, sizeof(visited));

	for(size_t i = 0; i < dfa.Size(); ++i)
	{
		for(size_t j = 0; j < dfa.GetGroupCount(); ++j)
		{
			STATEID nextsta = dfa[i][j];
			if(nextsta != (STATEID) -1)
			{
				if(visited[nextsta][i] == 0)
				{
					visited[nextsta][i] = 1;
					inStas[nextsta].push_back((STATEID)i);
				}
			}
		}
	}
}

bool Change(INT64* before, INT64* after)
{
	bool chg = false;
	for(size_t i = 0; i < LENGTH; ++i)
	{
		if(before[i] != after[i])
		{
			chg = true;
			break;
		}
	}
	return chg;
}

void WFSDfa(CDfanew &dfa, std::vector<STATEID> doms, STATEID *staRow, std::vector<CCString> &allStr)
{
	//入度
	size_t in[DFACOLSIZE - 1];
	//出度
	size_t out[DFACOLSIZE - 1];
	//记录所有入边
	std::vector<STATEID> inEdges[DFACOLSIZE - 1];
	STATEID inEdgeVis[DFACOLSIZE - 1][DFACOLSIZE];
	//深度
	STATEID deeps[DFACOLSIZE - 1];
	STATEID visited[DFACOLSIZE];
	std::memset(in, 0, sizeof(in));
	std::memset(out, 0, sizeof(out));
	std::memset(deeps, BYTE(-1), sizeof(deeps));
	std::memset(visited, 0, sizeof(visited));
	std::memset(inEdges, 0, sizeof(inEdges));
	std::memset(inEdgeVis, 0, sizeof(inEdgeVis));

	std::vector<STATEID> stack;
	stack.push_back(dfa.GetStartId());
	deeps[dfa.GetStartId()] = 0;
	while (!stack.empty())
	{
		STATEID cur = stack.front();
		stack.erase(stack.begin());
		if (visited[cur] == 0)
		{
			STATEID tempVisit[DFACOLSIZE - 1];
			std::memset(tempVisit, 0, sizeof(tempVisit));
			visited[cur] = 1;
			for(size_t i = 0; i < DFACOLSIZE; ++i)
			{
				size_t group = dfa.GetOneGroup(i);
				STATEID next = dfa[cur][group];
				if (next != BYTE(-1))
				{
					++out[cur];
					++in[next];
					if(staRow[next] != STATEID(-1))
					{
						inEdges[next].push_back(i);
						if(inEdgeVis[next][i] == 0)
						{
							inEdgeVis[next][i] = 1;
						}
					}
					if((tempVisit[next] == 0) && (visited[next] == 0))
					{
						tempVisit[next] = 1;
						deeps[next] = deeps[cur] + 1;
						stack.resize(stack.size() + 1);
						stack.back() = next;
					}
				}
			}
		}
	}
	//judge if all the in edges are the same
	STATEID sameIn[DFACOLSIZE - 1];
	std::memset(sameIn, 0, sizeof(sameIn));
	for(std::vector<STATEID>::iterator iter = doms.begin(); iter != doms.end(); ++iter)
	{
		size_t count = 0;
		for(size_t i = 0; i < DFACOLSIZE; ++i)
		{
			if(inEdgeVis[*iter][i] == 1) 
			{
				++count;
			}

		}

		sameIn[*iter] = count;

	}

	CCString str;

	for(std::vector<STATEID>::iterator iter = doms.begin(); iter != doms.end(); ++iter)
	{
		if(sameIn[*iter] != 1)
		{
			continue;
		}
		else
		{
			//const char a = inEdges[*iter][0];
			std::string str1;
			str1 = inEdges[*iter][0];
			str.Append(str1.c_str());
		}
		if(out[*iter] > 1 | iter== (doms.end() - 1))
		{
			if(str.Size() != 0)
			{
				allStr.resize(allStr.size() + 1);
				allStr.back() = str;
				str.Clear();
			}
			continue;
		}
	}

	std::cout << std::endl;
}