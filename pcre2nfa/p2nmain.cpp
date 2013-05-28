#include "stdafx.h"
#include "p2nmain.h"
#include "pcre2nfa.h"

#pragma warning (push)
#pragma warning (disable : 4100)
const unsigned char Steps[] = { OP_LENGTHS };

size_t OP_NOT_DIGIT_ELEMS[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
	21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 
	47, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 
	84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108,
	109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 
	131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 
	153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 
	175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 
	197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218,
	219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 
	241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255};

size_t OP_DIGIT_ELEMS[] = {48, 49, 50, 51, 52, 53, 54, 55, 56, 57};

size_t OP_NOT_WHITESPACE_ELEMS[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 11, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 
	26, 27, 28, 29, 30, 31, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54,
	55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82,
	83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 
	109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131,
	132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 
	155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 
	178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
	201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 
	224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 
	247, 248, 249, 250, 251, 252, 253, 254, 255};

size_t OP_WHITESPACE_ELEMS[] = {9, 10, 12, 13, 32};

size_t OP_NOT_WORDCHAR_ELEMS[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 
	24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 58, 59, 60, 61, 62,
	63, 64, 91, 92, 93, 94, 96, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140,
	141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164,
	165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188,
	189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 
	213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 
	237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255};

size_t OP_WORDCHAR_ELEMS[] = {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 
	79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 95, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 
	112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122};

size_t OP_ANY_ELEMS[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
	31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,
	62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92,
	93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118,
	119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
	144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168,
	169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193,
	194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218,
	219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243,
	244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255};

size_t OP_ALLANY_ELEMS[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 
	28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
	60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 
	91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117,
	118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 
	143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 
	168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 
	193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 
	218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 
	243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255};

size_t* ptr[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	OP_NOT_DIGIT_ELEMS,
	OP_DIGIT_ELEMS,
	OP_NOT_WHITESPACE_ELEMS,
	OP_WHITESPACE_ELEMS,
	OP_NOT_WORDCHAR_ELEMS,
	OP_WORDCHAR_ELEMS,
	OP_ANY_ELEMS,
	OP_ALLANY_ELEMS
};

size_t NUMS[] = {
	0,
	0,
	0,
	0,
	0,
	0,
	sizeof(OP_NOT_DIGIT_ELEMS) / sizeof(size_t),
	sizeof(OP_DIGIT_ELEMS) / sizeof(size_t),
	sizeof(OP_NOT_WHITESPACE_ELEMS) / sizeof(size_t),
	sizeof(OP_WHITESPACE_ELEMS) / sizeof(size_t),
	sizeof(OP_NOT_WORDCHAR_ELEMS) / sizeof(size_t),
	sizeof(OP_WORDCHAR_ELEMS) / sizeof(size_t),
	sizeof(OP_ANY_ELEMS) / sizeof(size_t),
	sizeof(OP_ALLANY_ELEMS) / sizeof(size_t),
};

