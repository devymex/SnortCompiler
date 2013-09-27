/*!
* @file				comprule.h
* @author			Lab 435, Xidian University
* @brief			Functions declaration of rules' processing
* @copyright	本项目开发组版权所有。未经许可，不得复制、更改、编译、分发。
*/

#pragma once
#pragma warning (disable:4996)

#include <hwprj\snortrule.h>
#include <hwprj\regrule.h>
#include <hwprj\nfa.h>
#include <hwprj\compiler.h>

extern double rule2pcretime;
extern double pcre2nfatime;
extern double nfa2dfatime;
extern double dfamintimetime;

struct RULEOPTIONRAW
{
	std::string name;
	std::string value;
};

//eliminate quotation mark
template<typename _Iter>
void QuotedContext(_Iter &beg, _Iter &end)
{
	_Iter iBeg = std::find(beg, end, '"'), iEnd = end - 1;
	if (iBeg == end)
	{
		TTHROW(TI_INVALIDDATA);
	}
	++iBeg;

	for (; iEnd > iBeg && *iEnd != '"'; --iEnd);

	if (iEnd == iBeg)
	{
		TTHROW(TI_INVALIDDATA);
	}
	beg = iBeg;
	end = iEnd;
}

struct ISSPACE
{
	ISSPACE()
	{
		memset(g_map, 0, sizeof(g_map) / sizeof(g_map[0]));
		g_map[' '] = 1;
		g_map['\t'] = 1;
		g_map['\n'] = 1;
		g_map['\r'] = 1;
		g_map['\f'] = 1;
	}
	bool operator()(char c)
	{
		return g_map[c] == 1;
	}
private:
	char g_map[256];
};

extern ISSPACE g_isSpace;

struct ISEMPTYRULE
{
	bool operator()(const std::string &str)
	{
		if (str.empty())
		{
			return true;
		}
		STRING_CITER i = std::find_if_not(str.begin(), str.end(), g_isSpace);
		return (i == str.end() || *i == '#');
	}
};

struct ISCONTENT
{
	bool operator()(RULEOPTIONRAW &rp)
	{
		if (0 == stricmp("content", rp.name.c_str()) ||
			0 == stricmp("uricontent", rp.name.c_str()))
		{
			return true;
		}
		return false;
	}
};

inline bool CaselessComp(char a, char b)
{
	return tolower(a) == tolower(b);
}

struct INCLUDESEQUENCE
{
	const CByteArray *m_pSeq;
	bool m_bCaseless;
	INCLUDESEQUENCE(const CByteArray &seq, bool bCaseless)
		: m_pSeq(&seq), m_bCaseless(bCaseless)
	{
	}
	bool operator() (const CByteArray &seq)
	{
		const byte *pBeg = m_pSeq->Data(), *pEnd = pBeg + m_pSeq->Size();

		const byte *pFBeg = seq.Data(), *pFEnd = pFBeg + seq.Size();
		if (m_bCaseless)
		{
			return (pFEnd != std::search(pFBeg, pFEnd, pBeg, pEnd, CaselessComp));
		}
		else
		{
			return (pFEnd != std::search(pFBeg, pFEnd, pBeg, pEnd));
		}
	}
};

typedef std::vector<CByteArray>			PCRESEQUENCE;
typedef std::vector<PCRESEQUENCE>		CHAINSEQUENCE;
typedef std::vector<CHAINSEQUENCE>		RULESEQUENCE;

typedef std::vector<CByteArray>			CHAINCOMPDATA;
typedef std::vector<CHAINCOMPDATA>		RULECOMPDATA;


ulong __stdcall CompileCallback(const PARSERESULT &parseRes, void *lpVoid);

bool SeqIncBy(const CRegRule &regRule, const RULESEQUENCE &ruleSeq, ulong ulIdx);

bool IsOneContentChain(const CPcreChain &chain, const CHAINSEQUENCE &chainSeq);

void ProcessRule(CRegRule &regRule, RULECOMPDATA &result);

void ParseOptions(std::string &ruleOptions, CSnortRule &snortRule);

void Rule2RegRule(const CSnortRule &rule, CRegRule &regrule);

void CompileRule(pcstr rule, RECIEVER recv, void* lpUser);

void PreCompileRule(const CRegRule &regRule,
					RULESEQUENCE &ruleSeq,
					RULECOMPDATA &ruleCompData);

ulong LoadFile(const char *fileName, std::vector<std::string> &rules);

void Rule2Dfas(const CSnortRule &rule,
			   CCompileResults &result,
			   COMPILEDINFO &ruleResult);

void AssignSig(CCompileResults &result, ulong BegIdx, ulong EndIdx);

void Rule2Dfas(const CRegRule &rule, CCompileResults &result);
