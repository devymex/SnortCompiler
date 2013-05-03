#include "stdafx.h"
#include "dfanew.h"
#include "GetDfaSig.h"
#include "../common/common.h"



DFANEWSC void GetDfaSig(CDfanew &dfa)
{
	static STATEID serNum = 0;

	//深度优先遍历DFA时标记已被访问过的状态
	size_t visited[DFACOLSIZE];
	std::memset((char*)visited, 0, sizeof(visited));

	//顺序记录深度优先搜索DFA时访问的状态
	STATEID deepSer[DFACOLSIZE];
	std::memset((char*)deepSer, BYTE(-1), sizeof(deepSer));

	//支配点矩阵
	INT64 domMax[4 * 256];
	std::memset(domMax, 0xFFFFFFFFFFFF, sizeof(domMax));
	//UINT8 domMax[DFACOLSIZE][DFACOLSIZE];
	//std::memset(domMax, 1, sizeof(domMax));

	//记录所有点在支配点矩阵中的行号
	STATEID staRow[DFACOLSIZE];
	std::memset((char*)staRow, (STATEID)-1, sizeof(staRow));

	DeepSearch(dfa.GetStartId(), serNum, dfa, visited, deepSer, staRow);

	Dominates(dfa, domMax, deepSer, staRow);

}

//深度遍历dfa，填写deepser,terRow,staRow
void DeepSearch(STATEID startSta, STATEID &serNum, CDfanew &dfa, size_t visited[], STATEID deepSer[], STATEID staRow[])
{
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
		DeepSearch(cursta, serNum, dfa, visited, deepSer, staRow);
	}
}

void Dominates(CDfanew &dfa, INT64 domMax[], STATEID deepSer[], STATEID staRow[])
{
	std::vector<STATEID> inStas[DFACOLSIZE];
	InStas(dfa, inStas);
	//initial domMax first row
	domMax[0] &= (UINT64)1;
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
			size_t cur = i * 4 + i / 64 + 1;
			domMax[cur] |= (INT64)1 << (63 - i % 64);

			if(!chanFlag)
			{
				chanFlag =  Change(temp, &domMax[i * 4]);
			}
		}
	}

	INT64 lastDoms[LENGTH];
	for()
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