Fn FUNC[156] = 
{
	NULL,                       /* 0 End of pattern */

	/* Values corresponding to backslashed metacharacters */

	NULL,                       /* 1 Start of data: \A */
	NULL,                       /* 2 Start of match (subject + offset): \G */
	NULL,                       /* 3 Set start of match (\K) */
	NULL,                       /*  4 \B */
	NULL,                       /*  5 \b */
	OP_COMMON_FUNC,          /*  6 \D */
	OP_COMMON_FUNC,              /*  7 \d */
	OP_COMMON_FUNC,     /*  8 \S */
	OP_COMMON_FUNC,         /*  9 \s */
	OP_COMMON_FUNC,       /* 10 \W */
	OP_COMMON_FUNC,           /* 11 \w */

	OP_COMMON_FUNC,                /* 12 Match any character except newline (\N) */
	OP_COMMON_FUNC,             /* 13 Match any character */
	NULL,                       /* 14 Match any byte (\C); different to OP_ANY for UTF-8 */
	NULL,                       /* 15 \P (not Unicode property) */
	NULL,                       /* 16 \p (Unicode property) */
	NULL,                       /* 17 \R (any newline sequence) */
	NULL,                       /* 18 \H (not horizontal whitespace) */
	NULL,                       /* 19 \h (horizontal whitespace) */
	NULL,                       /* 20 \V (not vertical whitespace) */
	NULL,                       /* 21 \v (vertical whitespace) */
	NULL,                       /* 22 \X (extended Unicode sequence */
	NULL,                       /* 23 End of data or \n at end of data (\Z) */
	NULL,                       /* 24 End of data (\z) */

	NULL,                       /* 25 Start of line - not multiline */
	NULL,                       /* 26 Start of line - multiline */
	NULL,                       /* 27 End of line - not multiline */
	NULL,                       /* 28 End of line - multiline */
	OP_CHAR_FUNC,               /* 29 Match one character, casefully */
	OP_CHARI_FUNC,              /* 30 Match one character, caselessly */
	OP_NOT_FUNC,                /* 31 Match one character, not the given one, casefully */
	OP_NOTI_FUNC,               /* 32 Match one character, not the given one, caselessly */

	/* The following sets of 13 opcodes must always be kept in step because
	the offset from the first one is used to generate the others. */

	/**** Single characters, caseful, must precede the caseless ones ****/

	OP_STAR_FUNC,               /* 33 The maximizing and minimizing versions of */
	OP_STAR_FUNC,               /* 34 these six opcodes must come in pairs, with */
	OP_PLUS_FUNC,               /* 35 the minimizing one second. */
	OP_PLUS_FUNC,               /* 36 */
	OP_QUERY_FUNC,              /* 37 */
	NULL,                       /* 38 */

	OP_UPTO_FUNC,               /* 39 From 0 to n matches of one character, caseful*/
	NULL,                       /* 40 */
	OP_EXACT_FUNC,              /* 41 Exactly n matches */

	NULL,                       /* 42 Possessified star, caseful */
	NULL,                       /* 43 Possessified plus, caseful */
	NULL,                       /* 44 Posesssified query, caseful */
	NULL,                       /* 45 Possessified upto, caseful */

	/**** Single characters, caseless, must follow the caseful ones */

	OP_STARI_FUNC,              /* 46 */
	OP_STARI_FUNC,              /* 47 */
	OP_PLUSI_FUNC,              /* 48 */
	OP_PLUSI_FUNC,              /* 49 */
	OP_QUERYI_FUNC,             /* 50 */
	NULL,                       /* 51 */

	OP_UPTOI_FUNC,              /* 52 From 0 to n matches of one character, caseless */
	NULL,                       /* 53 */
	OP_EXACTI_FUNC,             /* 54 */

	OP_STARI_FUNC,              /* 55 Possessified star, caseless */
	NULL,                       /* 56 Possessified plus, caseless */
	NULL,                       /* 57 Posesssified query, caseless */
	NULL,                       /* 58 Possessified upto, caseless */

	/**** The negated ones must follow the non-negated ones, and match them ****/
	/**** Negated single character, caseful; must precede the caseless ones ****/

	OP_NOTSTAR_FUNC,            /* 59 The maximizing and minimizing versions of */
	OP_NOTSTAR_FUNC,            /* 60 these six opcodes must come in pairs, with */
	OP_NOTPLUS_FUNC,            /* 61 the minimizing one second. They must be in */
	OP_NOTPLUS_FUNC,            /* 62 exactly the same order as those above. */
	OP_NOTQUERY_FUNC,           /* 63 */
	NULL,                       /* 64 */

	OP_NOTUPTO_FUNC,        /* 65 From 0 to n matches, caseful */
	NULL,     /* 66 */
	OP_NOTEXACT_FUNC,       /* 67 Exactly n matches */

	OP_NOTSTAR_FUNC,     /* 68 Possessified versions, caseful */
	OP_NOTPLUS_FUNC,     /* 69 */
	OP_NOTQUERY_FUNC,    /* 70 */
	OP_NOTUPTO_FUNC,     /* 71 */

	/**** Negated single character, caseless; must follow the caseful ones ****/

	OP_NOTSTARI_FUNC,       /* 72 */
	OP_NOTSTARI_FUNC,       /* 73 */
	OP_NOTPLUSI_FUNC,       /* 74 */
	OP_NOTPLUSI_FUNC,       /* 75 */
	OP_NOTQUERYI_FUNC,      /* 76 */
	NULL,   /* 77 */

	OP_NOTUPTOI_FUNC,       /* 78 From 0 to n matches, caseless */
	OP_NOTUPTOI_FUNC,       /* 79 */
	OP_NOTEXACTI_FUNC,      /* 80 Exactly n matches */

	OP_NOTSTARI_FUNC,       /* 81 Possessified versions, caseless */
	OP_NOTPLUSI_FUNC,       /* 82 */
	OP_NOTQUERYI_FUNC,      /* 83 */
	OP_NOTUPTOI_FUNC,       /* 84 */

	/**** Character types ****/

	OP_TYPESTAR_FUNC,       /* 85 The maximizing and minimizing versions of */
	OP_TYPESTAR_FUNC,       /* 86 these six opcodes must come in pairs, with */
	OP_TYPEPLUS_FUNC,       /* 87 the minimizing one second. These codes must */
	OP_TYPEPLUS_FUNC,       /* 88 be in exactly the same order as those above. */
	OP_TYPEQUERY_FUNC,      /* 89 */
	NULL,   /* 90 */

	OP_TYPEUPTO_FUNC,       /* 91 From 0 to n matches */
	NULL,    /* 92 */
	OP_TYPEEXACT_FUNC,      /* 93 Exactly n matches */

	OP_TYPESTAR_FUNC,       /* 94 Possessified versions */
	OP_TYPEPLUS_FUNC,       /* 95 */
	NULL,                   /* 96 */
	NULL,                   /* 97 */

	/* These are used for character classes and back references; only the
	first six are the same as the sets above. */

	NULL,                   /* 98 The maximizing and minimizing versions of */
	NULL,                   /* 99 all these opcodes must come in pairs, with */
	NULL,                   /* 100 the minimizing one second. These codes must */
	NULL,                   /* 101 be in exactly the same order as those above. */
	NULL,                   /* 102 */
	NULL,                   /* 103 */

	NULL,                   /* 104 These are different to the three sets above. */
	NULL,                   /* 105 */

	/* End of quantifier opcodes */

	NULL,                   /* 106 Match a character class, chars < 256 only */
	NULL,                   /* 107 Same, but the bitmap was created from a negative
							class - the difference is relevant only when a
							character > 255 is encountered. */
	NULL,                   /* 108 Extended class for handling > 255 chars within the
							class. This does both positive and negative. */
	NULL,                   /* 109 Match a back reference, casefully */
	NULL,                   /* 110 Match a back reference, caselessly */
	NULL,                   /* 111 Match a numbered subpattern (possibly recursive) */
	NULL,                   /* 112 Call out to external function if provided */

	NULL,                   /* 113 Start of alternation */
	NULL,                   /* 114 End of group that doesn't have an unbounded repeat */
	NULL,                   /* 115 These two must remain together and in this */
	NULL,                   /* 116 order. They are for groups the repeat for ever. */
	NULL,                   /* 117 Possessive unlimited repeat. */

						    /* The assertions must come before BRA, CBRA, ONCE, and COND, and the four
							asserts must remain in order. */

	NULL,                   /* 118 Move pointer back - used in lookbehind assertions */
	NULL,                   /* 119 Positive lookahead */
	NULL,                   /* 120 Negative lookahead */
	NULL,                   /* 121 Positive lookbehind */
	NULL,                   /* 122 Negative lookbehind */

	/* ONCE, ONCE_NC, BRA, BRAPOS, CBRA, CBRAPOS, and COND must come immediately
	after the assertions, with ONCE first, as there's a test for >= ONCE for a
	subpattern that isn't an assertion. The POS versions must immediately follow
	the non-POS versions in each case. */

	NULL,                  /* 123 Atomic group, contains captures */
	NULL,                  /* 124 Atomic group containing no captures */
	NULL,                  /* 125 Start of non-capturing bracket */
	NULL,                  /* 126 Ditto, with unlimited, possessive repeat */
	NULL,                  /* 127 Start of capturing bracket */
	NULL,                  /* 128 Ditto, with unlimited, possessive repeat */
	NULL,                  /* 129 Conditional group */

	/* These five must follow the previous five, in the same order. There's a
	check for >= SBRA to distinguish the two sets. */

	NULL,                  /* 130 Start of non-capturing bracket, check empty  */
	NULL,                  /* 131 Ditto, with unlimited, possessive repeat */
	NULL,                  /* 132 Start of capturing bracket, check empty */
	NULL,                  /* 133 Ditto, with unlimited, possessive repeat */
	NULL,                  /* 134 Conditional group, check empty */

	/* The next two pairs must (respectively) be kept together. */

	NULL,                  /* 135 Used to hold a capture number as condition */
	NULL,                  /* 136 Same, but generated by a name reference*/
	NULL,                  /* 137 Used to hold a recursion number as condition */
	NULL,                  /* 138 Same, but generated by a name reference*/
	NULL,                  /* 139 The DEFINE condition */

	NULL,                  /* 140 These two must remain together and in this */
	NULL,                  /* 141 order. */
	NULL,                  /* 142 */

	/* These are backtracking control verbs */

	NULL,                  /* 143 always has an argument */
	NULL,                  /* 144 */
	NULL,                  /* 145 same, but with argument */
	NULL,                  /* 146 */
	NULL,                  /* 147 same, but with argument */
	NULL,                  /* 148 */
	NULL,                  /* 149 same, but with argument */
	NULL,                  /* 150 */

	/* These are forced failure and success verbs */

	NULL,                  /* 151 */
	NULL,                  /* 152 */
	NULL,                  /* 153 Used inside assertions */
	NULL,                  /* 154 Used before OP_ACCEPT to close open captures */

	/* This is used to skip a subpattern with a {0} quantifier */

	NULL                   /* 155 */

	/* This is not an opcode, but is used to check that tables indexed by opcode
	are the correct length, in order to catch updating errors - there have been
	some in the past. */
};

enum PCRESIGN
{
	ALT,
	BRA,
	CBRA,
	SCBRA,
	BRAZERO,
	ONCE,
};

