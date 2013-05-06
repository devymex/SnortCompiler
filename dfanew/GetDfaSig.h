#include "../common/common.h"
#include "dfanew.h"

#define LENGTH 4

void DeepSearch(STATEID startSta, STATEID &serNum, CDfanew &dfa, std::vector<STATEID> &termStas, size_t visited[], STATEID deepSer[], STATEID staRow[]);
void Dominates(CDfanew &dfa, std::vector<STATEID> termStas, INT64 domMax[], STATEID deepSer[], STATEID staRow[], std::vector<STATEID> &doms);
void InStas(CDfanew &dfa, std::vector<STATEID> *inStas);
void RowAnd(INT64* firRow, INT64* secRow, INT64* lastRow);
bool Change(INT64* before, INT64* after);