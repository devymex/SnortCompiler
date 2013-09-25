/*!*
* @file				compilernew.cpp
**
* @author			Lab 435, Xidian University
**
* @brief			Support functions for compile rule to dfa
**
* This implements compile rule to dfa algorithm
**
*/

#include "stdafx.h"
#include <hwprj\snortrule.h>
#include "comprule.h"
#include "p2nmain.h"
#include <hwprj\compiler.h>

COMPILERHDR double rule2pcretime = 0.0;
COMPILERHDR double pcre2nfatime = 0.0;
COMPILERHDR double nfa2dfatime = 0.0;
COMPILERHDR double dfamintimetime = 0.0;

/*!
* read rules from a file
* then process the rules to CSnortRule
* callback function RECIEVER to handle CSnortRule
*/
COMPILERHDR void ParseRuleFile(const char *pFileName, RECIEVER recv, void *lpUser)
{
	typedef std::vector<std::string>	STRINGVEC;
	typedef STRINGVEC::iterator			STRINGVEC_ITER;
	if(recv == NULL)
	{
		throw 0;
	}

	std::ofstream fCompileError("CompileError.txt");
	std::ofstream fParseError("ParseError.txt");
	std::ofstream fSuccess("Sucess.txt");
	std::ofstream fPcreError("PcreError.txt");
	std::ofstream fOptionError("OptionError.txt");
	std::ofstream fHasByte("HasByte.txt");
	std::ofstream fHasNot("HasNot.txt");
	std::ofstream fEmpty("Empty.txt");
	std::ofstream fHasNoSig("HasNotSig.txt");
	std::ofstream fExceedLimit("ExceedLimit.txt");

	STRINGVEC rules;
	if(0 == LoadFile(pFileName, rules))
	{
		if(!rules.empty())
		{
			for(STRINGVEC_ITER i = rules.begin(); i != rules.end(); ++i)
			{
				g_log << "Compiling: " << i - rules.begin() + 1 << g_log.nl;
				std::string ruleStr = *i;
				STRING_ITER iBra = std::find(i->begin(), i->end(), '(');
				if (iBra == i->end())
				{
					g_log << "Compile error!" << g_log.nl;
					fCompileError << ruleStr << std::endl;
					continue;
					//TTHROW(TI_INVALIDDATA);
				}
				i->erase(i->begin(), iBra + 1);

				iBra = std::find(i->rbegin(), i->rend(), ')').base();
				if (iBra == i->rend().base())
				{
					g_log << "Compile error!" << g_log.nl;
					fCompileError << ruleStr << std::endl;
					continue;
					//TTHROW(TI_INVALIDDATA);
				}
				i->erase(iBra - 1, i->end());

				CSnortRule snortRule;
				try
				{
					ParseOptions(*i, snortRule);
				}
				catch (CTrace &e)
				{
					g_log << "ParseOptions error: " << e.What() << g_log.nl;
					fParseError << ruleStr << std::endl;
					continue;
					//throw;
				}
				PARSERESULT pr;
				pr.ulSid = snortRule.GetSid();
				pr.ulFlag = PARSEFLAG::PARSE_SUCCESS;
				if (snortRule.GetFlags() == CSnortRule::NORMAL)
				{
					try
					{
						Rule2RegRule(snortRule, pr.regRule);
					}
					catch (CTrace &e)
					{
						pr.ulFlag |= PARSEFLAG::PARSE_ERROR;
						g_log << "Rule2RegRule error: " << e.What() << g_log.nl;
					}
				}
				if (snortRule.GetFlags() & CSnortRule::HASBYTE)
				{
					pr.ulFlag |= PARSEFLAG::PARSE_HASBYTE;
				}
				if (snortRule.GetFlags() & CSnortRule::HASNOT)
				{
					pr.ulFlag |= PARSEFLAG::PARSE_HASNOT;
				}
				if (snortRule.Size() == 0)
				{
					pr.ulFlag |= PARSEFLAG::PARSE_EMPTY;
				}

				ulong nr = recv(pr, lpUser);

				if (nr == COMPILEDINFO::RES_SUCCESS)
				{
					fSuccess << ruleStr << std::endl;
				}

				if (nr & COMPILEDINFO::RES_PCREERROR)
				{
					fPcreError << ruleStr << std::endl;
				}

				if (nr & COMPILEDINFO::RES_OPTIONERROR)
				{
					fOptionError << ruleStr << std::endl;
				}

				if (nr & COMPILEDINFO::RES_HASBYTE)
				{
					fHasByte << ruleStr << std::endl;
				}

				if (nr & COMPILEDINFO::RES_HASNOT)
				{
					fHasNot << ruleStr << std::endl;
				}

				if (nr & COMPILEDINFO::RES_EMPTY)
				{
					fEmpty << ruleStr << std::endl;
				}

				if (nr & COMPILEDINFO::RES_HASNOSIG)
				{
					fHasNoSig << ruleStr << std::endl;
				}

				if (nr & COMPILEDINFO::RES_EXCEEDLIMIT)
				{
					fExceedLimit << ruleStr << std::endl;
				}
			}
		}
	}

	fCompileError.close();
	fParseError.close();
	fSuccess.close();
	fPcreError.close();
	fOptionError.close();
	fHasByte.close();
	fHasNot.close();
	fEmpty.close();
	fHasNoSig.close();
	fExceedLimit.close();
}

COMPILERHDR void CompileRuleFile(const char *pFileName, CCompileResults &compRes)
{
	ParseRuleFile(pFileName, CompileCallback, &compRes);
}

