#pragma once

#include <hwprj\groupres.h>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <algorithm>
#include <cmath>
#include <map>
#include <fstream>
#include <unordered_map>
#include <limits>

#include <set>

typedef std::vector<size_t> ROWSET;
typedef std::vector<ROWSET> VECROWSET;
typedef VECROWSET::iterator NODEARRAY_ITER;
typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef std::vector<ushort> ROW;
typedef std::vector<ROW> MATRIX;

typedef struct
{
	size_t rowNum;
	size_t colNum;
}STATENODE;

typedef struct
{
	char jumpCharacter;
	ushort nextNode;
} SKIPNODE;

typedef struct
{
	ushort dfaId;
	ulong skipTableNum;
	std::vector<ushort> rowTransform;
} ROWTRANSFORM;

typedef struct
{
	ushort column;
	std::vector<std::vector<ushort> > sameColumnMatrix;
	std::vector<ROWTRANSFORM> rowTrans;
} COLUMNCOMBINE;

typedef std::vector<COLUMNCOMBINE> COLCOMBINEARRAY;
typedef std::vector<COLUMNCOMBINE>::iterator COLCOMBINEARRAY_ITERATOR;
typedef std::vector<STATENODE> CORESPOND;

void SameColDfaCombine(CDfaArray &CDfaSet, COLCOMBINEARRAY &colCombineArray);

void CoreCompress(std::vector<COLUMNCOMBINE> &allData, std::vector<std::vector<SKIPNODE> > &skipTable);

void WriteSkipTable(std::vector<COLUMNCOMBINE> &allData,const std::vector<std::vector<SKIPNODE> > &skipTable, const char * str);

void ReadSkipTable(const char *str, std::vector<std::vector<std::vector<char> > > &skipTable);