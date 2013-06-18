#include "stdafx.h"
#include "p2nmain.h"

#pragma warning (push)
#pragma warning (disable : 4100)
const byte Steps[] = { OP_LENGTHS };

ulong OP_NOT_DIGIT_ELEMS[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
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

ulong OP_DIGIT_ELEMS[] = {48, 49, 50, 51, 52, 53, 54, 55, 56, 57};

ulong OP_NOT_WHITESPACE_ELEMS[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 11, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 
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

ulong OP_WHITESPACE_ELEMS[] = {9, 10, 12, 13, 32};

ulong OP_NOT_WORDCHAR_ELEMS[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 
	24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 58, 59, 60, 61, 62,
	63, 64, 91, 92, 93, 94, 96, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140,
	141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164,
	165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188,
	189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 
	213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 
	237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255};

ulong OP_WORDCHAR_ELEMS[] = {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 
	79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 95, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 
	112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122};

ulong OP_ANY_ELEMS[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
	31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,
	62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92,
	93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118,
	119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
	144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168,
	169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193,
	194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218,
	219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243,
	244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255};

ulong OP_ALLANY_ELEMS[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 
	28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
	60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 
	91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117,
	118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 
	143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 
	168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 
	193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 
	218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 
	243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255};

ulong* ptr[] = {
	null,
	null,
	null,
	null,
	null,
	null,
	OP_NOT_DIGIT_ELEMS,
	OP_DIGIT_ELEMS,
	OP_NOT_WHITESPACE_ELEMS,
	OP_WHITESPACE_ELEMS,
	OP_NOT_WORDCHAR_ELEMS,
	OP_WORDCHAR_ELEMS,
	OP_ANY_ELEMS,
	OP_ALLANY_ELEMS
};

ulong NUMS[] = {
	0,
	0,
	0,
	0,
	0,
	0,
	sizeof(OP_NOT_DIGIT_ELEMS)		/ sizeof(ulong),
	sizeof(OP_DIGIT_ELEMS)			/ sizeof(ulong),
	sizeof(OP_NOT_WHITESPACE_ELEMS)	/ sizeof(ulong),
	sizeof(OP_WHITESPACE_ELEMS)		/ sizeof(ulong),
	sizeof(OP_NOT_WORDCHAR_ELEMS)	/ sizeof(ulong),
	sizeof(OP_WORDCHAR_ELEMS)		/ sizeof(ulong),
	sizeof(OP_ANY_ELEMS)			/ sizeof(ulong),
	sizeof(OP_ALLANY_ELEMS)			/ sizeof(ulong),
};

Fn FUNC[156] = 
{
	null,						/* 0 End of pattern */

	/* Values corresponding to backslashed metacharacters */

	null,						/* 1 Start of data: \A */
	null,						/* 2 Start of match (subject + offset): \G */
	null,						/* 3 Set start of match (\K) */
	null,						/* 4 \B */
	null,						/* 5 \b */
	OP_COMMON_FUNC,				/* 6 \D */
	OP_COMMON_FUNC,				/* 7 \d */
	OP_COMMON_FUNC,				/* 8 \S */
	OP_COMMON_FUNC,				/* 9 \s */
	OP_COMMON_FUNC,				/* 10 \W */
	OP_COMMON_FUNC,				/* 11 \w */

	OP_COMMON_FUNC,				/* 12 Match any character except newline (\N) */
	OP_COMMON_FUNC,				/* 13 Match any character */
	null,						/* 14 Match any byte (\C); different to OP_ANY for UTF-8 */
	null,						/* 15 \P (not Unicode property) */
	null,						/* 16 \p (Unicode property) */
	null,						/* 17 \R (any newline sequence) */
	null,						/* 18 \H (not horizontal whitespace) */
	null,						/* 19 \h (horizontal whitespace) */
	null,						/* 20 \V (not vertical whitespace) */
	null,						/* 21 \v (vertical whitespace) */
	null,						/* 22 \X (extended Unicode sequence */
	null,						/* 23 End of data or \n at end of data (\Z) */
	null,						/* 24 End of data (\z) */

	null,						/* 25 Start of line - not multiline */
	null,						/* 26 Start of line - multiline */
	null,						/* 27 End of line - not multiline */
	null,						/* 28 End of line - multiline */
	OP_CHAR_FUNC,				/* 29 Match one character, casefully */
	OP_CHARI_FUNC,				/* 30 Match one character, caselessly */
	OP_NOT_FUNC,				/* 31 Match one character, not the given one, casefully */
	OP_NOTI_FUNC,				/* 32 Match one character, not the given one, caselessly */

	/* The following sets of 13 opcodes must always be kept in step because
	the offset from the first one is used to generate the others. */

	/**** Single characters, caseful, must precede the caseless ones ****/

	OP_STAR_FUNC,				/* 33 The maximizing and minimizing versions of */
	OP_STAR_FUNC,				/* 34 these six opcodes must come in pairs, with */
	OP_PLUS_FUNC,				/* 35 the minimizing one second. */
	OP_PLUS_FUNC,				/* 36 */
	OP_QUERY_FUNC,				/* 37 */
	OP_QUERY_FUNC,				/* 38 */

	OP_UPTO_FUNC,				/* 39 From 0 to n matches of one character, caseful*/
	OP_UPTO_FUNC,				/* 40 */
	OP_EXACT_FUNC,				/* 41 Exactly n matches */

	OP_STAR_FUNC,				/* 42 Possessified star, caseful */
	OP_PLUS_FUNC,				/* 43 Possessified plus, caseful */
	OP_QUERY_FUNC,				/* 44 Posesssified query, caseful */
	OP_UPTO_FUNC,				/* 45 Possessified upto, caseful */

	/**** Single characters, caseless, must follow the caseful ones */

	OP_STARI_FUNC,				/* 46 */
	OP_STARI_FUNC,				/* 47 */
	OP_PLUSI_FUNC,				/* 48 */
	OP_PLUSI_FUNC,				/* 49 */
	OP_QUERYI_FUNC,				/* 50 */
	OP_QUERYI_FUNC,				/* 51 */

	OP_UPTOI_FUNC,				/* 52 From 0 to n matches of one character, caseless */
	OP_UPTOI_FUNC,				/* 53 */
	OP_EXACTI_FUNC,				/* 54 */

	OP_STARI_FUNC,				/* 55 Possessified star, caseless */
	OP_PLUSI_FUNC,				/* 56 Possessified plus, caseless */
	OP_QUERYI_FUNC,				/* 57 Posesssified query, caseless */
	OP_UPTOI_FUNC,				/* 58 Possessified upto, caseless */

	/**** The negated ones must follow the non-negated ones, and match them ****/
	/**** Negated single character, caseful; must precede the caseless ones ****/

	OP_NOTSTAR_FUNC,			/* 59 The maximizing and minimizing versions of */
	OP_NOTSTAR_FUNC,			/* 60 these six opcodes must come in pairs, with */
	OP_NOTPLUS_FUNC,			/* 61 the minimizing one second. They must be in */
	OP_NOTPLUS_FUNC,			/* 62 exactly the same order as those above. */
	OP_NOTQUERY_FUNC,			/* 63 */
	OP_NOTQUERY_FUNC,			/* 64 */

	OP_NOTUPTO_FUNC,			/* 65 From 0 to n matches, caseful */
	OP_NOTUPTO_FUNC,			/* 66 */
	OP_NOTEXACT_FUNC,			/* 67 Exactly n matches */

	OP_NOTSTAR_FUNC,			/* 68 Possessified versions, caseful */
	OP_NOTPLUS_FUNC,			/* 69 */
	OP_NOTQUERY_FUNC,			/* 70 */
	OP_NOTUPTO_FUNC,			/* 71 */

	/**** Negated single character, caseless; must follow the caseful ones ****/

	OP_NOTSTARI_FUNC,			/* 72 */
	OP_NOTSTARI_FUNC,			/* 73 */
	OP_NOTPLUSI_FUNC,			/* 74 */
	OP_NOTPLUSI_FUNC,			/* 75 */
	OP_NOTQUERYI_FUNC,			/* 76 */
	OP_NOTQUERYI_FUNC,			/* 77 */

	OP_NOTUPTOI_FUNC,			/* 78 From 0 to n matches, caseless */
	OP_NOTUPTOI_FUNC,			/* 79 */
	OP_NOTEXACTI_FUNC,			/* 80 Exactly n matches */

	OP_NOTSTARI_FUNC,			/* 81 Possessified versions, caseless */
	OP_NOTPLUSI_FUNC,			/* 82 */
	OP_NOTQUERYI_FUNC,			/* 83 */
	OP_NOTUPTOI_FUNC,			/* 84 */

	/**** Character types ****/

	OP_TYPESTAR_FUNC,			/* 85 The maximizing and minimizing versions of */
	OP_TYPESTAR_FUNC,			/* 86 these six opcodes must come in pairs, with */
	OP_TYPEPLUS_FUNC,			/* 87 the minimizing one second. These codes must */
	OP_TYPEPLUS_FUNC,			/* 88 be in exactly the same order as those above. */
	OP_TYPEQUERY_FUNC,			/* 89 */
	OP_TYPEQUERY_FUNC,			/* 90 */

	OP_TYPEUPTO_FUNC,			/* 91 From 0 to n matches */
	OP_TYPEUPTO_FUNC,			/* 92 */
	OP_TYPEEXACT_FUNC,			/* 93 Exactly n matches */

	OP_TYPESTAR_FUNC,			/* 94 Possessified versions */
	OP_TYPEPLUS_FUNC,			/* 95 */
	OP_TYPEQUERY_FUNC,			/* 96 */
	OP_TYPEUPTO_FUNC,			/* 97 */

	/* These are used for character classes and back references; only the
	first six are the same as the sets above. */

	null,						/* 98 The maximizing and minimizing versions of */
	null,						/* 99 all these opcodes must come in pairs, with */
	null,						/* 100 the minimizing one second. These codes must */
	null,						/* 101 be in exactly the same order as those above. */
	null,						/* 102 */
	null,						/* 103 */

	null,						/* 104 These are different to the three sets above. */
	null,						/* 105 */

	/* End of quantifier opcodes */

	null,						/* 106 Match a character class, chars < 256 only */
	null,						/* 107 Same, but the bitmap was created from a negative
								class - the difference is relevant only when a
								character > 255 is encountered. */
	null,						/* 108 Extended class for handling > 255 chars within the
								class. This does both positive and negative. */
	null,						/* 109 Match a back reference, casefully */
	null,						/* 110 Match a back reference, caselessly */
	null,						/* 111 Match a numbered subpattern (possibly recursive) */
	null,						/* 112 Call out to external function if provided */

	null,						/* 113 Start of alternation */
	null,						/* 114 End of group that doesn't have an unbounded repeat */
	null,						/* 115 These two must remain together and in this */
	null,						/* 116 order. They are for groups the repeat for ever. */
	null,						/* 117 Possessive unlimited repeat. */

								/* The assertions must come before BRA, CBRA, ONCE, and COND, and the four
								asserts must remain in order. */

	null,						/* 118 Move pointer back - used in lookbehind assertions */
	null,						/* 119 Positive lookahead */
	null,						/* 120 Negative lookahead */
	null,						/* 121 Positive lookbehind */
	null,						/* 122 Negative lookbehind */

	/* ONCE, ONCE_NC, BRA, BRAPOS, CBRA, CBRAPOS, and COND must come immediately
	after the assertions, with ONCE first, as there's a test for >= ONCE for a
	subpattern that isn't an assertion. The POS versions must immediately follow
	the non-POS versions in each case. */

	null,						/* 123 Atomic group, contains captures */
	null,						/* 124 Atomic group containing no captures */
	null,						/* 125 Start of non-capturing bracket */
	null,						/* 126 Ditto, with unlimited, possessive repeat */
	null,						/* 127 Start of capturing bracket */
	null,						/* 128 Ditto, with unlimited, possessive repeat */
	null,						/* 129 Conditional group */

	/* These five must follow the previous five, in the same order. There's a
	check for >= SBRA to distinguish the two sets. */

	null,						/* 130 Start of non-capturing bracket, check empty */
	null,						/* 131 Ditto, with unlimited, possessive repeat */
	null,						/* 132 Start of capturing bracket, check empty */
	null,						/* 133 Ditto, with unlimited, possessive repeat */
	null,						/* 134 Conditional group, check empty */

	/* The next two pairs must (respectively) be kept together. */

	null,						/* 135 Used to hold a capture number as condition */
	null,						/* 136 Same, but generated by a name reference*/
	null,						/* 137 Used to hold a recursion number as condition */
	null,						/* 138 Same, but generated by a name reference*/
	null,						/* 139 The DEFINE condition */

	null,						/* 140 These two must remain together and in this */
	null,						/* 141 order. */
	null,						/* 142 */

	/* These are backtracking control verbs */

	null,						/* 143 always has an argument */
	null,						/* 144 */
	null,						/* 145 same, but with argument */
	null,						/* 146 */
	null,						/* 147 same, but with argument */
	null,						/* 148 */
	null,						/* 149 same, but with argument */
	null,						/* 150 */

	/* These are forced failure and success verbs */

	null,						/* 151 */
	null,						/* 152 */
	null,						/* 153 Used inside assertions */
	null,						/* 154 Used before OP_ACCEPT to close open captures */

	/* This is used to skip a subpattern with a {0} quantifier */

	null						/* 155 */

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
//	ulong count = 0;
//	ulong NotProcess = 0;
//	for (std::vector<PCRE>::iterator i = vecPcres.begin(); i != vecPcres.end(); ++i)
//	{
//		std::cout << ++count << std::endl;
//		CNfa nfa;
//		BYTEARY_ITER Beg, End;
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

bool CanProcess(BYTEARY_ITER &Beg, const BYTEARY_ITER &End)
{
	BYTEARY_ITER start, end;
	for (;Beg != End;)
	{
		if (FUNC[*Beg] != null)
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

void NextForCLASS(BYTEARY_ITER &Beg)
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

ulong ProcessPcre(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa)
{
	ulong CurState = nfa.Size();
	bool ALTBeg = false;
	std::vector<PCRESIGN> vecPath;
	std::vector<ulong> PreStates;
	ulong ALTPreBeg = CurState;
	return Process(Beg, End, nfa, CurState, PreStates, ALTPreBeg, ALTBeg, ulong(-1), false, false, false, vecPath);
}

ulong Process(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, std::vector<ulong> &PreStates, ulong ALTPreBeg, bool &ALTBeg, ulong ALTBeginState, bool bCBRA, bool bALT, bool bBRAZERO, std::vector<PCRESIGN> &vecPath)
{
	BYTEARY_ITER start, end;
	ulong CurPreState;
	if (PreStates.size() > 0)
	{
		CurPreState = PreStates.back();
	}
	else
	{
		CurPreState = ulong(-1);
	}
	ulong ALTPreState = ALTPreBeg;
	bool IsCBRA = bCBRA;
	bool IsALT = bALT;
	bool IsBRAZERO = bBRAZERO;
	ulong ALTBegState = ALTBeginState;
	ulong flag = SC_SUCCESS;
	for (;Beg != End;)
	{
		if (FUNC[*Beg] != null)
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

void ProcessALT(CNfa &nfa, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	if (ALTBegin)
	{
		nfa[PreState][EMPTY].PushBack(ALTBegState);
		ALTBegin = false;
	}
}

ulong OP_COMMON_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	++CurState;
	ulong* tmp = ptr[*Beg];

	//std::cout << ulong(*Beg) << " " << NUMS[*Beg] << std::endl;
	for (ulong i = 0; i < NUMS[*Beg]; ++i)
	{
		nfa.Back()[tmp[i]].PushBack(CurState);
	}

	return SC_SUCCESS;
}

void OP_CIRCM_FUNC(CNfa &nfa, ulong &CurState)
{
	//ulong nCursize = nfa.Size();
	//nfa.Resize(nCursize + 3);
	//nfa[nCursize][EMPTY].PushBack(CurState + 1);
	//nfa[nCursize][EMPTY].PushBack(CurState + 3);
	//++CurState;

	//++nCursize;
	//for (ulong i = 0; i < 256; ++i)
	//{
	//	nfa[nCursize][i].PushBack(CurState);
	//}
	//nfa[nCursize]['\n'].PushBack(CurState + 1);
	//nfa[nCursize]['\n'].PushBack(CurState + 2);
	//nfa[nCursize]['\r'].PushBack(CurState + 2);
	//CurState += 2;
	//nfa[nCursize + 1]['\r'].PushBack(CurState);

	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + 2);

	nfa[nCursize][EMPTY].PushBack(CurState + 1);
	nfa[nCursize][EMPTY].PushBack(CurState + 2);

	++CurState;
	++nCursize;
	for (ulong i = 0; i < 256; ++i)
	{
		nfa[nCursize][i].PushBack(CurState);
	}
	
	++CurState;
	nfa[nCursize]['\n'].PushBack(CurState);
}


ulong OP_CHAR_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	++CurState;
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	nfa.Back()[*(Beg + 1)].PushBack(CurState);

	return SC_SUCCESS;
}

ulong OP_CHARI_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	++CurState;
	byte c = *(Beg + 1);
	if (isalpha(c))
	{
		nfa.Back()[tolower(c)].PushBack(CurState);
		nfa.Back()[toupper(c)].PushBack(CurState);
	}
	else
	{
		nfa.Back()[c].PushBack(CurState);
	}
	
	return SC_SUCCESS;
}

ulong OP_NOT_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	++CurState;
	byte c = *(Beg + 1);
	for (ulong j = 0; j < 256; ++j)
	{
		if (j != c)
		{
			nfa.Back()[j].PushBack(CurState);
		}
	}
	
	return SC_SUCCESS;
}

ulong OP_NOTI_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	++CurState;
	byte c = *(Beg + 1);
	for (ulong j = 0; j < 256; ++j)
	{
		if (j != (ulong)tolower(c) && j != (ulong)toupper(c))
		{
			nfa.Back()[j].PushBack(CurState);
		}
	}
	
	return SC_SUCCESS;
}

ulong OP_STAR_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	nfa.Back()[*(Beg + 1)].PushBack(CurState);
	++CurState;
	nfa.Back()[EMPTY].PushBack(CurState);
	
	return SC_SUCCESS;
}

