#include "stdafx.h"

#include "pcre\pcre.h"
#include "p2nmain.h"
#include "pcre2nfa.h"

#define MYGET(cur) ((code[cur + 1] << 8) | code[cur + 2])

void GetSignature(BYTEARY &code, std::vector<BYTEARY> &strs)
{
	ulong cur = 0;
	ulong bra = 0;
	ulong times = 0;
	byte temp = 0;
	byte curCode = code[0];
	BYTEARY str;
	for(;;)
	{
		switch(curCode)
		{
		case OP_END:
			if(!str.empty() && (str.size() >= 4))
			{
				strs.resize(strs.size() + 1);
				strs.back() = str;
			}
			str.clear();
			return;

		case  OP_ALT:				/* 113 Start of alternation */
			strs.clear();
			return;
			break;

		case OP_CHARI:			 /* 30 Match one character: caselessly */
		case OP_CHAR:			/* 29 Match one character: casefully */
			str.resize(str.size() + 1);
			str.back() = code[cur +1];
			cur = cur + Steps[curCode];
			curCode = code[cur];
			break;

		case OP_CBRA:			/* 127 Start of capturing bracket */
			bra = cur;
			bra += MYGET(bra);
			if(code[bra] == OP_ALT)
			{
				if(!str.empty() && (str.size() >= 4))
				{
					strs.resize(strs.size() + 1);
					strs.back() = str;
				}
				str.clear();
				while(code[bra] == OP_ALT)
				{
					bra += MYGET(bra);
				}

				cur = bra + Steps[code[bra]];
				curCode = code[cur];

			}
			else
			{
				cur += Steps[curCode];
				curCode = code[cur];
			}
			break;

		case OP_POSPLUS:		/* 43 Possessified plus: caseful */
			str.resize(str.size() + 1);
			str.back() = code[cur + 1];
			if(str.size() >= 4)
			{
				strs.resize(strs.size() + 1);
				strs.back() = str;
			}
			str.clear();
			str.resize(str.size() + 1);
			str.back() = code[cur + 1];
			
			cur = cur + Steps[curCode];
			curCode = code[cur];
			break;

		case OP_KET:			/* 114 End of group that doesn't have an unbounded repeat */
			cur = cur + Steps[curCode];
			curCode = code[cur];
			break;

		case OP_EXACT:			/* 41 Exactly n matches */
		case OP_EXACTI:			/* 54 */

			times = MYGET(cur);
			for(ulong i = 0; i < times; ++i)
			{
				str.resize(str.size() + 1);
				str.back() = code[cur + 3];
			}
			temp = code[cur + Steps[curCode]];
			if(((temp == OP_UPTO) || (temp == OP_MINUPTO) || (temp == OP_UPTOI) || (temp == OP_MINUPTOI)) && (code[cur + 3] == code[cur + Steps[curCode] + 3]))
			{
				if(!str.empty() && (str.size() >= 4))
				{
					strs.resize(strs.size() + 1);
					strs.back() = str;
				}
				str.clear();
				for(ulong i = 0; i < times; ++i)
				{
					str.resize(str.size() + 1);
					str.back() = code[cur + 3];
				}
				cur = cur + Steps[curCode] + Steps[OP_UPTO];
				curCode = code[cur];
			}
			else
			{
				cur = cur + Steps[curCode];
				curCode = code[cur];
			}
			break;

		case OP_PLUS:			/* 35 the minimizing one second. */
			str.resize(str.size() + 1);
			str.back() = code[cur + 1];
			if(str.size() >= 4)
			{
				strs.resize(strs.size() + 1);
				strs.back() = str;
			}
			str.clear();
			str.resize(str.size() + 1);
			str.back() = code[cur + 1];

			cur = cur + Steps[curCode];
			curCode = code[cur];
			break;

		case OP_BRAZERO:		/* 140 These two must remain together and in this */

			if(!str.empty() && (str.size() >= 4))
			{
				strs.resize(strs.size()+ 1);
				strs.back() = str;
			}
			str.clear();

			bra = cur + Steps[curCode];
			bra += MYGET(bra);
			while(code[bra] == OP_ALT)
			{
				bra += MYGET(bra);
			}

			bra += Steps[OP_KET];
			cur = bra;
			curCode = code[cur];

			break;

		case OP_SOD:				/* 1 Start of data: \A */

		case OP_SOM:				/* 2 Start of match (subject + offset): \G */
		case OP_SET_SOM:			/* 3 Set start of match (\K) */
		case OP_NOT_WORD_BOUNDARY:  /*  4 \B */
		case OP_WORD_BOUNDARY:		/*  5 \b */
		case OP_NOT_DIGIT:			/*  6 \D */
		case OP_DIGIT:				/*  7 \d */
		case OP_NOT_WHITESPACE:		/*  8 \S */
		case OP_WHITESPACE:			/*  9 \s */
		case OP_NOT_WORDCHAR:		/* 10 \W */
		case OP_WORDCHAR:			/* 11 \w */

		case OP_ANY:				/* 12 Match any character except newline (\N) */
		case OP_ALLANY:				/* 13 Match any character */
		case OP_ANYBYTE:			/* 14 Match any byte (\C); different to OP_ANY for UTF-8 */
		case OP_NOTPROP:			/* 15 \P (not Unicode property) */
		case OP_PROP:				/* 16 \p (Unicode property) */
		case OP_ANYNL:				/* 17 \R (any newline sequence) */
		case OP_NOT_HSPACE:			/* 18 \H (not horizontal whitespace) */
		case OP_HSPACE:				/* 19 \h (horizontal whitespace) */
		case OP_NOT_VSPACE:			/* 20 \V (not vertical whitespace) */
		case OP_VSPACE:				/* 21 \v (vertical whitespace) */
		case OP_EXTUNI:				/* 22 \X (extended Unicode sequence */
		case OP_EODN:				/* 23 End of data or \n at end of data (\Z) */
		case OP_EOD:				/* 24 End of data (\z) */

		case OP_CIRC:				/* 25 Start of line - not multiline */



		case OP_CIRCM:				/* 26 Start of line - multiline */
		case OP_DOLL:				/* 27 End of line - not multiline */
		case OP_DOLLM:				/* 28 End of line - multiline */


		case OP_NOT:				/* 31 Match one character: not the given one: casefully */
		case OP_NOTI:				/* 32 Match one character: not the given one: caselessly */

			/* The following sets of 13 opcodes must always be kept in step because
			the offset from the first one is used to generate the others. */

			/**** Single characters: caseful: must precede the caseless ones ****/

		case OP_STAR:			/* 33 The maximizing and minimizing versions of */
		case OP_MINSTAR:		/* 34 these six opcodes must come in pairs: with */
		case OP_MINPLUS:		/* 36 */
		case OP_QUERY:			 /* 37 */
		case OP_MINQUERY:		 /* 38 */

		case OP_UPTO:			/* 39 From 0 to n matches of one character: caseful*/
		case OP_MINUPTO:		/* 40 */

		case OP_POSSTAR:		/* 42 Possessified star: caseful */
		case OP_POSQUERY:		 /* 44 Posesssified query: caseful */
		case OP_POSUPTO:		/* 45 Possessified upto: caseful */

			/**** Single characters: caseless: must follow the caseful ones */

		case OP_STARI:			 /* 46 */
		case OP_MINSTARI:		 /* 47 */
		case OP_PLUSI:			 /* 48 */
		case OP_MINPLUSI:		 /* 49 */
		case OP_QUERYI:			/* 50 */
		case OP_MINQUERYI:		/* 51 */

		case OP_UPTOI:			 /* 52 From 0 to n matches of one character: caseless */
		case OP_MINUPTOI:		 /* 53 */

		case OP_POSSTARI:		 /* 55 Possessified star: caseless */
		case OP_POSPLUSI:		 /* 56 Possessified plus: caseless */
		case OP_POSQUERYI:		/* 57 Posesssified query: caseless */
		case OP_POSUPTOI:		 /* 58 Possessified upto: caseless */

			/**** The negated ones must follow the non-negated ones: and match them ****/
			/**** Negated single character: caseful; must precede the caseless ones ****/

		case OP_NOTSTAR:		/* 59 The maximizing and minimizing versions of */
		case OP_NOTMINSTAR:	  /* 60 these six opcodes must come in pairs: with */
		case OP_NOTPLUS:		/* 61 the minimizing one second. They must be in */
		case OP_NOTMINPLUS:	  /* 62 exactly the same order as those above. */
		case OP_NOTQUERY:		 /* 63 */
		case OP_NOTMINQUERY:	 /* 64 */

		case OP_NOTUPTO:		/* 65 From 0 to n matches: caseful */
		case OP_NOTMINUPTO:	  /* 66 */
		case OP_NOTEXACT:		 /* 67 Exactly n matches */

		case OP_NOTPOSSTAR:	  /* 68 Possessified versions: caseful */
		case OP_NOTPOSPLUS:	  /* 69 */
		case OP_NOTPOSQUERY:	 /* 70 */
		case OP_NOTPOSUPTO:	  /* 71 */

			/**** Negated single character: caseless; must follow the caseful ones ****/

		case OP_NOTSTARI:		 /* 72 */
		case OP_NOTMINSTARI:	 /* 73 */
		case OP_NOTPLUSI:		 /* 74 */
		case OP_NOTMINPLUSI:	 /* 75 */
		case OP_NOTQUERYI:		/* 76 */
		case OP_NOTMINQUERYI:	/* 77 */

		case OP_NOTUPTOI:		 /* 78 From 0 to n matches: caseless */
		case OP_NOTMINUPTOI:	 /* 79 */
		case OP_NOTEXACTI:		/* 80 Exactly n matches */

		case OP_NOTPOSSTARI:	 /* 81 Possessified versions: caseless */
		case OP_NOTPOSPLUSI:	 /* 82 */
		case OP_NOTPOSQUERYI:	/* 83 */
		case OP_NOTPOSUPTOI:	 /* 84 */

			/**** Character types ****/

		case OP_TYPESTAR:		 /* 85 The maximizing and minimizing versions of */
		case OP_TYPEMINSTAR:	 /* 86 these six opcodes must come in pairs: with */
		case OP_TYPEPLUS:		 /* 87 the minimizing one second. These codes must */
		case OP_TYPEMINPLUS:	 /* 88 be in exactly the same order as those above. */
		case OP_TYPEQUERY:		/* 89 */
		case OP_TYPEMINQUERY:	/* 90 */

		case OP_TYPEUPTO:		 /* 91 From 0 to n matches */
		case OP_TYPEMINUPTO:	 /* 92 */
		case OP_TYPEEXACT:		/* 93 Exactly n matches */

		case OP_TYPEPOSSTAR:	 /* 94 Possessified versions */
		case OP_TYPEPOSPLUS:	 /* 95 */
		case OP_TYPEPOSQUERY:	/* 96 */
		case OP_TYPEPOSUPTO:	 /* 97 */

			/* These are used for character classes and back references; only the
			first six are the same as the sets above. */

		case OP_CRSTAR:			/* 98 The maximizing and minimizing versions of */
		case OP_CRMINSTAR:		/* 99 all these opcodes must come in pairs: with */
		case OP_CRPLUS:			/* 100 the minimizing one second. These codes must */
		case OP_CRMINPLUS:		/* 101 be in exactly the same order as those above. */
		case OP_CRQUERY:		/* 102 */
		case OP_CRMINQUERY:	  /* 103 */

		case OP_CRRANGE:		/* 104 These are different to the three sets above. */
		case OP_CRMINRANGE:	  /* 105 */

			/* End of quantifier opcodes */

		case OP_CLASS:			 /* 106 Match a character class: chars < 256 only */
		case OP_NCLASS:	  
		case OP_XCLASS:	  
		case OP_REF:				/* 109 Match a back reference: casefully */
		case OP_REFI:			/* 110 Match a back reference: caselessly */
		case OP_RECURSE:		/* 111 Match a numbered subpattern (possibly recursive) */
		case OP_CALLOUT:		/* 112 Call out to external function if provided */



		case OP_KETRMAX:		/* 115 These two must remain together and in this */
		case OP_KETRMIN:		/* 116 order. They are for groups the repeat for ever. */
		case OP_KETRPOS:		/* 117 Possessive unlimited repeat. */


		case OP_REVERSE:		/* 118 Move pointer back - used in lookbehind assertions */
		case OP_ASSERT:			/* 119 Positive lookahead */
		case OP_ASSERT_NOT:	  /* 120 Negative lookahead */
		case OP_ASSERTBACK:	  /* 121 Positive lookbehind */
		case OP_ASSERTBACK_NOT: /* 122 Negative lookbehind */


		case OP_ONCE:			/* 123 Atomic group: contains captures */
		case OP_ONCE_NC:		/* 124 Atomic group containing no captures */
		case OP_BRA:				/* 125 Start of non-capturing bracket */

		case OP_BRAPOS:			/* 126 Ditto: with unlimited: possessive repeat */

		case OP_CBRAPOS:		/* 128 Ditto: with unlimited: possessive repeat */
		case OP_COND:			/* 129 Conditional group */

			/* These five must follow the previous five: in the same order. There's a
			check for >= SBRA to distinguish the two sets. */

		case OP_SBRA:			/* 130 Start of non-capturing bracket: check empty  */
		case OP_SBRAPOS:		/* 131 Ditto: with unlimited: possessive repeat */
		case OP_SCBRA:			 /* 132 Start of capturing bracket: check empty */
		case OP_SCBRAPOS:		 /* 133 Ditto: with unlimited: possessive repeat */
		case OP_SCOND:			 /* 134 Conditional group: check empty */

			/* The next two pairs must (respectively) be kept together. */

		case OP_CREF:			/* 135 Used to hold a capture number as condition */
		case OP_NCREF:			 /* 136 Same: but generated by a name reference*/
		case OP_RREF:			/* 137 Used to hold a recursion number as condition */
		case OP_NRREF:			 /* 138 Same: but generated by a name reference*/
		case OP_DEF:				/* 139 The DEFINE condition */

		case OP_BRAMINZERO:	  /* 141 order. */
		case OP_BRAPOSZERO:	  /* 142 */


		case OP_MARK:			
		case OP_PRUNE:			
		case OP_PRUNE_ARG:	  
		case OP_SKIP:			
		case OP_THEN:			
		case OP_THEN_ARG:		 
		case OP_COMMIT:			



		case OP_FAIL:			
		case  OP_ACCEPT:		 
		case OP_ASSERT_ACCEPT:  
		case OP_CLOSE:		

		case OP_SKIPZERO:		 

		case  OP_TABLE_LENGTH:
			cur = cur + Steps[curCode];
			curCode = code[cur];
			if(!str.empty() && (str.size() >= 4))
			{
				strs.resize(strs.size() + 1);
				strs.back() = str;
			}
				str.clear();
			break;
		}
	}
}
//使用Pcre8.32库解析单个pcre
ulong PcreToCode(const std::string &OnePcre, BYTEARY &code)
{
	ulong nFromBeg = 0;
	std::string Pcre;//pcre的具体内容
	std::string attribute;//pcre之后的修饰

	int BegPos;//pcre中第一个/出现位置
	int EndPos;//pcre中最后一个/出现位置
	BegPos = OnePcre.find("/", 0);
	EndPos = OnePcre.find_last_of("/");
	if (BegPos != -1 && EndPos != -1 && BegPos < EndPos)
	{
		Pcre = OnePcre.substr(BegPos + 1, EndPos - BegPos - 1);
	}
	else
	{
		std::cout << "Input Pcre Error!" << std::endl;
		return ulong(-2);
	}
	if (!Pcre.empty() && Pcre[0] != '^')
	{
		nFromBeg = ulong(-1);
		Pcre = "(" + Pcre + ")";
	}
	//至此pcre的具体内容已存放至变量Pcre中

	attribute = OnePcre.substr(EndPos + 1, OnePcre.size() - EndPos - 1);
	//pcre的修饰内容已存放入变量attribute中

	const char* pattern = Pcre.c_str();
	int options = 0;
	for (std::string::iterator i = attribute.begin(); i != attribute.end(); ++i)
	{
		switch(*i)
		{
		case 's':
			options |= PCRE_DOTALL;
			break;
		case 'm':
			options |= PCRE_MULTILINE;
			break;
		case 'i':
			options |= PCRE_CASELESS;
			break;
		}
	}

	pcre *re;
	const char *error;
	int erroffset;
	re = pcre_compile(pattern, options, &error, &erroffset, null);
	if (re == null)
	{
		std::cout << pattern << std::endl;
		printf("PCRE compilation failed at offset %d: %s\n", erroffset, error);
		return ulong(-2);
	}

	unsigned int size;
	unsigned short name_table_offset;

	size = *((unsigned int*)re + 1);
	name_table_offset = *((unsigned short*)re + 12);

	for (ulong i = 0; i < size - name_table_offset; ++i)
	{
		code.push_back((byte)*((byte*)re + name_table_offset + i));
	}

	return nFromBeg;
}