//void GenerateNFA(std::vector<PCRE> &vecPcres)
//{
//	size_t count = 0;
//	size_t NotProcess = 0;
//	for (std::vector<PCRE>::iterator i = vecPcres.begin(); i != vecPcres.end(); ++i)
//	{
//		std::cout << ++count << std::endl;
//		CNfa nfa;
//		std::vector<unsigned char>::iterator Beg, End;
//		Beg = i->PcreCode.begin();
//		End = i->PcreCode.end();
//		if (!CanProcess(Beg, End))
//		{
//			++NotProcess;
//			continue;
//		}
//		Beg = i->PcreCode.begin();
//		End = i->PcreCode.end();
//		nfa.reserve(10000);
//		ProcessPcre(Beg, End, nfa);
//		//OutPut(i, nfa, count);
//		//OutPutForTest(i, nfa, count);
//	}
//	std::cout << "NotProcess number = " << NotProcess << std::endl;
//}

bool CanProcess(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End)
{
	std::vector<unsigned char>::iterator start, end;
	for (;Beg != End;)
	{
		if (FUNC[*Beg] != NULL)
		{
			Beg += Steps[*Beg];
		}
		else
		{
			switch (*Beg)
			{
			case OP_END:
			case OP_CIRC:
			case OP_CIRCM:
				Beg += Steps[*Beg];
				break;
			case OP_CLASS:
				Beg += Steps[OP_CLASS];
				NextForCLASS(Beg);
				break;
			case OP_NCLASS:
				Beg += Steps[OP_NCLASS];
				NextForCLASS(Beg);
				break;
			case OP_ALT:
				end = Beg + GET(Beg + 1);
				start = Beg + Steps[OP_ALT];
				if (!CanProcess(start, end))
				{
					return false;
				}
				Beg = end;
				break;
			case OP_KET:
				Beg += Steps[OP_KET];
				break;
			case OP_KETRMAX:
				Beg += Steps[OP_KETRMAX];
				break;
			case OP_ONCE:
				end = Beg + GET(Beg + 1);
				start = Beg + Steps[OP_ONCE];
				if (!CanProcess(start, end))
				{
					return false;
				}
				Beg = end;
				break;
			case OP_BRA:
				end = Beg + GET(Beg + 1);
				start = Beg + Steps[OP_BRA];
				if (*start == OP_CIRCM)
				{
					start += Steps[OP_CIRCM];
				}
				if (!CanProcess(start, end))
				{
					return false;
				}
				Beg = end;
				break;
			case OP_CBRA:
				end = Beg + GET(Beg + 1);
				start = Beg + Steps[OP_CBRA];
				if (!CanProcess(start, end))
				{
					return false;
				}
				Beg = end;
				break;
			case OP_SCBRA:
				end = Beg + GET(Beg + 1);
				start = Beg + Steps[OP_SCBRA];
				if (!CanProcess(start, end))
				{
					return false;
				}
				Beg = end;
				break;
			case OP_BRAZERO:
				end = Beg + 1 + GET(Beg + 2);
				start = Beg + Steps[OP_BRAZERO];
				if (!CanProcess(start, end))
				{
					return false;
				}
				Beg = end;
				break;
			default:
				return false;
			}
		}
	}
	return true;
}

void NextForCLASS(std::vector<unsigned char>::iterator &Beg)
{
	switch (*Beg)
	{
	case OP_CRSTAR:
	case OP_CRMINSTAR:
	case OP_CRPLUS:
	case OP_CRMINPLUS:
	case OP_CRQUERY:
	case OP_CRRANGE:
		Beg += Steps[*Beg];
		break;
	default:
		break;
	//case OP_CRSTAR:
	//	Beg += Steps[OP_CRSTAR];
	//	break;
	//case OP_CRMINSTAR:
	//	Beg += Steps[OP_CRMINSTAR];
	//	break;
	//case OP_CRPLUS:
	//	Beg += Steps[OP_CRPLUS];
	//	break;
	//case OP_CRMINPLUS:
	//	Beg += Steps[OP_CRMINPLUS];
	//	break;
	//case OP_CRQUERY:
	//	Beg += Steps[OP_CRQUERY];
	//	break;
	//case OP_CRRANGE:
	//	Beg += Steps[OP_CRRANGE];
	//	break;
	}
}

size_t ProcessPcre(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa)
{
	size_t CurState = nfa.Size();
	bool ALTBeg = false;
	std::vector<PCRESIGN> vecPath;
	std::vector<size_t> PreStates;
	size_t ALTPreBeg = CurState;
	return Process(Beg, End, nfa, CurState, PreStates, ALTPreBeg, ALTBeg, size_t(-1), false, false, false, vecPath);
}

