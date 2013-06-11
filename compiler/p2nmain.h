#pragma once

#define OVECCOUNT	30	/* should be a multiple of 3 */
#define EBUFLEN		128
#define BUFLEN		1024

#define ONEPCRETONFA_H_
#include <hwprj\common.h>
#include <hwprj\nfa.h>


#define SC_LIMIT 200 // What's this??

#define IMM2_SIZE 2
#define LINK_SIZE 2
#define OP_LENGTHS \
	1,                             /* End                                    */ \
	1, 1, 1, 1, 1,                 /* \A, \G, \K, \B, \b                     */ \
	1, 1, 1, 1, 1, 1,              /* \D, \d, \S, \s, \W, \w                 */ \
	1, 1, 1,                       /* Any, AllAny, Anybyte                   */ \
	3, 3,                          /* \P, \p                                 */ \
	1, 1, 1, 1, 1,                 /* \R, \H, \h, \V, \v                     */ \
	1,                             /* \X                                     */ \
	1, 1, 1, 1, 1, 1,              /* \Z, \z, ^, ^M, $, $M                   */ \
	2,                             /* Char  - the minimum length             */ \
	2,                             /* Chari  - the minimum length            */ \
	2,                             /* not                                    */ \
	2,                             /* noti                                   */ \
	/* Positive single-char repeats                             ** These are */ \
	2, 2, 2, 2, 2, 2,              /* *, *?, +, +?, ?, ??       ** minima in */ \
	2+IMM2_SIZE, 2+IMM2_SIZE,      /* upto, minupto             ** mode      */ \
	2+IMM2_SIZE,                   /* exact                                  */ \
	2, 2, 2, 2+IMM2_SIZE,          /* *+, ++, ?+, upto+                      */ \
	2, 2, 2, 2, 2, 2,              /* *I, *?I, +I, +?I, ?I, ??I ** UTF-8     */ \
	2+IMM2_SIZE, 2+IMM2_SIZE,      /* upto I, minupto I                      */ \
	2+IMM2_SIZE,                   /* exact I                                */ \
	2, 2, 2, 2+IMM2_SIZE,          /* *+I, ++I, ?+I, upto+I                  */ \
	/* Negative single-char repeats - only for chars < 256                   */ \
	2, 2, 2, 2, 2, 2,              /* NOT *, *?, +, +?, ?, ??                */ \
	2+IMM2_SIZE, 2+IMM2_SIZE,      /* NOT upto, minupto                      */ \
	2+IMM2_SIZE,                   /* NOT exact                              */ \
	2, 2, 2, 2+IMM2_SIZE,          /* Possessive NOT *, +, ?, upto           */ \
	2, 2, 2, 2, 2, 2,              /* NOT *I, *?I, +I, +?I, ?I, ??I          */ \
	2+IMM2_SIZE, 2+IMM2_SIZE,      /* NOT upto I, minupto I                  */ \
	2+IMM2_SIZE,                   /* NOT exact I                            */ \
	2, 2, 2, 2+IMM2_SIZE,          /* Possessive NOT *I, +I, ?I, upto I      */ \
	/* Positive type repeats                                                 */ \
	2, 2, 2, 2, 2, 2,              /* Type *, *?, +, +?, ?, ??               */ \
	2+IMM2_SIZE, 2+IMM2_SIZE,      /* Type upto, minupto                     */ \
	2+IMM2_SIZE,                   /* Type exact                             */ \
	2, 2, 2, 2+IMM2_SIZE,          /* Possessive *+, ++, ?+, upto+           */ \
	/* Character class & ref repeats                                         */ \
	1, 1, 1, 1, 1, 1,              /* *, *?, +, +?, ?, ??                    */ \
	1+2*IMM2_SIZE, 1+2*IMM2_SIZE,  /* CRRANGE, CRMINRANGE                    */ \
	1+(32/sizeof(byte)),     /* CLASS                                  */ \
	1+(32/sizeof(byte)),     /* NCLASS                                 */ \
	0,                             /* XCLASS - variable length               */ \
	1+IMM2_SIZE,                   /* REF                                    */ \
	1+IMM2_SIZE,                   /* REFI                                   */ \
	1+LINK_SIZE,                   /* RECURSE                                */ \
	2+2*LINK_SIZE,                 /* CALLOUT                                */ \
	1+LINK_SIZE,                   /* Alt                                    */ \
	1+LINK_SIZE,                   /* Ket                                    */ \
	1+LINK_SIZE,                   /* KetRmax                                */ \
	1+LINK_SIZE,                   /* KetRmin                                */ \
	1+LINK_SIZE,                   /* KetRpos                                */ \
	1+LINK_SIZE,                   /* Reverse                                */ \
	1+LINK_SIZE,                   /* Assert                                 */ \
	1+LINK_SIZE,                   /* Assert not                             */ \
	1+LINK_SIZE,                   /* Assert behind                          */ \
	1+LINK_SIZE,                   /* Assert behind not                      */ \
	1+LINK_SIZE,                   /* ONCE                                   */ \
	1+LINK_SIZE,                   /* ONCE_NC                                */ \
	1+LINK_SIZE,                   /* BRA                                    */ \
	1+LINK_SIZE,                   /* BRAPOS                                 */ \
	1+LINK_SIZE+IMM2_SIZE,         /* CBRA                                   */ \
	1+LINK_SIZE+IMM2_SIZE,         /* CBRAPOS                                */ \
	1+LINK_SIZE,                   /* COND                                   */ \
	1+LINK_SIZE,                   /* SBRA                                   */ \
	1+LINK_SIZE,                   /* SBRAPOS                                */ \
	1+LINK_SIZE+IMM2_SIZE,         /* SCBRA                                  */ \
	1+LINK_SIZE+IMM2_SIZE,         /* SCBRAPOS                               */ \
	1+LINK_SIZE,                   /* SCOND                                  */ \
	1+IMM2_SIZE, 1+IMM2_SIZE,      /* CREF, NCREF                            */ \
	1+IMM2_SIZE, 1+IMM2_SIZE,      /* RREF, NRREF                            */ \
	1,                             /* DEF                                    */ \
	1, 1, 1,                       /* BRAZERO, BRAMINZERO, BRAPOSZERO        */ \
	3, 1, 3,                       /* MARK, PRUNE, PRUNE_ARG                 */ \
	1, 3,                          /* SKIP, SKIP_ARG                         */ \
	1, 3,                          /* THEN, THEN_ARG                         */ \
	1, 1, 1, 1,                    /* COMMIT, FAIL, ACCEPT, ASSERT_ACCEPT    */ \
	1+IMM2_SIZE, 1                 /* CLOSE, SKIPZERO                        */

