#pragma once
//#include "stdafx.h"
//#include "CreDfa.h"

#include "../SnortCommon/snortcommon.h"

#define CHARSETSIZE 260


void IncreNfaStaNum(int increNum, CNfa &curNfa);

//void SetPoEdge(CNfa &seriaNfa, int begin, int end);

//CNfa IntersectNfa(CNfa &firNfa, CNfa &secNfa);

//void InterSection(std::vector<size_t> &firVec, std::vector<size_t> &secVec, size_t firNfaSize, size_t secNfaSize,std::map<_int64,size_t> &newIdMap, std::vector<size_t> &nextStas);

//void  RowIntersection(size_t fStaNum, size_t sStaNum, CNfaRow &firSta, CNfaRow &secSta, size_t firSize, size_t secSize, std::map<_int64, size_t> &newIdMap, CNfaRow &newSta);

//void HalfRowIntersection(bool fORs, CNfaRow &newSta, CNfaRow fORSSta, size_t fORsSize, std::map<_int64, size_t> newIdMap);

//void combineNfaTree(std::vector<std::vector<CNfa>> &nfaTree, CNfa &lastNfa);

//////////////////////////////////////////////////////////////////

//void IntersectDfaRow(size_t firDfaSize, size_t secDfaSize, size_t firStaNum, size_t secStaNum, CDfaRow firRow, CDfaRow secRow, CDfaRow &newRow, std::map<_int64, size_t> newIdMap);

//void CombineTree(const CNfaTree &nfaTree, CDfa &lastDfa);

//CDfa IntersectDfa(CDfa firDfa, CDfa secDfa);

//void HalfIntersectDfa(bool terFlag, size_t firDfaSize, size_t secDfaSize, size_t firStaNum, size_t secStaNum, CDfaRow firRow, CDfaRow secRow, CDfaRow &newRow, std::map<_int64, size_t> newIdMap);