size_t Process(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, std::vector<size_t> &PreStates, size_t ALTPreBeg, bool &ALTBeg, size_t ALTBeginState, bool bCBRA, bool bALT, bool bBRAZERO, std::vector<PCRESIGN> &vecPath)
{
	std::vector<unsigned char>::iterator start, end;
	size_t CurPreState;
	if (PreStates.size() > 0)
	{
		CurPreState = PreStates.back();
	}
	else
	{
		CurPreState = size_t(-1);
	}
	size_t ALTPreState = ALTPreBeg;
	bool IsCBRA = bCBRA;
	bool IsALT = bALT;
	bool IsBRAZERO = bBRAZERO;
	size_t ALTBegState = ALTBeginState;
	size_t flag = SC_SUCCESS;
	for (;Beg != End;)
	{
		if (FUNC[*Beg] != NULL)
		{
			flag = FUNC[*Beg](Beg, End, nfa, CurState, ALTPreState, ALTBeg, ALTBegState);
			Beg += Steps[*Beg];
		}
		else
		{
			switch (*Beg)
			{
			case OP_END:
				Beg += Steps[*Beg];
				break;
			case OP_CIRC:
				Beg += Steps[*Beg];
				break;
			case OP_CIRCM:
				Beg += Steps[*Beg];
				break;
			case OP_CLASS:
				flag = OP_CLASS_FUNC(Beg, End, nfa, CurState, ALTPreState, ALTBeg, ALTBegState);
				break;
			case OP_NCLASS:
				flag = OP_NCLASS_FUNC(Beg, End, nfa, CurState, ALTPreState, ALTBeg, ALTBegState);
				break;
			case OP_ALT:
				ALTBeg = true;
				ALTBegState = CurState;
				if (vecPath.back() != ALT)
				{
					vecPath.push_back(ALT);
				}
				end = Beg + GET(Beg + 1);
				start = Beg + Steps[OP_ALT];
				OP_ALT_FUNC(start, end, nfa, ALTPreState, CurState);
				flag = Process(start, end, nfa, CurState, PreStates, ALTPreState, ALTBeg, ALTBegState, IsCBRA, IsALT, IsBRAZERO, vecPath);
				Beg = end;
				break;
			case OP_KET:
				if (vecPath.size() == 0)
				{
					std::cout << "ERROR!" << std::endl;
					break;
				}
				if (vecPath.back() == CBRA)
				{
					vecPath.pop_back();
					PreStates.pop_back();
					if (vecPath.size() > 0 && vecPath.back() == BRAZERO)
					{
						vecPath.pop_back();
						CurPreState = PreStates.back();
						PreStates.pop_back();
						IsBRAZERO = true;
					}
					else
					{
						IsBRAZERO = false;
						IsCBRA = true;
					}
					IsALT = false;
				}
				else if (vecPath.back() == SCBRA)
				{
					vecPath.pop_back();
					PreStates.pop_back();
					if (vecPath.size() > 0 && vecPath.back() == BRAZERO)
					{
						vecPath.pop_back();
						CurPreState = PreStates.back();
						PreStates.pop_back();
						IsBRAZERO = true;
					}
					else
					{
						IsBRAZERO = false;
						IsCBRA = true;
					}
					IsALT = false;
				}
				else if (vecPath.back() == ALT)
				{
					ALTBeg = false;
					IsALT = true;
					IsBRAZERO = false;
					vecPath.pop_back();
					CurPreState = PreStates.back();
					if (vecPath.size() >= 2 && ((vecPath[vecPath.size() - 1] == CBRA && vecPath[vecPath.size() - 2] == BRAZERO) || 
						(vecPath[vecPath.size() - 1] == SCBRA && vecPath[vecPath.size() - 2] == BRAZERO)))
					{
						IsBRAZERO = true;
						CurPreState = PreStates.back();
						vecPath.pop_back();
						vecPath.pop_back();
						PreStates.pop_back();
						CurPreState = PreStates.back();
						PreStates.pop_back();
					}
					else if (vecPath.size() >= 1 && vecPath[vecPath.size() - 1] == CBRA)
					{
						CurPreState = PreStates.back();
						PreStates.pop_back();
						vecPath.pop_back();
					}
					else
					{

					}
				}
				else if (vecPath.back() == ONCE)
				{
					vecPath.pop_back();
					PreStates.pop_back();
					IsALT = false;
					IsBRAZERO = false;
				}
				else if (vecPath.back() == BRA)
				{
					vecPath.pop_back();
					PreStates.pop_back();
					IsALT = false;
					IsBRAZERO = false;
				}
				else
				{
					std::cout << "ERROR!" << std::endl;
					break;
				}
				OP_KET_FUNC(nfa, CurPreState, CurState, IsALT, IsBRAZERO);
				Beg += Steps[OP_KET];
				break;
			case OP_KETRMAX:
				if (vecPath.size() == 0)
				{
					std::cout << "ERROR!" << std::endl;
					break;
				}
				IsALT = false;
				IsBRAZERO = false;
				if (vecPath.back() == ALT)
				{
					IsALT = true;
					vecPath.pop_back();
					if (vecPath.size() >= 2 && (vecPath.back() == CBRA && vecPath[vecPath.size() - 2] == BRAZERO) || vecPath.back() == SCBRA && vecPath[vecPath.size() - 2] == BRAZERO)
					{
						vecPath.pop_back();
						vecPath.pop_back();
						PreStates.pop_back();
						PreStates.pop_back();
						IsBRAZERO = true;
					}
				}
				else if (vecPath.size() >= 2 && (vecPath.back() == CBRA && vecPath[vecPath.size() - 2] == BRAZERO) || vecPath.back() == SCBRA && vecPath[vecPath.size() - 2] == BRAZERO)
				{
					vecPath.pop_back();
					vecPath.pop_back();
					PreStates.pop_back();
					PreStates.pop_back();
					IsBRAZERO = true;
				}
				else if (vecPath.back() == CBRA)
				{
					vecPath.pop_back();
					PreStates.pop_back();
				}
				else
				{

				}
				OP_KETRMAX_FUNC(nfa, ALTPreState, CurState, IsBRAZERO, IsALT);
				Beg += Steps[OP_KETRMAX];
				break;
			case OP_ONCE:
				end = Beg + GET(Beg + 1);
				start = Beg + Steps[OP_ONCE];
				vecPath.push_back(ONCE);
				PreStates.push_back(CurState);
				AddEMPTY(nfa, CurState);
				flag = Process(start, end, nfa, CurState, PreStates, ALTPreState, ALTBeg, ALTBegState, IsCBRA, IsALT, IsBRAZERO, vecPath);
				Beg = end;
				break;
			case OP_BRA:
				vecPath.push_back(BRA);
				end = Beg + GET(Beg + 1);
				start = Beg + Steps[OP_BRA];
				if (*start == OP_CIRCM)
				{
					OP_CIRCM_FUNC(nfa, CurState);
					start += Steps[OP_CIRCM];
				}
				ALTPreState = CurState;
				PreStates.push_back(CurState);
				AddEMPTY(nfa, CurState);
				OP_BRA_FUNC(start, end, nfa, CurState);
				flag = Process(start, end, nfa, CurState, PreStates, ALTPreState, ALTBeg, ALTBegState, IsCBRA, IsALT, IsBRAZERO, vecPath);
				Beg = end;
				break;
			case OP_CBRA:
				vecPath.push_back(CBRA);
				end = Beg + GET(Beg + 1);
				start = Beg + Steps[OP_CBRA];
				if (!ALTBeg)
				{
					ALTPreState = CurState;
				}
				PreStates.push_back(CurState);
				AddEMPTY(nfa, CurState);
				OP_CBRA_FUNC(start, end, nfa, CurState);
				flag = Process(start, end, nfa, CurState, PreStates, ALTPreState, ALTBeg, ALTBegState, IsCBRA, IsALT, IsBRAZERO, vecPath);
				Beg = end;
				break;
			case OP_SCBRA:
				vecPath.push_back(SCBRA);
				end = Beg + GET(Beg + 1);
				start = Beg + Steps[OP_SCBRA];
				OP_SCBRA_FUNC(start, end, nfa, CurState);
				if (!ALTBeg)
				{
					ALTPreState = CurState;
				}
				PreStates.push_back(CurState);
				AddEMPTY(nfa, CurState);
				flag = Process(start, end, nfa, CurState, PreStates, ALTPreState, ALTBeg, ALTBegState, IsCBRA, IsALT, IsBRAZERO, vecPath);
				Beg = end;
				break;
			case OP_BRAZERO:
				vecPath.push_back(BRAZERO);
				end = Beg + 1 + GET(Beg + 2);
				start = Beg + Steps[OP_BRAZERO];
				if (!ALTBeg)
				{
					ALTPreState = CurState;
				}
				PreStates.push_back(CurState);
				AddEMPTY(nfa, CurState);
				flag = Process(start, end, nfa, CurState, PreStates, ALTPreState, ALTBeg, ALTBegState, IsCBRA, IsALT, IsBRAZERO, vecPath);
				Beg = end;
				break;
			}
		}
		if (flag != SC_SUCCESS)
		{
			return flag;
		}
	}

	return SC_SUCCESS;
}

void ProcessALT(CNfa &nfa, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	if (ALTBegin)
	{
		nfa[PreState].AddDest(EMPTY, ALTBegState);
		ALTBegin = false;
	}
}

size_t OP_COMMON_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	++CurState;
	size_t* tmp = ptr[*Beg];

	//std::cout << size_t(*Beg) << " " << NUMS[*Beg] << std::endl;
	for (size_t i = 0; i < NUMS[*Beg]; ++i)
	{
		nfa.Back().AddDest(tmp[i], CurState);
	}

	return SC_SUCCESS;
}

