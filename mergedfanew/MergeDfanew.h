#include "../common/common.h"
#include "../dfanew/dfanew.h"
#include "../dfanew/NCreDfa.h"

#ifndef MERDFANEW_H_
#define MERDFANEW __declspec(dllimport)
#else
#define MERDFANEW __declspec(dllexport)
#endif

MERDFANEW bool NOrMerge(std::vector<CDfanew> &dfas, CDfanew &lastDfa);