COMPILERHDR void ExtractSequence(const CByteArray &pcResult,
								 std::vector<CByteArray> &strs)
{

	struct MYGET
	{
		const CByteArray *pCode;
		MYGET(const CByteArray &pcResult) : pCode(&pcResult) {}
		ulong operator()(ulong ulIdx)
		{
			return (((*pCode)[ulIdx + 1] << 8) | (*pCode)[ulIdx + 2]);
		}
	} myget(pcResult);

	ulong cur = 0;
	ulong bra = 0;
	ulong times = 0;
	byte temp = 0;
	byte curCode = pcResult[0];
	CByteArray str;
	for (;;)
	{
		switch(curCode)
		{
		case OP_END:
			if(!str.Empty())
			{
				strs.push_back(str);
			}
			str.Clear();
			return;

			//这是什么情况？
		case OP_ALT:				/* 113 Start of alternation */
			strs.clear();
			return;

		case OP_CHARI:				/* 30 Match one character: caselessly */
		case OP_CHAR:				/* 29 Match one character: casefully */
			str.PushBack(pcResult[cur +1]);
			cur = cur + Steps[curCode];
			curCode = pcResult[cur];
			break;

		case OP_CBRA:				/* 127 Start of capturing bracket */
			bra = cur;
			bra += myget(bra);
			if(pcResult[bra] == OP_ALT)
			{
				if(!str.Empty())
				{
					strs.push_back(str);
				}
				str.Clear();
				while(pcResult[bra] == OP_ALT)
				{
					bra += myget(bra);
				}

				cur = bra + Steps[pcResult[bra]];
				curCode = pcResult[cur];

			}
			else
			{
				cur += Steps[curCode];
				curCode = pcResult[cur];
			}
			break;

		case OP_POSPLUS:			/* 43 Possessified plus: caseful */
		case OP_POSPLUSI:
			str.PushBack(pcResult[cur + 1]);
			strs.push_back(str);

			str.Clear();
			str.PushBack(pcResult[cur + 1]);
			
			cur = cur + Steps[curCode];
			curCode = pcResult[cur];
			break;

		case OP_KET:				/* 114 End of group that doesn't have an unbounded repeat */
			cur = cur + Steps[curCode];
			curCode = pcResult[cur];
			break;

		case OP_EXACT:				/* 41 Exactly n matches */
		case OP_EXACTI:				/* 54 */

			times = myget(cur);
			for(ulong i = 0; i < times; ++i)
			{
				str.PushBack(pcResult[cur + 3]);
			}
			temp = pcResult[cur + Steps[curCode]];
			if(((temp == OP_UPTO) || (temp == OP_MINUPTO) || (temp == OP_UPTOI) || (temp == OP_MINUPTOI)) 
				&& (pcResult[cur + 3] == pcResult[cur + Steps[curCode] + 3]))
			{
				if(!str.Empty() && (str.Size() >= 4))
				{
					strs.push_back(str);
				}
				str.Clear();
				for(ulong i = 0; i < times; ++i)
				{
					str.PushBack(pcResult[cur + 3]);
				}
				cur = cur + Steps[curCode] + Steps[OP_UPTO];
				curCode = pcResult[cur];
			}
			else
			{
				cur = cur + Steps[curCode];
				curCode = pcResult[cur];
			}
			break;

		case OP_PLUS:				/* 35 the minimizing one second. */
			str.PushBack(pcResult[cur + 1]);
			strs.push_back(str);
			str.Clear();
			str.PushBack(pcResult[cur + 1]);

			cur = cur + Steps[curCode];
			curCode = pcResult[cur];
			break;

		case OP_BRAZERO:			/* 140 These two must remain together and in this */

			if(!str.Empty())
			{
				strs.push_back(str);
			}
			str.Clear();

			bra = cur + Steps[curCode];
			bra += myget(bra);
			while(pcResult[bra] == OP_ALT)
			{
				bra += myget(bra);
			}

			bra += Steps[OP_KET];
			cur = bra;
			curCode = pcResult[cur];

			break;

		default:
			cur = cur + Steps[curCode];
			curCode = pcResult[cur];
			if (!str.Empty())
			{
				strs.push_back(str);
			}
			str.Clear();
			break;
		}
	}
}

COMPILERHDR void ExtractSignatures(const CByteArray &strs, CUnsignedArray &sigs)
{
	long ulLen = long(strs.Size()) - 3;
	for (long j = 0; j < ulLen; ++j)
	{
		ulong nCurSig = 0;
		for (ulong k = 0; k < 4; ++k)
		{
			((byte*)&nCurSig)[k] = byte(tolower(strs[j + k]));
		}
		sigs.PushBack(nCurSig);
	}
}

COMPILERHDR void CodeToNFA(const CByteArray &pcResult, bool bFromBeg, CNfa &nfa)
{
	BYTEARY codeAry(pcResult.Data(), pcResult.Data() + pcResult.Size());

	BYTEARY_ITER Beg, End;
	Beg = codeAry.begin();
	End = codeAry.end();
	if (!CanProcess(Beg, End))
	{
		TTHROW(TI_UNSUPPORT);
	}
	Beg = codeAry.begin();
	End = codeAry.end();

	if (!bFromBeg)
	{
		ulong nLastRow = nfa.Size();
		nfa.PushBack(CNfaRow());
		CNfaRow &row = nfa.Back();
		for (ulong i = 0; i < EMPTY; ++i)
		{
			row[i].PushBack(nLastRow);
		}
		row[EMPTY].PushBack(nLastRow + 1);
	}

	ProcessPcre(Beg, End, nfa);
}