ulong OP_PLUS_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + 2);
	++CurState;
	nfa[nCursize][*(Beg + 1)].PushBack(CurState);
	nfa.Back()[*(Beg + 1)].PushBack(CurState);
	++CurState;
	nfa.Back()[EMPTY].PushBack(CurState);
	
	return SC_SUCCESS;
}

ulong OP_QUERY_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	++CurState;
	nfa.Back()[*(Beg + 1)].PushBack(CurState);
	nfa.Back()[EMPTY].PushBack(CurState);
	
	return SC_SUCCESS;
}

ulong OP_UPTO_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);

	ulong count = GET(Beg + 1);
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + count);
	byte c = *(Beg + 3);
	for (ulong i = 0; i < count; ++i)
	{
		nfa[nCursize + i][EMPTY].PushBack(CurState - i + count);
		++CurState;
		nfa[nCursize + i][c].PushBack(CurState);
	}
	
	return SC_SUCCESS;
}

ulong OP_EXACT_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	ulong count = GET(Beg + 1);
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + count);
	byte c = *(Beg + 3);
	for (ulong i = 0; i < count; ++i)
	{
		++CurState;
		nfa[nCursize + i][c].PushBack(CurState);
	}
	
	return SC_SUCCESS;
}

ulong OP_STARI_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	byte c = *(Beg + 1);
	if (isalpha(c))
	{
		nfa.Back()[tolower(c)].PushBack(CurState);
		nfa.Back()[toupper(c)].PushBack(CurState);
	}
	else
	{
		nfa.Back()[c].PushBack(CurState);
	}
	++CurState;
	nfa.Back()[EMPTY].PushBack(CurState);
	
	return SC_SUCCESS;
}