void OP_CIRCM_FUNC(CNfa &nfa, size_t &CurState)
{
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + 3);
	nfa[nCursize].AddDest(EMPTY, CurState + 1);
	nfa[nCursize].AddDest(EMPTY, CurState + 3);
	++CurState;

	++nCursize;
	for (size_t i = 0; i < 256; ++i)
	{
		nfa[nCursize].AddDest(i, CurState);
	}
	//for (size_t i = 0; i < 10; ++i)
	//{
	//	nfa[nCursize].AddDest(i, CurState);
	//}
	nfa[nCursize].AddDest('\n', CurState + 1);
	nfa[nCursize].AddDest('\n', CurState + 2);
	nfa[nCursize].AddDest('\r', CurState + 2);
	//nfa[nCursize].AddDest(11, CurState);
	//nfa[nCursize].AddDest(12, CurState);
	//for (size_t i = 14; i < 256; ++i)
	//{
	//	nfa[nCursize].AddDest(i, CurState);
	//}
	CurState += 2;
	nfa[nCursize + 1].AddDest('\r', CurState);

//	size_t nCursize = nfa.Size();
//	nfa.Resize(nCursize + 3);
//	nfa[nCursize].AddDest(EMPTY, CurState + 1);
//	nfa[nCursize].AddDest(EMPTY, CurState + 3);
//	++CurState;
//
//	++nCursize;
//	for (size_t i = 0; i < 10; ++i)
//	{
//		nfa[nCursize].AddDest(i, CurState);
//	}
//	nfa[nCursize].AddDest('\n', CurState + 1);
//	nfa[nCursize].AddDest('\n', CurState + 2);
//	nfa[nCursize].AddDest('\r', CurState + 2);
//	nfa[nCursize].AddDest(11, CurState);
//	nfa[nCursize].AddDest(12, CurState);
//	for (size_t i = 14; i < 256; ++i)
//	{
//		nfa[nCursize].AddDest(i, CurState);
//	}
//	CurState += 2;
//	nfa[nCursize + 1].AddDest('\r', CurState);
}


size_t OP_CHAR_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	++CurState;
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	nfa.Back().AddDest(*(Beg + 1), CurState);

	return SC_SUCCESS;
}

size_t OP_CHARI_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	++CurState;
	unsigned char c = *(Beg + 1);
	if (isalpha(c))
	{
		nfa.Back().AddDest(tolower(c), CurState);
		nfa.Back().AddDest(toupper(c), CurState);
	}
	else
	{
		nfa.Back().AddDest(c, CurState);
	}
	
	return SC_SUCCESS;
}

size_t OP_NOT_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	++CurState;
	unsigned char c = *(Beg + 1);
	for (size_t j = 0; j < 256; ++j)
	{
		if (j != c)
		{
			nfa.Back().AddDest(j, CurState);
		}
	}
	
	return SC_SUCCESS;
}

size_t OP_NOTI_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	++CurState;
	unsigned char c = *(Beg + 1);
	for (size_t j = 0; j < 256; ++j)
	{
		if (j != (size_t)tolower(c) && j != (size_t)toupper(c))
		{
			nfa.Back().AddDest(j, CurState);
		}
	}
	
	return SC_SUCCESS;
}

size_t OP_STAR_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	nfa.Back().AddDest(*(Beg + 1), CurState);
	++CurState;
	nfa.Back().AddDest(EMPTY, CurState);
	
	return SC_SUCCESS;
}

size_t OP_PLUS_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + 2);
	++CurState;
	nfa[nCursize].AddDest(*(Beg + 1), CurState);
	nfa.Back().AddDest(*(Beg + 1), CurState);
	++CurState;
	nfa.Back().AddDest(EMPTY, CurState);
	
	return SC_SUCCESS;
}

size_t OP_QUERY_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	++CurState;
	nfa.Back().AddDest(*(Beg + 1), CurState);
	nfa.Back().AddDest(EMPTY, CurState);
	
	return SC_SUCCESS;
}

size_t OP_UPTO_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);

	size_t count = GET(Beg + 1);
	if (count > SC_LIMIT)
	{
		return SC_EXCEED;
	}
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + count);
	unsigned char c = *(Beg + 3);
	for (size_t i = 0; i < count; ++i)
	{
		nfa[nCursize + i].AddDest(EMPTY, CurState - i + count);
		++CurState;
		nfa[nCursize + i].AddDest(c, CurState);
	}
	
	return SC_SUCCESS;
}

size_t OP_EXACT_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	size_t count = GET(Beg + 1);
	if (count > SC_LIMIT)
	{
		return SC_EXCEED;
	}
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + count);
	unsigned char c = *(Beg + 3);
	for (size_t i = 0; i < count; ++i)
	{
		++CurState;
		nfa[nCursize + i].AddDest(c, CurState);
	}
	
	return SC_SUCCESS;
}

size_t OP_STARI_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	unsigned char c = *(Beg + 1);
	if (isalpha(c))
	{
		nfa.Back().AddDest(tolower(c), CurState);
		nfa.Back().AddDest(toupper(c), CurState);
	}
	else
	{
		nfa.Back().AddDest(c, CurState);
	}
	++CurState;
	nfa.Back().AddDest(EMPTY, CurState);
	
	return SC_SUCCESS;
}

size_t OP_PLUSI_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + 2);
	unsigned char c = *(Beg + 1);
	++CurState;
	if (isalpha(c))
	{
		nfa[nCursize].AddDest(tolower(c), CurState);
		nfa[nCursize].AddDest(toupper(c), CurState);
	}
	else
	{
		nfa[nCursize].AddDest(c, CurState);
	}
	if (isalpha(c))
	{
		nfa.Back().AddDest(tolower(c), CurState);
		nfa.Back().AddDest(toupper(c), CurState);
	}
	else
	{
		nfa.Back().AddDest(c, CurState);
	}
	++CurState;
	nfa.Back().AddDest(EMPTY, CurState);
	
	return SC_SUCCESS;
}

size_t OP_QUERYI_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	unsigned char c = *(Beg + 1);
	++CurState;
	if (isalpha(c))
	{
		nfa.Back().AddDest(tolower(c), CurState);
		nfa.Back().AddDest(toupper(c), CurState);
	}
	else
	{
		nfa.Back().AddDest(c, CurState);
	}
	nfa.Back().AddDest(EMPTY, CurState);
	
	return SC_SUCCESS;
}

size_t OP_UPTOI_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);

	size_t count = GET(Beg + 1);
	if (count > SC_LIMIT)
	{
		return SC_EXCEED;
	}
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + count);
	unsigned char c = *(Beg + 3);
	for (size_t i = 0; i < count; ++i)
	{
		nfa[nCursize + i].AddDest(EMPTY, CurState - i + count);
		++CurState;
		if (isalpha(c))
		{
			nfa[nCursize + i].AddDest(tolower(c), CurState);
			nfa[nCursize + i].AddDest(toupper(c), CurState);
		}
		else
		{
			nfa[nCursize + i].AddDest(c, CurState);
		}
	}
	
	return SC_SUCCESS;
}

size_t OP_EXACTI_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	size_t count = GET(Beg + 1);
	if (count > SC_LIMIT)
	{
		return SC_EXCEED;
	}
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + count);
	unsigned char c = *(Beg + 3);
	for (size_t i = 0; i < count; ++i)
	{
		++CurState;
		if (isalpha(c))
		{
			nfa[nCursize + i].AddDest(tolower(c), CurState);
			nfa[nCursize + i].AddDest(toupper(c), CurState);
		}
		else
		{
			nfa[nCursize + i].AddDest(c, CurState);
		}
	}
	
	return SC_SUCCESS;
}