enum {
	OP_END,            /* 0 End of pattern */

	/* Values corresponding to backslashed metacharacters */

	OP_SOD,            /* 1 Start of data: \A */
	OP_SOM,            /* 2 Start of match (subject + offset): \G */
	OP_SET_SOM,        /* 3 Set start of match (\K) */
	OP_NOT_WORD_BOUNDARY,  /*  4 \B */
	OP_WORD_BOUNDARY,      /*  5 \b */
	OP_NOT_DIGIT,          /*  6 \D */
	OP_DIGIT,              /*  7 \d */
	OP_NOT_WHITESPACE,     /*  8 \S */
	OP_WHITESPACE,         /*  9 \s */
	OP_NOT_WORDCHAR,       /* 10 \W */
	OP_WORDCHAR,           /* 11 \w */

	OP_ANY,            /* 12 Match any character except newline (\N) */
	OP_ALLANY,         /* 13 Match any character */
	OP_ANYBYTE,        /* 14 Match any byte (\C); different to OP_ANY for UTF-8 */
	OP_NOTPROP,        /* 15 \P (not Unicode property) */
	OP_PROP,           /* 16 \p (Unicode property) */
	OP_ANYNL,          /* 17 \R (any newline sequence) */
	OP_NOT_HSPACE,     /* 18 \H (not horizontal whitespace) */
	OP_HSPACE,         /* 19 \h (horizontal whitespace) */
	OP_NOT_VSPACE,     /* 20 \V (not vertical whitespace) */
	OP_VSPACE,         /* 21 \v (vertical whitespace) */
	OP_EXTUNI,         /* 22 \X (extended Unicode sequence */
	OP_EODN,           /* 23 End of data or \n at end of data (\Z) */
	OP_EOD,            /* 24 End of data (\z) */

	OP_CIRC,           /* 25 Start of line - not multiline */
	OP_CIRCM,          /* 26 Start of line - multiline */
	OP_DOLL,           /* 27 End of line - not multiline */
	OP_DOLLM,          /* 28 End of line - multiline */
	OP_CHAR,           /* 29 Match one character, casefully */
	OP_CHARI,          /* 30 Match one character, caselessly */
	OP_NOT,            /* 31 Match one character, not the given one, casefully */
	OP_NOTI,           /* 32 Match one character, not the given one, caselessly */

