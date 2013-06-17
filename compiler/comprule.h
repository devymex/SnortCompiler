/*
**	@file		comprule.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Functions declaration of rules' processing
**
**	Includes compile rule etc.
**
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
protected:
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

void __stdcall CompileCallback(const PARSERESULT &parseRes, void *lpVoid);

void ParseOptions(std::string &ruleOptions, CSnortRule &snortRule);

void Rule2RegRule(const CSnortRule &rule, CRegRule &regrule);

ulong Chain2NFA(const CPcreChain &regchain, CNfa &nfa, CSignatures &sigs);

void CompileRule(LPCSTR rule, RECIEVER recv, LPVOID lpUser);

ulong LoadFile(const char *fileName, std::vector<std::string> &rules);

void Rule2Dfas(const CSnortRule &rule, CCompileResults &result, COMPILEDINFO &ruleResult);

void AssignSig(CCompileResults &result, ulong BegIdx, ulong EndIdx);

void Rule2Dfas(const CRegRule &rule, CCompileResults &result);
