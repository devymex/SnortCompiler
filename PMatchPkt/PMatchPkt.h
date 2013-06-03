#include <unordered_map>
#include "../common/common.h"
#include "../rule2nfa/rule2nfa.h"
#include "../compilernew/compilernew.h"

#ifndef PMATCHPKT_H_
#define PMATCHPKT __declspec(dllimport)
#else
#define PMATCHPKT __declspec(dllexport)
#endif


struct REGRULES
{
	size_t m_nSid;
	CRegRule regrule;
};

struct SIG_HASH
{
	size_t operator()(const SIGNATURE &str)
	{
		const size_t _FNV_offset_basis = 2166136261U;
		const size_t _FNV_prime = 16777619U;

		size_t _Val = _FNV_offset_basis;	
		for(size_t i = 0; i < 4; ++i)
		{
			_Val ^= str >> (8 * i);
			_Val *= _FNV_prime;
		}

		return (_Val);
	}
};

typedef std::unordered_map<SIGNATURE, std::vector<size_t>, SIG_HASH> SIGSMAP;
struct REGRULESMAP
{
	std::vector<REGRULES> result;
	 SIGSMAP sigmap;
};

void CALLBACK MyProcess(const CSnortRule &rule, LPVOID lpParam);
PMATCHPKT void MchCompile(LPCTSTR filename, LPVOID result);