size_t OP_NOTSTAR_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	unsigned char c = *(Beg + 1);
	for (size_t i = 0; i < 256; ++i)
	{
		if (i != c)
		{
			nfa.Back().AddDest(i, CurState);
		}
	}
	++CurState;
	nfa.Back().AddDest(EMPTY, CurState);
	
	return SC_SUCCESS;
}

size_t OP_NOTPLUS_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + 2);
	unsigned char c = *(Beg + 1);
	++CurState;
	for (size_t i = 0; i < 256; ++i)
	{
		if (i != c)
		{
			nfa[nCursize].AddDest(i, CurState);
			nfa.Back().AddDest(i, CurState);
		}
	}
	++CurState;
	nfa.Back().AddDest(EMPTY, CurState);
	
	return SC_SUCCESS;
}

size_t OP_NOTQUERY_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	unsigned char c = *(Beg + 1);
	++CurState;
	for (size_t i = 0; i < 256; ++i)
	{
		if (i != c)
		{
			nfa.Back().AddDest(i, CurState);
		}
	}
	nfa.Back().AddDest(EMPTY, CurState);
	
	return SC_SUCCESS;
}

size_t OP_NOTUPTO_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	size_t count = GET(Beg + 1);
	if (count > SC_LIMIT)
	{
		return SC_EXCEED;
	}
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + count);
	unsigned char c = *(Beg + 3);
	for (size_t i = 0; i < count; ++i)
	{
		nfa[nCursize + i].AddDest(EMPTY, CurState - i + count);
		++CurState;
		for (size_t j = 0; j < 256; ++j)
		{
			if (j != c)
			{
				nfa[nCursize + i].AddDest(j, CurState);
			}
		}
	}
	
	return SC_SUCCESS;
}

size_t OP_NOTEXACT_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	size_t count = GET(Beg + 1);
	if (count > SC_LIMIT)
	{
		return SC_EXCEED;
	}
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + count);
	unsigned char c = *(Beg + 3);
	for (size_t i = 0; i < count; ++i)
	{
		++CurState;
		for (size_t j = 0; j < 256; ++j)
		{
			if (j != c)
			{
				nfa[nCursize + i].AddDest(j, CurState);
			}
		}
	}
	
	return SC_SUCCESS;
}

size_t OP_NOTSTARI_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	unsigned char c = *(Beg + 1);
	for (size_t i = 0; i < 256; ++i)
	{
		if (i != (size_t)tolower(c) && i != (size_t)toupper(c))
		{
			nfa.Back().AddDest(i, CurState);
		}
	}
	++CurState;
	nfa.Back().AddDest(EMPTY, CurState);
	
	return SC_SUCCESS;
}

size_t OP_NOTPLUSI_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + 2);
	++CurState;
	unsigned char c = *(Beg + 1);
	for (size_t i = 0; i < 256; ++i)
	{
		if (i != (size_t)tolower(c) && i != (size_t)toupper(c))
		{
			nfa[nCursize].AddDest(i, CurState);
			nfa.Back().AddDest(i, CurState);
		}
	}
	++CurState;
	nfa.Back().AddDest(EMPTY, CurState);
	
	return SC_SUCCESS;
}

size_t OP_NOTQUERYI_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	++CurState;
	unsigned char c = *(Beg + 1);
	for (size_t i = 0; i < 256; ++i)
	{
		if (i != (size_t)tolower(c) && i != (size_t)toupper(c))
		{
			nfa.Back().AddDest(i, CurState);
		}
	}
	nfa.Back().AddDest(EMPTY, CurState);
	
	return SC_SUCCESS;
}

size_t OP_NOTUPTOI_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	size_t count = GET(Beg + 1);
	if (count > SC_LIMIT)
	{
		return SC_EXCEED;
	}
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + count);
	unsigned char c = *(Beg + 3);
	for (size_t i = 0; i < count; ++i)
	{
		nfa[nCursize + i].AddDest(EMPTY, CurState - i + count);
		++CurState;
		for (size_t j = 0; j < 256; ++j)
		{
			if (j != (size_t)tolower(c) && j != (size_t)toupper(c))
			{
				nfa[nCursize + i].AddDest(j, CurState);
			}
		}
	}
	
	return SC_SUCCESS;
}

size_t OP_NOTEXACTI_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	size_t count = GET(Beg + 1);
	if (count > SC_LIMIT)
	{
		return SC_EXCEED;
	}
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + count);
	unsigned char c = *(Beg + 3);
	for (size_t i = 0; i < count; ++i)
	{
		++CurState;
		for (size_t j = 0; j < 256; ++j)
		{
			if (j != (size_t)tolower(c) && j != (size_t)toupper(c))
			{
				nfa[nCursize + i].AddDest(j, CurState);
			}
		}
	}
	
	return SC_SUCCESS;
}

size_t OP_TYPESTAR_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	unsigned char c = *(Beg + 1);
	if (c < sizeof(NUMS) / sizeof(size_t) && ptr[c] != NULL)
	{
		size_t nCursize = nfa.Size();
		nfa.Resize(nCursize + 1);
		size_t Cnt = NUMS[c];
		size_t* tmp = ptr[c];
		for (size_t i = 0; i < Cnt; ++i)
		{
			nfa.Back().AddDest(*(tmp + i), CurState);
		}
		++CurState;
		nfa.Back().AddDest(EMPTY, CurState);
	}
	
	return SC_SUCCESS;
}

size_t OP_TYPEPLUS_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	unsigned char c = *(Beg + 1);
	if (c < sizeof(NUMS) / sizeof(size_t) && ptr[c] != NULL)
	{
		size_t nCursize = nfa.Size();
		nfa.Resize(nCursize + 2);
		++CurState;
		size_t Cnt = NUMS[c];
		size_t* tmp = ptr[c];
		for (size_t i = 0; i < Cnt; ++i)
		{
			nfa[nCursize].AddDest(*(tmp + i), CurState);
			nfa.Back().AddDest(*(tmp + i), CurState);
		}
		++CurState;
		nfa.Back().AddDest(EMPTY, CurState);
	}
	
	return SC_SUCCESS;
}

size_t OP_TYPEQUERY_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	unsigned char c = *(Beg + 1);
	if (c < sizeof(NUMS) / sizeof(size_t) && ptr[c] != NULL)
	{
		size_t nCursize = nfa.Size();
		nfa.Resize(nCursize + 1);
		++CurState;
		size_t Cnt = NUMS[c];
		size_t* tmp = ptr[c];
		for (size_t i = 0; i < Cnt; ++i)
		{
			nfa.Back().AddDest(*(tmp + i), CurState);
		}
		nfa.Back().AddDest(EMPTY, CurState);
	}
	
	return SC_SUCCESS;
}

size_t OP_TYPEUPTO_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	size_t count = GET(Beg + 1);
	if (count > SC_LIMIT)
	{
		return SC_EXCEED;
	}
	bool ALTBeg = ALTBegin;
	unsigned char c = *(Beg + 3);
	if (c >=OP_NOT_DIGIT && c <= OP_ALLANY  && FUNC[c] != NULL)
	{
		std::vector<unsigned char>::iterator tmpBeg = Beg + 3;
		for (size_t i = 0; i < count; ++i)
		{
			FUNC[c](tmpBeg, End, nfa, CurState, PreState, ALTBeg, ALTBegState);
			nfa.Back().AddDest(EMPTY, CurState - 1 + count - i);
		}
	}
	
	return SC_SUCCESS;
}