ulong OP_PLUSI_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + 2);
	byte c = *(Beg + 1);
	++CurState;
	if (isalpha(c))
	{
		nfa[nCursize][tolower(c)].PushBack(CurState);
		nfa[nCursize][toupper(c)].PushBack(CurState);
	}
	else
	{
		nfa[nCursize][c].PushBack(CurState);
	}
	if (isalpha(c))
	{
		nfa.Back()[tolower(c)].PushBack(CurState);
		nfa.Back()[toupper(c)].PushBack(CurState);
	}
	else
	{
		nfa.Back()[c].PushBack(CurState);
	}
	++CurState;
	nfa.Back()[EMPTY].PushBack(CurState);
	
	return SC_SUCCESS;
}

ulong OP_QUERYI_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	byte c = *(Beg + 1);
	++CurState;
	if (isalpha(c))
	{
		nfa.Back()[tolower(c)].PushBack(CurState);
		nfa.Back()[toupper(c)].PushBack(CurState);
	}
	else
	{
		nfa.Back()[c].PushBack(CurState);
	}
	nfa.Back()[EMPTY].PushBack(CurState);
	
	return SC_SUCCESS;
}

ulong OP_UPTOI_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);

	ulong count = GET(Beg + 1);
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + count);
	byte c = *(Beg + 3);
	for (ulong i = 0; i < count; ++i)
	{
		nfa[nCursize + i][EMPTY].PushBack(CurState - i + count);
		++CurState;
		if (isalpha(c))
		{
			nfa[nCursize + i][tolower(c)].PushBack(CurState);
			nfa[nCursize + i][toupper(c)].PushBack(CurState);
		}
		else
		{
			nfa[nCursize + i][c].PushBack(CurState);
		}
	}
	
	return SC_SUCCESS;
}