//把单个pcre转化为NFA
ulong PcreToNFA(const char *pPcre, CNfa &nfa, CSignatures &sigs)
{
	BYTEARY code;
	std::string strPcre(pPcre);
	ulong nFromBeg = PcreToCode(strPcre, code);
	BYTEARY_ITER Beg, End;
	Beg = code.begin();
	End = code.end();
	if (!CanProcess(Beg, End))
	{
		return ulong(SC_ERROR);
	}
	Beg = code.begin();
	End = code.end();

	if (code.size() > 0)
	{
		std::vector<BYTEARY> strs;
		GetSignature(code, strs);

		if (strs.size() > 0)
		{
			for (ulong i = 0; i < strs.size(); ++i)
			{
				for(BYTEARY_ITER iter = strs[i].begin(); iter != strs[i].end(); ++iter)
				{
					if ((*iter >= 65) && (*iter <= 90))
					{
						*iter = (byte)tolower(*iter);
					}
				}
			}

			for (ulong i = 0; i < strs.size(); ++i)
			{
				for (BYTEARY_ITER iter = strs[i].begin(); iter + 3 != strs[i].end(); ++iter)
				{
					SIGNATURE sig = *(SIGNATURE*)&(*iter);
					sigs.PushBack(sig);
				}
			}
		}
	}

	if (nFromBeg == ulong(-1))
	{
		ulong nCurSize = nfa.Size();
		nfa.Resize(nCurSize + 1);
		CNfaRow &row = nfa.Back();
		for (ulong i = 0; i < EMPTY; ++i)
		{
			row[i].PushBack(nCurSize);
		}
		row[EMPTY].PushBack(nCurSize + 1);
	}
	ulong nr = ProcessPcre(Beg, End, nfa);
	return nr;
}

