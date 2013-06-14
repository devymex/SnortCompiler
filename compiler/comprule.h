#pragma once

#include <hwprj\snortrule.h>
#include <hwprj\regrule.h>
#include <hwprj\nfa.h>
#include <hwprj\compiler.h>

extern double rule2pcretime;
extern double pcre2nfatime;
extern double nfa2dfatime;
extern double dfamintimetime;


enum OPTIONCONTENTFLAGS
{
	CF_NOCASE		= (1 << 0),//1
	CF_OFFSET		= (1 << 1),//2
	CF_DEPTH		= (1 << 2),//4
	CF_DISTANCE		= (1 << 3),//8
	CF_WITHIN		= (1 << 4),//16
};

enum PCREFLAGS
{
	PF_i = (1 << 0),
	PF_s = (1 << 1),
	PF_m = (1 << 2),
	PF_x = (1 << 3),
	PF_A = (1 << 4),
	PF_E = (1 << 5),
	PF_G = (1 << 6),
	PF_R = (1 << 7),
	PF_U = (1 << 8),
	PF_B = (1 << 9),
	PF_P = (1 << 10),
	PF_H = (1 << 11),
	PF_M = (1 << 12),
	PF_C = (1 << 13),
	PF_O = (1 << 14),
	PF_I = (1 << 15),
	PF_D = (1 << 16),
	PF_K = (1 << 17),
	PF_S = (1 << 18),
	PF_Y = (1 << 19)
};

struct OPTIONCONTENT : public CRuleOption
{
	BYTEARY vecconts;//content data
	int nOffset;// offset constraint in snort rule
	int nDepth;//depth constraint in snort rule
	int nDistance;//distance constraint in snort rule
	int nWithin;//within constraint in snort rule
};


struct OPTIONPCRE : public CRuleOption
{
};

ulong CRegChainToNFA(CRegChain &regchain, CNfa &nfa);

ulong LoadFile(const char *fileName, std::vector<std::string> &rules);

void Rule2Dfas(const CSnortRule &rule, CCompileResults &result, COMPILEDRULE &ruleResult);

void AssignSig(CCompileResults &result, ulong BegIdx, ulong EndIdx);

ulong ProcessOption(std::string &ruleOptions, CSnortRule &snortRule);

ulong content2Pcre(OPTIONCONTENT *pContent, CDllString &pcreStr);