ulong OP_EXACTI_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	ulong count = GET(Beg + 1);
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + count);
	byte c = *(Beg + 3);
	for (ulong i = 0; i < count; ++i)
	{
		++CurState;
		if (isalpha(c))
		{
			nfa[nCursize + i][tolower(c)].PushBack(CurState);
			nfa[nCursize + i][toupper(c)].PushBack(CurState);
		}
		else
		{
			nfa[nCursize + i][c].PushBack(CurState);
		}
	}
	
	return SC_SUCCESS;
}

ulong OP_NOTSTAR_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	byte c = *(Beg + 1);
	for (ulong i = 0; i < 256; ++i)
	{
		if (i != c)
		{
			nfa.Back()[i].PushBack(CurState);
		}
	}
	++CurState;
	nfa.Back()[EMPTY].PushBack(CurState);
	
	return SC_SUCCESS;
}

ulong OP_NOTPLUS_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + 2);
	byte c = *(Beg + 1);
	++CurState;
	for (ulong i = 0; i < 256; ++i)
	{
		if (i != c)
		{
			nfa[nCursize][i].PushBack(CurState);
			nfa.Back()[i].PushBack(CurState);
		}
	}
	++CurState;
	nfa.Back()[EMPTY].PushBack(CurState);
	
	return SC_SUCCESS;
}

