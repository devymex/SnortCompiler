#pragma once

#include <vector>
#include <hwprj\dfa.h>
#include <hwprj\dfa.h>
#include <hwprj\unsary.h>

#ifndef DFACOMPRESS_DS
#define DFACOMPRESS __declspec(dllimport)
#else
#define DFACOMPRESS __declspec(dllexport)
#endif

DFACOMPRESS void DfaCompress(CDfa &olddfa);
void GetDfaCluster(CDfa &olddfa, std::vector<CUnsignedArray> &clusterVec, STATEID *stateCluster);//获得dfa的簇信息
