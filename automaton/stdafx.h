#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <stdio.h>
#include <unordered_map>
#include <stack>
#include <limits>
#include <windows.h>
#include <xmmintrin.h>
#include <deque>
#include <exception>
#include <hwprj\common.h>
#include <hwprj\trace.h>
#include <hwprj\logger.h>

#undef max

#define DFAIDSETHDR_DS
#define FINSTAHDR_DS
#define DFAHDR_DS
#define NFAHDR_DS
#define DFAARYHDR_DS

typedef std::vector<STATEID>		STATEIDARY;
typedef STATEIDARY::iterator			STATEIDARY_ITER;
typedef STATEIDARY::const_iterator	STATEIDARY_CITER;

typedef std::list<STATEID>			STATELIST;
typedef STATELIST::iterator			STATELIST_ITER;
typedef STATELIST::const_iterator	STATELIST_CITER;

typedef std::vector<class CDfaRow>	DFAROWARY;
typedef std::vector<class CNfaRow>	NFAROWARY;
typedef std::vector<ulong>			ULONGVEC;

typedef std::set<DFAID>				DFAIDSET;
typedef std::vector<class CDfa>		CDFAVEC;

typedef std::unordered_map<STATEID, class CDfaIdSet> FINSTAMAP;
