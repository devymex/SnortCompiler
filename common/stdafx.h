/*
**	@file		stdafx.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Precompile header common library
**
*/

#include <Windows.h>
#include <algorithm>
#include <vector>
#include <list>
#include <iostream>
#include <iterator>
#include <fstream>

#define UNSARYHDR_DS
#define DLLSTRHDR_DS
#define TRACEHDR_DS
#define LOGGERHDR_DS

typedef std::string						DLLSTRING;
typedef std::vector<unsigned long>		DLLULONGVEC;
typedef std::vector<class CDllString>	DLLSTRINGARY;
typedef std::ostream					LOGFILE;