	/* The following sets of 13 opcodes must always be kept in step because
	the offset from the first one is used to generate the others. */

	/**** Single characters, caseful, must precede the caseless ones ****/

	OP_STAR,           /* 33 The maximizing and minimizing versions of */
	OP_MINSTAR,        /* 34 these six opcodes must come in pairs, with */
	OP_PLUS,           /* 35 the minimizing one second. */
	OP_MINPLUS,        /* 36 */
	OP_QUERY,          /* 37 */
	OP_MINQUERY,       /* 38 */

	OP_UPTO,           /* 39 From 0 to n matches of one character, caseful*/
	OP_MINUPTO,        /* 40 */
	OP_EXACT,          /* 41 Exactly n matches */

	OP_POSSTAR,        /* 42 Possessified star, caseful */
	OP_POSPLUS,        /* 43 Possessified plus, caseful */
	OP_POSQUERY,       /* 44 Posesssified query, caseful */
	OP_POSUPTO,        /* 45 Possessified upto, caseful */

	/**** Single characters, caseless, must follow the caseful ones */

	OP_STARI,          /* 46 */
	OP_MINSTARI,       /* 47 */
	OP_PLUSI,          /* 48 */
	OP_MINPLUSI,       /* 49 */
	OP_QUERYI,         /* 50 */
	OP_MINQUERYI,      /* 51 */

	OP_UPTOI,          /* 52 From 0 to n matches of one character, caseless */
	OP_MINUPTOI,       /* 53 */
	OP_EXACTI,         /* 54 */

	OP_POSSTARI,       /* 55 Possessified star, caseless */
	OP_POSPLUSI,       /* 56 Possessified plus, caseless */
	OP_POSQUERYI,      /* 57 Posesssified query, caseless */
	OP_POSUPTOI,       /* 58 Possessified upto, caseless */

	/**** The negated ones must follow the non-negated ones, and match them ****/
	/**** Negated single character, caseful; must precede the caseless ones ****/

	OP_NOTSTAR,        /* 59 The maximizing and minimizing versions of */
	OP_NOTMINSTAR,     /* 60 these six opcodes must come in pairs, with */
	OP_NOTPLUS,        /* 61 the minimizing one second. They must be in */
	OP_NOTMINPLUS,     /* 62 exactly the same order as those above. */
	OP_NOTQUERY,       /* 63 */
	OP_NOTMINQUERY,    /* 64 */

	OP_NOTUPTO,        /* 65 From 0 to n matches, caseful */
	OP_NOTMINUPTO,     /* 66 */
	OP_NOTEXACT,       /* 67 Exactly n matches */

	OP_NOTPOSSTAR,     /* 68 Possessified versions, caseful */
	OP_NOTPOSPLUS,     /* 69 */
	OP_NOTPOSQUERY,    /* 70 */
	OP_NOTPOSUPTO,     /* 71 */

	/**** Negated single character, caseless; must follow the caseful ones ****/

	OP_NOTSTARI,       /* 72 */
	OP_NOTMINSTARI,    /* 73 */
	OP_NOTPLUSI,       /* 74 */
	OP_NOTMINPLUSI,    /* 75 */
	OP_NOTQUERYI,      /* 76 */
	OP_NOTMINQUERYI,   /* 77 */

	OP_NOTUPTOI,       /* 78 From 0 to n matches, caseless */
	OP_NOTMINUPTOI,    /* 79 */
	OP_NOTEXACTI,      /* 80 Exactly n matches */

	OP_NOTPOSSTARI,    /* 81 Possessified versions, caseless */
	OP_NOTPOSPLUSI,    /* 82 */
	OP_NOTPOSQUERYI,   /* 83 */
	OP_NOTPOSUPTOI,    /* 84 */

	/**** Character types ****/

	OP_TYPESTAR,       /* 85 The maximizing and minimizing versions of */
	OP_TYPEMINSTAR,    /* 86 these six opcodes must come in pairs, with */
	OP_TYPEPLUS,       /* 87 the minimizing one second. These codes must */
	OP_TYPEMINPLUS,    /* 88 be in exactly the same order as those above. */
	OP_TYPEQUERY,      /* 89 */
	OP_TYPEMINQUERY,   /* 90 */