size_t OP_TYPEEXACT_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	size_t count = GET(Beg + 1);
	if (count > SC_LIMIT)
	{
		return SC_EXCEED;
	}
	bool ALTBeg = ALTBegin;
	unsigned char c = *(Beg + 3);
	if (c >=OP_NOT_DIGIT && c <= OP_ALLANY  && FUNC[c] != NULL)
	{
		std::vector<unsigned char>::iterator tmpBeg = Beg + 3;

		for (size_t i = 0; i < count; ++i)
		{
			FUNC[c](tmpBeg, End, nfa, CurState, PreState, ALTBeg, ALTBegState);
		}
	}
	
	return SC_SUCCESS;
}

size_t OP_CLASS_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	CNfaRow Row;
	size_t tmp = CurState + 1;
	for (size_t i = 0; i < 32; ++i)
	{
		for (size_t j = 0; j < 8; ++j)
		{
			if (*(Beg + i + 1) & 1 << j)
			{
				nfa.Back().AddDest(i * 8 + j, tmp);
				Row.AddDest(i * 8 + j, tmp);
			}
		}
	}
	Beg += Steps[OP_CLASS];
	size_t min, max;

	switch (*Beg)
	{
	case OP_CRSTAR:
		for (size_t i = 0; i < CHARSETSIZE; ++i)
		{
			for (size_t j = 0; j < nfa.Back().DestCnt(i); ++j)
			{
				if (nfa.Back().GetDest(i, j) == CurState + 1)
				{
					nfa.Back().GetDest(i, j) = CurState;
				}
			}
		}
		++CurState;
		nfa.Back().AddDest(EMPTY, CurState);
		Beg += Steps[OP_CRSTAR];
		break;
	case OP_CRMINSTAR:
		for (size_t i = 0; i < CHARSETSIZE; ++i)
		{
			for (size_t j = 0; j < nfa.Back().DestCnt(i); ++j)
			{
				if (nfa.Back().GetDest(i, j) == CurState + 1)
				{
					nfa.Back().GetDest(i, j) = CurState;
				}
			}
		}
		++CurState;
		nfa.Back().AddDest(EMPTY, CurState);
		Beg += Steps[OP_CRMINSTAR];
		break;
	case OP_CRPLUS:
		nfa.Resize(nCursize + 2);
		Copy(nfa.Back(), Row, 0);
		++CurState;
		++CurState;
		nfa.Back().AddDest(EMPTY, CurState);
		Beg += Steps[OP_CRPLUS];
		break;
	case OP_CRMINPLUS:
		nfa.Resize(nCursize + 2);
		Copy(nfa.Back(), Row, 0);
		++CurState;
		++CurState;
		nfa.Back().AddDest(EMPTY, CurState);
		Beg += Steps[OP_CRMINPLUS];
		break;
	case OP_CRQUERY:
		++CurState;
		nfa.Back().AddDest(EMPTY, CurState);
		Beg += Steps[OP_CRQUERY];
		break;
	case OP_CRRANGE:
		min = GET(Beg + 1);
		max = GET(Beg + 3);
		if (min > SC_LIMIT || max > SC_LIMIT)
		{
			return SC_EXCEED;
		}
		nfa.PopBack();
		nfa.Resize(nCursize + min);
		for (size_t i = 0; i < min; ++i)
		{
			Copy(nfa[nCursize + i], Row, i);
			++CurState;
		}
		if (max == 0)
		{
			nfa.Resize(nCursize + min + 1);
			for (size_t i = 0; i < CHARSETSIZE; ++i)
			{
				if (Row.DestCnt(i) == 1)
				{
					nfa.Back().AddDest(i, CurState);
				}
			}
			++CurState;
			nfa.Back().AddDest(EMPTY, CurState);
		}
		else
		{
			nfa.Resize(nCursize + max);
			size_t nDiff = max - min, nBeg = nCursize + min;
			for (size_t i = 0; i < nDiff; ++i)
			{
				Copy(nfa[nBeg + i], Row, min + i);
				nfa[nBeg + i].AddDest(EMPTY, CurState - i + nDiff);
				++CurState;
			}
		}
		Beg += Steps[OP_CRRANGE];
		break;
	default:
		++CurState;
		break;
	}
	
	return SC_SUCCESS;
}

size_t OP_NCLASS_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState, size_t PreState, bool &ALTBegin, size_t ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	CNfaRow Row;
	size_t tmp = CurState + 1;
	for (size_t i = 0; i < 32; ++i)
	{
		for (size_t j = 0; j < 8; ++j)
		{
			if (*(Beg + i + 1) & 1 << j)
			{
				size_t nCol = i * 8 + j;
				nfa.Back().AddDest(nCol, tmp);
				Row.AddDest(nCol, tmp);
			}
		}
	}
	Beg += Steps[OP_CLASS];
	size_t min, max;
	switch (*Beg)
	{
	case OP_CRSTAR:
		for (size_t i = 0; i < CHARSETSIZE; ++i)
		{
			size_t nCnt = nfa.Back().DestCnt(i);
			for (size_t j = 0; j < nCnt; ++j)
			{
				size_t &nSta = nfa.Back().GetDest(i, j);
				if (nSta == CurState + 1)
				{
					nSta = CurState;
				}
			}
		}
		++CurState;
		nfa.Back().AddDest(EMPTY, CurState);
		Beg += Steps[OP_CRSTAR];
		break;
	case OP_CRMINSTAR:
		for (size_t i = 0; i < CHARSETSIZE; ++i)
		{
			size_t nCnt = nfa.Back().DestCnt(i);
			for (size_t j = 0; j < nCnt; ++j)
			{
				size_t &nSta = nfa.Back().GetDest(i, j);
				if (nSta == CurState + 1)
				{
					nSta = CurState;
				}
			}
		}
		++CurState;
		nfa.Back().AddDest(EMPTY, CurState);
		Beg += Steps[OP_CRMINSTAR];
		break;
	case OP_CRPLUS:
		nfa.Resize(nCursize + 2);
		Copy(nfa.Back(), Row, 0);
		++CurState;
		++CurState;
		nfa.Back().AddDest(EMPTY, CurState);
		Beg += Steps[OP_CRPLUS];
		break;
	case OP_CRMINPLUS:
		nfa.Resize(nCursize + 2);
		Copy(nfa.Back(), Row, 0);
		++CurState;
		++CurState;
		nfa.Back().AddDest(EMPTY, CurState);
		Beg += Steps[OP_CRMINPLUS];
		break;
	case OP_CRQUERY:
		++CurState;
		nfa.Back().AddDest(EMPTY, CurState);
		Beg += Steps[OP_CRQUERY];
		break;
	case OP_CRRANGE:
		min = GET(Beg + 1);
		max = GET(Beg + 3);
		if (min > SC_LIMIT || max > SC_LIMIT)
		{
			return SC_EXCEED;
		}
		nfa.PopBack();
		nfa.Resize(nCursize + min);
		for (size_t i = 0; i < min; ++i)
		{
			Copy(nfa[nCursize + i], Row, i);
			++CurState;
		}
		if (max == 0)
		{
			nfa.Resize(nCursize + min + 1);
			for (size_t i = 0; i < CHARSETSIZE; ++i)
			{
				if (Row.DestCnt(i) == 1)
				{
					nfa.Back().AddDest(i, CurState);
				}
			}
			++CurState;
			nfa.Back().AddDest(EMPTY, CurState);
		}
		else
		{
			nfa.Resize(nCursize + max);
			for (size_t i = 0; i < max - min; ++i)
			{
				size_t nCnt = min + i;
				Copy(nfa[nCursize + nCnt], Row, nCnt);
				nfa[nCursize + nCnt].AddDest(EMPTY, CurState + max - nCnt);
				++CurState;
			}
		}
		Beg += Steps[OP_CRRANGE];
		break;
	default:
		++CurState;
		break;
	}
	
	return SC_SUCCESS;
}