ulong OP_NOTQUERY_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	byte c = *(Beg + 1);
	++CurState;
	for (ulong i = 0; i < 256; ++i)
	{
		if (i != c)
		{
			nfa.Back()[i].PushBack(CurState);
		}
	}
	nfa.Back()[EMPTY].PushBack(CurState);
	
	return SC_SUCCESS;
}

ulong OP_NOTUPTO_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	ulong count = GET(Beg + 1);
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + count);
	byte c = *(Beg + 3);
	for (ulong i = 0; i < count; ++i)
	{
		nfa[nCursize + i][EMPTY].PushBack(CurState - i + count);
		++CurState;
		for (ulong j = 0; j < 256; ++j)
		{
			if (j != c)
			{
				nfa[nCursize + i][j].PushBack(CurState);
			}
		}
	}
	
	return SC_SUCCESS;
}

ulong OP_NOTEXACT_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	ulong count = GET(Beg + 1);
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + count);
	byte c = *(Beg + 3);
	for (ulong i = 0; i < count; ++i)
	{
		++CurState;
		for (ulong j = 0; j < 256; ++j)
		{
			if (j != c)
			{
				nfa[nCursize + i][j].PushBack(CurState);
			}
		}
	}
	
	return SC_SUCCESS;
}

ulong OP_NOTSTARI_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	byte c = *(Beg + 1);
	for (ulong i = 0; i < 256; ++i)
	{
		if (i != (ulong)tolower(c) && i != (ulong)toupper(c))
		{
			nfa.Back()[i].PushBack(CurState);
		}
	}
	++CurState;
	nfa.Back()[EMPTY].PushBack(CurState);
	
	return SC_SUCCESS;
}

ulong OP_NOTPLUSI_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + 2);
	++CurState;
	byte c = *(Beg + 1);
	for (ulong i = 0; i < 256; ++i)
	{
		if (i != (ulong)tolower(c) && i != (ulong)toupper(c))
		{
			nfa[nCursize][i].PushBack(CurState);
			nfa.Back()[i].PushBack(CurState);
		}
	}
	++CurState;
	nfa.Back()[EMPTY].PushBack(CurState);
	
	return SC_SUCCESS;
}

ulong OP_NOTQUERYI_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	++CurState;
	byte c = *(Beg + 1);
	for (ulong i = 0; i < 256; ++i)
	{
		if (i != (ulong)tolower(c) && i != (ulong)toupper(c))
		{
			nfa.Back()[i].PushBack(CurState);
		}
	}
	nfa.Back()[EMPTY].PushBack(CurState);
	
	return SC_SUCCESS;
}

ulong OP_NOTUPTOI_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	ulong count = GET(Beg + 1);
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + count);
	byte c = *(Beg + 3);
	for (ulong i = 0; i < count; ++i)
	{
		nfa[nCursize + i][EMPTY].PushBack(CurState - i + count);
		++CurState;
		for (ulong j = 0; j < 256; ++j)
		{
			if (j != (ulong)tolower(c) && j != (ulong)toupper(c))
			{
				nfa[nCursize + i][j].PushBack(CurState);
			}
		}
	}
	
	return SC_SUCCESS;
}

ulong OP_NOTEXACTI_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	ulong count = GET(Beg + 1);
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + count);
	byte c = *(Beg + 3);
	for (ulong i = 0; i < count; ++i)
	{
		++CurState;
		for (ulong j = 0; j < 256; ++j)
		{
			if (j != (ulong)tolower(c) && j != (ulong)toupper(c))
			{
				nfa[nCursize + i][j].PushBack(CurState);
			}
		}
	}
	
	return SC_SUCCESS;
}

