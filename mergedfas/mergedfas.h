#include "../common/common.h"

#ifndef MERDFA_H_
#define MERDFA __declspec(dllimport)
#else
#define MERDFA __declspec(dllexport)
#endif

MERDFA void AndMerge(CDfa &dfa1, CDfa &dfa2, CDfa &andDfa, CVectorNumber &termFlag);
MERDFA void OrMerge(std::vector<CDfa> &dfas, CDfa &lastDfa);