	OP_TYPEUPTO,       /* 91 From 0 to n matches */
	OP_TYPEMINUPTO,    /* 92 */
	OP_TYPEEXACT,      /* 93 Exactly n matches */

	OP_TYPEPOSSTAR,    /* 94 Possessified versions */
	OP_TYPEPOSPLUS,    /* 95 */
	OP_TYPEPOSQUERY,   /* 96 */
	OP_TYPEPOSUPTO,    /* 97 */

	/* These are used for character classes and back references; only the
	first six are the same as the sets above. */

	OP_CRSTAR,         /* 98 The maximizing and minimizing versions of */
	OP_CRMINSTAR,      /* 99 all these opcodes must come in pairs, with */
	OP_CRPLUS,         /* 100 the minimizing one second. These codes must */
	OP_CRMINPLUS,      /* 101 be in exactly the same order as those above. */
	OP_CRQUERY,        /* 102 */
	OP_CRMINQUERY,     /* 103 */

	OP_CRRANGE,        /* 104 These are different to the three sets above. */
	OP_CRMINRANGE,     /* 105 */

	/* End of quantifier opcodes */

	OP_CLASS,          /* 106 Match a character class, chars < 256 only */
	OP_NCLASS,         /* 107 Same, but the bitmap was created from a negative
					   class - the difference is relevant only when a
					   character > 255 is encountered. */
	OP_XCLASS,         /* 108 Extended class for handling > 255 chars within the
						class. This does both positive and negative. */
	OP_REF,            /* 109 Match a back reference, casefully */
	OP_REFI,           /* 110 Match a back reference, caselessly */
	OP_RECURSE,        /* 111 Match a numbered subpattern (possibly recursive) */
	OP_CALLOUT,        /* 112 Call out to external function if provided */

	OP_ALT,            /* 113 Start of alternation */
	OP_KET,            /* 114 End of group that doesn't have an unbounded repeat */
	OP_KETRMAX,        /* 115 These two must remain together and in this */
	OP_KETRMIN,        /* 116 order. They are for groups the repeat for ever. */
	OP_KETRPOS,        /* 117 Possessive unlimited repeat. */

						/* The assertions must come before BRA, CBRA, ONCE, and COND, and the four
						asserts must remain in order. */

	OP_REVERSE,        /* 118 Move pointer back - used in lookbehind assertions */
	OP_ASSERT,         /* 119 Positive lookahead */
	OP_ASSERT_NOT,     /* 120 Negative lookahead */
	OP_ASSERTBACK,     /* 121 Positive lookbehind */
	OP_ASSERTBACK_NOT, /* 122 Negative lookbehind */

	/* ONCE, ONCE_NC, BRA, BRAPOS, CBRA, CBRAPOS, and COND must come immediately
	after the assertions, with ONCE first, as there's a test for >= ONCE for a
	subpattern that isn't an assertion. The POS versions must immediately follow
	the non-POS versions in each case. */

	OP_ONCE,           /* 123 Atomic group, contains captures */
	OP_ONCE_NC,        /* 124 Atomic group containing no captures */
	OP_BRA,            /* 125 Start of non-capturing bracket */
	OP_BRAPOS,         /* 126 Ditto, with unlimited, possessive repeat */
	OP_CBRA,           /* 127 Start of capturing bracket */
	OP_CBRAPOS,        /* 128 Ditto, with unlimited, possessive repeat */
	OP_COND,           /* 129 Conditional group */

	/* These five must follow the previous five, in the same order. There's a
	check for >= SBRA to distinguish the two sets. */

	OP_SBRA,           /* 130 Start of non-capturing bracket, check empty  */
	OP_SBRAPOS,        /* 131 Ditto, with unlimited, possessive repeat */
	OP_SCBRA,          /* 132 Start of capturing bracket, check empty */
	OP_SCBRAPOS,       /* 133 Ditto, with unlimited, possessive repeat */
	OP_SCOND,          /* 134 Conditional group, check empty */

	/* The next two pairs must (respectively) be kept together. */

	OP_CREF,           /* 135 Used to hold a capture number as condition */
	OP_NCREF,          /* 136 Same, but generated by a name reference*/
	OP_RREF,           /* 137 Used to hold a recursion number as condition */
	OP_NRREF,          /* 138 Same, but generated by a name reference*/
	OP_DEF,            /* 139 The DEFINE condition */