ulong OP_TYPESTAR_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	byte c = *(Beg + 1);
	if (c < sizeof(NUMS) / sizeof(ulong) && ptr[c] != null)
	{
		ulong nCursize = nfa.Size();
		nfa.Resize(nCursize + 1);
		ulong Cnt = NUMS[c];
		ulong* tmp = ptr[c];
		for (ulong i = 0; i < Cnt; ++i)
		{
			nfa.Back()[*(tmp + i)].PushBack(CurState);
		}
		++CurState;
		nfa.Back()[EMPTY].PushBack(CurState);
	}
	
	return SC_SUCCESS;
}

ulong OP_TYPEPLUS_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	byte c = *(Beg + 1);
	if (c < sizeof(NUMS) / sizeof(ulong) && ptr[c] != null)
	{
		ulong nCursize = nfa.Size();
		nfa.Resize(nCursize + 2);
		++CurState;
		ulong Cnt = NUMS[c];
		ulong* tmp = ptr[c];
		for (ulong i = 0; i < Cnt; ++i)
		{
			nfa[nCursize][*(tmp + i)].PushBack(CurState);
			nfa.Back()[*(tmp + i)].PushBack(CurState);
		}
		++CurState;
		nfa.Back()[EMPTY].PushBack(CurState);
	}
	
	return SC_SUCCESS;
}

ulong OP_TYPEQUERY_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	byte c = *(Beg + 1);
	if (c < sizeof(NUMS) / sizeof(ulong) && ptr[c] != null)
	{
		ulong nCursize = nfa.Size();
		nfa.Resize(nCursize + 1);
		++CurState;
		ulong Cnt = NUMS[c];
		ulong* tmp = ptr[c];
		for (ulong i = 0; i < Cnt; ++i)
		{
			nfa.Back()[*(tmp + i)].PushBack(CurState);
		}
		nfa.Back()[EMPTY].PushBack(CurState);
	}
	
	return SC_SUCCESS;
}

ulong OP_TYPEUPTO_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	ulong count = GET(Beg + 1);
	bool ALTBeg = ALTBegin;
	byte c = *(Beg + 3);
	if (c >=OP_NOT_DIGIT && c <= OP_ALLANY && FUNC[c] != null)
	{
		BYTEARY_ITER tmpBeg = Beg + 3;
		for (ulong i = 0; i < count; ++i)
		{
			FUNC[c](tmpBeg, End, nfa, CurState, PreState, ALTBeg, ALTBegState);
			nfa.Back()[EMPTY].PushBack(CurState - 1 + count - i);
		}
	}
	
	return SC_SUCCESS;
}

ulong OP_TYPEEXACT_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	ulong count = GET(Beg + 1);
	bool ALTBeg = ALTBegin;
	byte c = *(Beg + 3);
	if (c >=OP_NOT_DIGIT && c <= OP_ALLANY && FUNC[c] != null)
	{
		BYTEARY_ITER tmpBeg = Beg + 3;

		for (ulong i = 0; i < count; ++i)
		{
			FUNC[c](tmpBeg, End, nfa, CurState, PreState, ALTBeg, ALTBegState);
		}
	}
	
	return SC_SUCCESS;
}