void OP_ALT_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t PreState, size_t &CurState)
{
	for (size_t i = PreState; i < nfa.Size(); ++i)
	{
		CNfaRow &row = nfa[i];
		for (size_t j = 0; j < CHARSETSIZE; ++j)
		{
			size_t nCnt = row.DestCnt(j);
			for (size_t k = 0; k < nCnt; ++k)
			{
				size_t &nSta = row.GetDest(j, k);
				if (nSta == CurState)
				{
					nSta = MAX;
				}
			}
		}
	}
	if (Beg == End)
	{
		nfa[PreState].AddDest(EMPTY, MAX);
	}
}

void OP_KET_FUNC(CNfa &nfa, size_t PreState, size_t &CurState, bool IsALT, bool IsBRAZERO)
{
	if (IsBRAZERO)
	{
		nfa[PreState].AddDest(EMPTY, CurState);
	}
	if (IsALT)
	{
		for (size_t i = PreState; i < nfa.Size(); ++i)
		{
			CNfaRow &row = nfa[i];
			for (size_t j = 0; j < CHARSETSIZE; ++j)
			{
				size_t nCnt = row.DestCnt(j);
				for (size_t k = 0; k < nCnt; ++k)
				{
					size_t &nSta = row.GetDest(j, k);
					if (nSta == MAX)
					{
						nSta = CurState;
					}
				}
			}
		}
	}
}

void OP_KETRMAX_FUNC(CNfa &nfa, size_t PreState, size_t &CurState, bool IsBRAZERO, bool IsALT)
{
	if (IsALT)
	{
		for (size_t i = PreState; i < nfa.Size(); ++i)
		{
			CNfaRow &row = nfa[i];
			for (size_t j = 0; j < CHARSETSIZE; ++j)
			{
				size_t nCnt = row.DestCnt(j);
				for (size_t k = 0; k < nCnt; ++k)
				{
					size_t &nSta = row.GetDest(j, k);
					if (nSta == MAX)
					{
						nSta = CurState;
					}
				}
			}
		}
	}
	if (IsBRAZERO)
	{
		for (size_t i = PreState; i < nfa.Size(); ++i)
		{
			CNfaRow &row = nfa[i];
			for (size_t j = 0; j < CHARSETSIZE; ++j)
			{
				size_t nCnt = row.DestCnt(j);
				for (size_t k = 0; k < nCnt; ++k)
				{
					size_t &nSta = row.GetDest(j, k);
					if (nSta == CurState)
					{
						nSta = PreState;
					}
				}
			}
			
		}
		nfa[PreState].AddDest(EMPTY, CurState);
	}
	else
	{
		size_t nCursize = nfa.Size();
		nfa.Resize(nCursize + nCursize - PreState);
		size_t StartState = CurState;
		for (size_t i = PreState; i < nCursize; ++i)
		{
			CNfaRow &row = nfa[i];
			for (size_t j = 0; j < CHARSETSIZE; ++j)
			{
				size_t nCnt = row.DestCnt(j);
				for (size_t k = 0; k < nCnt; ++k)
				{
					nfa[nCursize + i - PreState].AddDest(j, CurState + row.GetDest(j, k) - i);
				}
			}
			++CurState;
		}
		for (size_t i = StartState; i < nfa.Size(); ++i)
		{
			CNfaRow &row = nfa[i];
			for (size_t j = 0; j < CHARSETSIZE; ++j)
			{
				size_t nCnt = row.DestCnt(j);
				for (size_t k = 0; k < nCnt; ++k)
				{
					size_t &nSta = row.GetDest(j, k);
					if (nSta == CurState)
					{
						nSta = StartState;
					}
				}
			}
		}
		nfa[StartState].AddDest(EMPTY, CurState);
	}
}

size_t OP_BRA_CBRA_SCBRA_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState)
{
	if (Beg == End)
	{
		size_t nCursize = nfa.Size();
		nfa.Resize(nCursize + 1);
		nfa.Back().AddDest(EMPTY, MAX);
		++CurState;
	}
	
	return SC_SUCCESS;
}

size_t OP_BRA_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState)
{
	return OP_BRA_CBRA_SCBRA_FUNC(Beg, End, nfa, CurState);
}

size_t OP_CBRA_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState)
{
	return OP_BRA_CBRA_SCBRA_FUNC(Beg, End, nfa, CurState);
}

size_t OP_SCBRA_FUNC(std::vector<unsigned char>::iterator &Beg, const std::vector<unsigned char>::iterator &End, CNfa &nfa, size_t &CurState)
{
	return OP_BRA_CBRA_SCBRA_FUNC(Beg, End, nfa, CurState);
}

void Copy(CNfaRow &NewRow, CNfaRow &Row, size_t increment)
{
	for (size_t i = 0; i < CHARSETSIZE; ++i)
	{
		size_t nDestCnt = Row.DestCnt(i);
		for (size_t j = 0; j < nDestCnt; ++j)
		{
			NewRow.AddDest(i, Row.GetDest(i, j) + increment);
		}
	}
}

void AddEMPTY(CNfa &nfa, size_t &CurState)
{
	size_t nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	++CurState;
	nfa.Back().AddDest(EMPTY, CurState);
}

void OutPut(std::vector<PCRE>::iterator &Pcre, CNfa &nfa, size_t count)
{
	std::stringstream ss;
	std::string filename;
	filename = "D:\\NFAResults\\";
	ss << count;
	filename += ss.str();
	filename += ".txt";
	std::ofstream fout(filename.c_str(), std::ios::binary);

	size_t Cnt;
	unsigned char c;
	Cnt = Pcre->PcreStr.length();
	fout.write((char*)&Cnt, 4);
	for (size_t i = 0; i < Cnt; ++i)
	{
		c = Pcre->PcreStr[i];
		fout.write((char*)&c, 1);
	}
	Cnt = nfa.Size();
	fout.write((char*)&Cnt, 4);
	size_t num;
	for (size_t i = 0; i < Cnt; ++i)
	{
		CNfaRow &row = nfa[i];
		for (size_t j = 0; j < CHARSETSIZE; ++j)
		{
			num = row.DestCnt(j);
			fout.write((char*)&num, 4);
			for (size_t k = 0; k < num; ++k)
			{
				fout.write((char*)&(row.GetDest(j, k)), 4);
			}
		}
	}
	fout.close();
	fout.clear();
}

#pragma warning (pop)