	OP_BRAZERO,        /* 140 These two must remain together and in this */
	OP_BRAMINZERO,     /* 141 order. */
	OP_BRAPOSZERO,     /* 142 */

	/* These are backtracking control verbs */

	OP_MARK,           /* 143 always has an argument */
	OP_PRUNE,          /* 144 */
	OP_PRUNE_ARG,      /* 145 same, but with argument */
	OP_SKIP,           /* 146 */
	OP_SKIP_ARG,       /* 147 same, but with argument */
	OP_THEN,           /* 148 */
	OP_THEN_ARG,       /* 149 same, but with argument */
	OP_COMMIT,         /* 150 */

	/* These are forced failure and success verbs */

	OP_FAIL,           /* 151 */
	OP_ACCEPT,         /* 152 */
	OP_ASSERT_ACCEPT,  /* 153 Used inside assertions */
	OP_CLOSE,          /* 154 Used before OP_ACCEPT to close open captures */

	/* This is used to skip a subpattern with a {0} quantifier */

	OP_SKIPZERO,       /* 155 */

	/* This is not an opcode, but is used to check that tables indexed by opcode
	are the correct length, in order to catch updating errors - there have been
	some in the past. */

	OP_TABLE_LENGTH
};

#define MAX 65535
#define EMPTY 256

#define GET(Iter) \
	((*(Iter) << 8) | *(Iter + 1))

extern const byte Steps[];

extern enum PCRESIGN;

struct PCRE
{
	std::string PcreStr;//pcre字符串
	BYTEARY PcreCode;//存放经过pcre库解析后的pcre
};

//void GenerateNFA(std::vector<PCRE> &vecPcres);

bool CanProcess(BYTEARY_ITER &Beg, const BYTEARY_ITER &End);

void NextForCLASS(BYTEARY_ITER &Beg);

ulong ProcessPcre(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa);

ulong Process(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, std::vector<ulong> &PreStates, ulong ALTPreBeg, bool &ALTBeg, ulong ALTBeginState, bool bCBRA, bool bALT, bool bBRAZERO, std::vector<PCRESIGN> &vecPath);

void ProcessALT(CNfa &nfa, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_COMMON_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

void OP_CIRCM_FUNC(CNfa &nfa, ulong &CurState);

ulong OP_CHAR_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_CHARI_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_NOT_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_NOTI_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_STAR_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_PLUS_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_QUERY_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_UPTO_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_EXACT_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_STARI_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_PLUSI_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_QUERYI_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_UPTOI_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_EXACTI_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_NOTSTAR_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_NOTPLUS_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_NOTQUERY_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_NOTUPTO_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_NOTEXACT_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_NOTSTARI_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_NOTPLUSI_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_NOTQUERYI_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_NOTUPTOI_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_NOTEXACTI_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_TYPESTAR_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_TYPEPLUS_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_TYPEQUERY_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_TYPEUPTO_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_TYPEEXACT_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_CLASS_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

ulong OP_NCLASS_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState);

void OP_ALT_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong PreState, ulong &CurState);

void OP_KET_FUNC(CNfa &nfa, ulong PreState, ulong &CurState, bool IsALT, bool IsBRAZERO);

void OP_KETRMAX_FUNC(CNfa &nfa, ulong PreState, ulong &CurState, bool IsBRAZERO, bool IsALT);

ulong OP_BRA_CBRA_SCBRA_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState);

ulong OP_BRA_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState);

ulong OP_CBRA_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState);

ulong OP_SCBRA_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState);

void Copy(CNfaRow &NewRow, CNfaRow &Row, ulong increment);

void AddEMPTY(CNfa &nfa, ulong &CurState);

void OutPut(std::vector<PCRE>::iterator &Pcre, CNfa &nfa, ulong count);

typedef ulong (*Fn)(BYTEARY_ITER&, const BYTEARY_ITER&, CNfa&, ulong&, ulong, bool&, ulong);

extern Fn FUNC[156];

extern ulong OP_NOT_DIGIT_ELEMS[];

extern ulong OP_DIGIT_ELEMS[];

extern ulong OP_NOT_WHITESPACE_ELEMS[];

extern ulong OP_WHITESPACE_ELEMS[];

extern ulong OP_NOT_WORDCHAR_ELEMS[];

extern ulong OP_WORDCHAR_ELEMS[];

extern ulong OP_ANY_ELEMS[];

extern ulong OP_ALLANY_ELEMS[];

extern ulong* ptr[];

extern ulong NUMS[];