ulong OP_CLASS_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	CNfaRow Row;
	ulong tmp = CurState + 1;
	for (ulong i = 0; i < 32; ++i)
	{
		for (ulong j = 0; j < 8; ++j)
		{
			if (*(Beg + i + 1) & 1 << j)
			{
				nfa.Back()[i * 8 + j].PushBack(tmp);
				Row[i * 8 + j].PushBack(tmp);
			}
		}
	}
	Beg += Steps[OP_CLASS];
	ulong min, max;

	switch (*Beg)
	{
	case OP_CRSTAR:
		for (ulong i = 0; i < CNfaRow::COLUMNCNT; ++i)
		{
			for (ulong j = 0; j < nfa.Back()[i].Size(); ++j)
			{
				if (nfa.Back()[i][j] == CurState + 1)
				{
					nfa.Back()[i][j] = CurState;
				}
			}
		}
		++CurState;
		nfa.Back()[EMPTY].PushBack(CurState);
		Beg += Steps[OP_CRSTAR];
		break;
	case OP_CRMINSTAR:
		for (ulong i = 0; i < CNfaRow::COLUMNCNT; ++i)
		{
			for (ulong j = 0; j < nfa.Back()[i].Size(); ++j)
			{
				if (nfa.Back()[i][j] == CurState + 1)
				{
					nfa.Back()[i][j] = CurState;
				}
			}
		}
		++CurState;
		nfa.Back()[EMPTY].PushBack(CurState);
		Beg += Steps[OP_CRMINSTAR];
		break;
	case OP_CRPLUS:
		nfa.Resize(nCursize + 2);
		Copy(nfa.Back(), Row, 0);
		++CurState;
		++CurState;
		nfa.Back()[EMPTY].PushBack(CurState);
		Beg += Steps[OP_CRPLUS];
		break;
	case OP_CRMINPLUS:
		nfa.Resize(nCursize + 2);
		Copy(nfa.Back(), Row, 0);
		++CurState;
		++CurState;
		nfa.Back()[EMPTY].PushBack(CurState);
		Beg += Steps[OP_CRMINPLUS];
		break;
	case OP_CRQUERY:
		++CurState;
		nfa.Back()[EMPTY].PushBack(CurState);
		Beg += Steps[OP_CRQUERY];
		break;
	case OP_CRRANGE:
		min = GET(Beg + 1);
		max = GET(Beg + 3);
		nfa.PopBack();
		nfa.Resize(nCursize + min);
		for (ulong i = 0; i < min; ++i)
		{
			Copy(nfa[nCursize + i], Row, i);
			++CurState;
		}
		if (max == 0)
		{
			nfa.Resize(nCursize + min + 1);
			for (ulong i = 0; i < CNfaRow::COLUMNCNT; ++i)
			{
				if (Row[i].Size() == 1)
				{
					nfa.Back()[i].PushBack(CurState);
				}
			}
			++CurState;
			nfa.Back()[EMPTY].PushBack(CurState);
		}
		else
		{
			nfa.Resize(nCursize + max);
			ulong nDiff = max - min, nBeg = nCursize + min;
			for (ulong i = 0; i < nDiff; ++i)
			{
				Copy(nfa[nBeg + i], Row, min + i);
				nfa[nBeg + i][EMPTY].PushBack(CurState - i + nDiff);
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

ulong OP_NCLASS_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState, ulong PreState, bool &ALTBegin, ulong ALTBegState)
{
	ProcessALT(nfa, PreState, ALTBegin, ALTBegState);
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	CNfaRow Row;
	ulong tmp = CurState + 1;
	for (ulong i = 0; i < 32; ++i)
	{
		for (ulong j = 0; j < 8; ++j)
		{
			if (*(Beg + i + 1) & 1 << j)
			{
				ulong nCol = i * 8 + j;
				nfa.Back()[nCol].PushBack(tmp);
				Row[nCol].PushBack(tmp);
			}
		}
	}
	Beg += Steps[OP_CLASS];
	ulong min, max;
	switch (*Beg)
	{
	case OP_CRSTAR:
		for (ulong i = 0; i < CNfaRow::COLUMNCNT; ++i)
		{
			ulong nCnt = nfa.Back()[i].Size();
			for (ulong j = 0; j < nCnt; ++j)
			{
				ulong &nSta = nfa.Back()[i][j];
				if (nSta == CurState + 1)
				{
					nSta = CurState;
				}
			}
		}
		++CurState;
		nfa.Back()[EMPTY].PushBack(CurState);
		Beg += Steps[OP_CRSTAR];
		break;
	case OP_CRMINSTAR:
		for (ulong i = 0; i < CNfaRow::COLUMNCNT; ++i)
		{
			ulong nCnt = nfa.Back()[i].Size();
			for (ulong j = 0; j < nCnt; ++j)
			{
				ulong &nSta = nfa.Back()[i][j];
				if (nSta == CurState + 1)
				{
					nSta = CurState;
				}
			}
		}
		++CurState;
		nfa.Back()[EMPTY].PushBack(CurState);
		Beg += Steps[OP_CRMINSTAR];
		break;
	case OP_CRPLUS:
		nfa.Resize(nCursize + 2);
		Copy(nfa.Back(), Row, 0);
		++CurState;
		++CurState;
		nfa.Back()[EMPTY].PushBack(CurState);
		Beg += Steps[OP_CRPLUS];
		break;
	case OP_CRMINPLUS:
		nfa.Resize(nCursize + 2);
		Copy(nfa.Back(), Row, 0);
		++CurState;
		++CurState;
		nfa.Back()[EMPTY].PushBack(CurState);
		Beg += Steps[OP_CRMINPLUS];
		break;
	case OP_CRQUERY:
		++CurState;
		nfa.Back()[EMPTY].PushBack(CurState);
		Beg += Steps[OP_CRQUERY];
		break;
	case OP_CRRANGE:
		min = GET(Beg + 1);
		max = GET(Beg + 3);
		nfa.PopBack();
		nfa.Resize(nCursize + min);
		for (ulong i = 0; i < min; ++i)
		{
			Copy(nfa[nCursize + i], Row, i);
			++CurState;
		}
		if (max == 0)
		{
			nfa.Resize(nCursize + min + 1);
			for (ulong i = 0; i < CNfaRow::COLUMNCNT; ++i)
			{
				if (Row[i].Size() == 1)
				{
					nfa.Back()[i].PushBack(CurState);
				}
			}
			++CurState;
			nfa.Back()[EMPTY].PushBack(CurState);
		}
		else
		{
			nfa.Resize(nCursize + max);
			for (ulong i = 0; i < max - min; ++i)
			{
				ulong nCnt = min + i;
				Copy(nfa[nCursize + nCnt], Row, nCnt);
				nfa[nCursize + nCnt][EMPTY].PushBack(CurState + max - nCnt);
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

void OP_ALT_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong PreState, ulong &CurState)
{
	for (ulong i = PreState; i < nfa.Size(); ++i)
	{
		CNfaRow &row = nfa[i];
		for (ulong j = 0; j < CNfaRow::COLUMNCNT; ++j)
		{
			ulong nCnt = row[j].Size();
			for (ulong k = 0; k < nCnt; ++k)
			{
				ulong &nSta = row[j][k];
				if (nSta == CurState)
				{
					nSta = MAX;
				}
			}
		}
	}
	if (Beg == End)
	{
		nfa[PreState][EMPTY].PushBack(MAX);
	}
}

void OP_KET_FUNC(CNfa &nfa, ulong PreState, ulong &CurState, bool IsALT, bool IsBRAZERO)
{
	if (IsBRAZERO)
	{
		nfa[PreState][EMPTY].PushBack(CurState);
	}
	if (IsALT)
	{
		for (ulong i = PreState; i < nfa.Size(); ++i)
		{
			CNfaRow &row = nfa[i];
			for (ulong j = 0; j < CNfaRow::COLUMNCNT; ++j)
			{
				ulong nCnt = row[j].Size();
				for (ulong k = 0; k < nCnt; ++k)
				{
					ulong &nSta = row[j][k];
					if (nSta == MAX)
					{
						nSta = CurState;
					}
				}
			}
		}
	}
}

void OP_KETRMAX_FUNC(CNfa &nfa, ulong PreState, ulong &CurState, bool IsBRAZERO, bool IsALT)
{
	if (IsALT)
	{
		for (ulong i = PreState; i < nfa.Size(); ++i)
		{
			CNfaRow &row = nfa[i];
			for (ulong j = 0; j < CNfaRow::COLUMNCNT; ++j)
			{
				ulong nCnt = row[j].Size();
				for (ulong k = 0; k < nCnt; ++k)
				{
					ulong &nSta = row[j][k];
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
		for (ulong i = PreState; i < nfa.Size(); ++i)
		{
			CNfaRow &row = nfa[i];
			for (ulong j = 0; j < CNfaRow::COLUMNCNT; ++j)
			{
				ulong nCnt = row[j].Size();
				for (ulong k = 0; k < nCnt; ++k)
				{
					ulong &nSta = row[j][k];
					if (nSta == CurState)
					{
						nSta = PreState;
					}
				}
			}
			
		}
		nfa[PreState][EMPTY].PushBack(CurState);
	}
	else
	{
		ulong nCursize = nfa.Size();
		nfa.Resize(nCursize + nCursize - PreState);
		ulong StartState = CurState;
		for (ulong i = PreState; i < nCursize; ++i)
		{
			CNfaRow &row = nfa[i];
			for (ulong j = 0; j < CNfaRow::COLUMNCNT; ++j)
			{
				ulong nCnt = row[j].Size();
				for (ulong k = 0; k < nCnt; ++k)
				{
					nfa[nCursize + i - PreState][j].PushBack(CurState + row[j][k] - i);
				}
			}
			++CurState;
		}
		for (ulong i = StartState; i < nfa.Size(); ++i)
		{
			CNfaRow &row = nfa[i];
			for (ulong j = 0; j < CNfaRow::COLUMNCNT; ++j)
			{
				ulong nCnt = row[j].Size();
				for (ulong k = 0; k < nCnt; ++k)
				{
					ulong &nSta = row[j][k];
					if (nSta == CurState)
					{
						nSta = StartState;
					}
				}
			}
		}
		nfa[StartState][EMPTY].PushBack(CurState);
	}
}

ulong OP_BRA_CBRA_SCBRA_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState)
{
	if (Beg == End)
	{
		ulong nCursize = nfa.Size();
		nfa.Resize(nCursize + 1);
		nfa.Back()[EMPTY].PushBack(MAX);
		++CurState;
	}
	
	return SC_SUCCESS;
}

ulong OP_BRA_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState)
{
	return OP_BRA_CBRA_SCBRA_FUNC(Beg, End, nfa, CurState);
}

ulong OP_CBRA_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState)
{
	return OP_BRA_CBRA_SCBRA_FUNC(Beg, End, nfa, CurState);
}

ulong OP_SCBRA_FUNC(BYTEARY_ITER &Beg, const BYTEARY_ITER &End, CNfa &nfa, ulong &CurState)
{
	return OP_BRA_CBRA_SCBRA_FUNC(Beg, End, nfa, CurState);
}

void Copy(CNfaRow &NewRow, CNfaRow &Row, ulong increment)
{
	for (ulong i = 0; i < CNfaRow::COLUMNCNT; ++i)
	{
		ulong nDestCnt = Row[i].Size();
		for (ulong j = 0; j < nDestCnt; ++j)
		{
			NewRow[i].PushBack(Row[i][j] + increment);
		}
	}
}

void AddEMPTY(CNfa &nfa, ulong &CurState)
{
	ulong nCursize = nfa.Size();
	nfa.Resize(nCursize + 1);
	++CurState;
	nfa.Back()[EMPTY].PushBack(CurState);
}

void OutPut(std::vector<PCRE>::iterator &Pcre, CNfa &nfa, ulong count)
{
	std::stringstream ss;
	std::string filename;
	filename = "D:\\NFAResults\\";
	ss << count;
	filename += ss.str();
	filename += ".txt";
	std::ofstream fout(filename.c_str(), std::ios::binary);

	ulong Cnt;
	byte c;
	Cnt = Pcre->PcreStr.length();
	fout.write((char*)&Cnt, 4);
	for (ulong i = 0; i < Cnt; ++i)
	{
		c = Pcre->PcreStr[i];
		fout.write((char*)&c, 1);
	}
	Cnt = nfa.Size();
	fout.write((char*)&Cnt, 4);
	ulong num;
	for (ulong i = 0; i < Cnt; ++i)
	{
		CNfaRow &row = nfa[i];
		for (ulong j = 0; j < CNfaRow::COLUMNCNT; ++j)
		{
			num = row[j].Size();
			fout.write((char*)&num, 4);
			for (ulong k = 0; k < num; ++k)
			{
				fout.write((char*)&(row[j][k]), 4);
			}
		}
	}
	fout.close();
	fout.clear();
}

#pragma warning (pop)
