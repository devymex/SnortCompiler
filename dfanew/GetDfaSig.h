#include "../common/common.h"
#include "dfanew.h"

#define LENGTH 4

void DeepSearch(STATEID startSta, STATEID &serNum, CDfaNew &dfa, STATEVEC &termStas, size_t visited[], STATEID deepSer[], STATEID staRow[]);
void Dominates(CDfaNew &dfa, STATEVEC termStas, INT64 domMax[], STATEID deepSer[], STATEID staRow[], STATEVEC &doms);
void InStas(CDfaNew &dfa, STATEVEC *inStas);
void RowAnd(INT64* firRow, INT64* secRow, INT64* lastRow);
bool Change(INT64* before, INT64* after);
void WFSDfa(CDfaNew &dfa, STATEVEC doms, STATEID *staRow, std::vector<std::vector<BYTE>> &allStr);
