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
	STRINGVEC rules;
	if(0 == LoadFile(pFileName, rules))
	{
		if(!rules.empty())
		{
			for(STRINGVEC_ITER i = rules.begin(); i != rules.end(); ++i)
			{
				g_log << "Compiling: " << i - rules.begin() + 1 << g_log.nl;
				STRING_ITER iBra = std::find(i->begin(), i->end(), '(');
				if (iBra == i->end())
				{
					TTHROW(TI_INVALIDDATA);
				}
				i->erase(i->begin(), iBra + 1);

				iBra = std::find(i->rbegin(), i->rend(), ')').base();
				if (iBra == i->rend().base())
				{
					TTHROW(TI_INVALIDDATA);
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
					throw;
				}
				PARSERESULT pr;
				pr.ulSid = snortRule.GetSid();
				pr.ulFlag = PARSEFLAG::PARSE_SUCCESS;
				try
				{
					Rule2RegRule(snortRule, pr.regRule);
				}
				catch (CTrace &e)
				{
					pr.ulFlag |= PARSEFLAG::PARSE_ERROR;
					g_log << "Rule2RegRule error: " << e.What() << g_log.nl;
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
				recv(pr, lpUser);
			}
		}
	}
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
			str.Empty();
			return;

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
			if(((temp == OP_UPTO) || (temp == OP_MINUPTO) || (temp == OP_UPTOI) || (temp == OP_MINUPTOI)) && (pcResult[cur + 3] == pcResult[cur + Steps[curCode] + 3]))
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




// 2013.8.8

/// @brief		将一个正则表达式初始化成一个NODE对象
COMPILERHDR void InitNode(std::string &fstr, NODE &fnode)       
{
	fnode.Init(fstr.size(), fstr);

	for (ulong idx = 0; idx != fstr.size(); )
	{

		/// @brief		遇到转义字符"\"则将后边的所有字符按普通字符处理
		if ('\\' == fstr[idx])
		{
			fnode.pChar[idx] =  fstr[idx];
			fnode.isMeta[idx] = false;
			fnode.deepth[idx] = fnode.deep;
			++idx;
			fnode.pChar[idx] =  fstr[idx];
			if ('x' == fstr[idx] ||'A' == fstr[idx] || 'Z' == fstr[idx] || 'b' == fstr[idx] || 'B' == fstr[idx] || 'd' == fstr[idx] || 
				'D' == fstr[idx] || 'w' == fstr[idx] || 'W' == fstr[idx] || 's' == fstr[idx] || 'S' == fstr[idx] || 
				'f' == fstr[idx] || 't' == fstr[idx] || 'v' == fstr[idx] || 'o' == fstr[idx] || 'c' == fstr[idx])
			{
				fnode.isMeta[idx] = true;
			}
			else
			{
				fnode.isMeta[idx] = false;
			}
			fnode.deepth[idx] = fnode.deep;
			++idx;
		}
		/// @brief		遇到以下字符按元字符处理
		else if ( ']' == fstr[idx] || '+' == fstr[idx] || '*' == fstr[idx] || '?' == fstr[idx]  || '{' == fstr[idx] || '}' == fstr[idx] || '|' == fstr[idx])
		{
			fnode.pChar[idx] = fstr[idx];
			fnode.isMeta[idx] = true;
			fnode.deepth[idx] = fnode.deep;
			++idx;
		}
		/// @brief		遇到"("按元字符处理，并且深度加1
		else if( '(' == fstr[idx] )
		{
			++fnode.deep;
			fnode.pChar[idx] = fstr[idx];
			fnode.isMeta[idx] = true;
			fnode.deepth[idx] = fnode.deep;
			++idx;
		}
		/// @brief		遇到")"按元字符处理，并且深度减1
		else if( ')' == fstr[idx] )
		{
			fnode.pChar[idx] = fstr[idx];
			fnode.isMeta[idx] = true;
			fnode.deepth[idx] = fnode.deep;
			--fnode.deep;
			++idx;
		}
		/// @brief		遇到除去以上字符意外的字符，按普通字符处理
		else
		{
			fnode.pChar[idx] = fstr[idx];
			fnode.isMeta[idx] = false;
			fnode.deepth[idx] = fnode.deep;
			++idx;
		}
	}
}


/// @brief		如果正则表达式中有深度为0的"|"，则从该位置分成两组
COMPILERHDR void splitNodeOr( NODE &rnode, ulong divide, std::vector<NODE> &rGroup)
{
	NODE node;
	std::string strPrev;
	std::string strNext;

	strPrev.assign(rnode.regex.begin(), rnode.regex.begin() + divide);
	InitNode(strPrev, node);
	rGroup.push_back(node);

	strNext.assign(rnode.regex.begin() + divide + 1, rnode.regex.end());
	InitNode(strNext,node);
	rGroup.push_back(node);
}


/// @brief		如果正则表达式有深度为0的"*"、"?"字符，则从该位置分成两个
COMPILERHDR void splitNodeStar( NODE &rnode, ulong divide, std::vector<NODE> &rGroup)
{
	NODE node;
	std::string strPrev;
	std::string strNext;

	if(')'!= rnode.regex[divide - 1] && false == rnode.isMeta[divide - 1])
	{
		strPrev.assign(rnode.regex.begin(), rnode.regex.begin() + divide - 1);
		InitNode(strPrev, node);
		rGroup.push_back(node);

		strNext.assign(rnode.regex.begin() + divide + 1, rnode.regex.end());
		if (!strNext.empty())
		{
			InitNode(strNext,node);
			rGroup.push_back(node);
		}
	}
	else if (')' == rnode.regex[divide - 1] && true == rnode.isMeta[divide - 1])
	{
		ulong flag = 1;                                           
		ulong preBracket = 0;
		for ( ulong idx2 = divide - 1; idx2 != 0; --idx2)
		{
			if ( ')' == rnode.regex[idx2] && true == rnode.isMeta[idx2 - 1])
			{
				++flag;
			}
			else if ( '(' == rnode.regex[idx2] && true == rnode.isMeta[idx2 - 1])
			{
				if (1 == flag)
				{
					preBracket = idx2;
					break;
				}
				--flag;
			}
		}

		strPrev.assign(rnode.regex.begin(), rnode.regex.begin() + preBracket);
		InitNode(strPrev, node);
		rGroup.push_back(node);

		strNext.assign(rnode.regex.begin() + divide + 1, rnode.regex.end());
		if (!strNext.empty())
		{
			InitNode(strNext,node);
			rGroup.push_back(node);
		}
	}
	
}


/// @brief		如果正则表达式有深度为0的"+"字符，则从该位置分成两个
COMPILERHDR void splitNodePlus( NODE &rnode, ulong divide, std::vector<NODE> &rGroup )   // 分开深度为0的+
{
	NODE node;
	std::string strPrev;
	std::string strNext;
	std::string strTemp;

	if ( ')'!= rnode.regex[divide - 1] && false == rnode.isMeta[divide - 1])
	{
		strPrev.assign(rnode.regex.begin(), rnode.regex.begin() + divide);
		InitNode(strPrev, node);
		rGroup.push_back(node);

		strNext += rnode.regex[divide - 1];
		strTemp.assign(rnode.regex.begin() + divide + 1, rnode.regex.end());
		strNext += strTemp;
		InitNode(strNext, node);
		rGroup.push_back(node);
	}
	else if (')' == rnode.regex[divide - 1] && true == rnode.isMeta[divide - 1])
	{
		/// @brief		向前查找与之对应的"("位置，注意小括号的嵌套情况
		ulong flag = 1;                                           
		ulong preBracket = 0;
		for ( ulong idx2 = divide - 1; idx2 != 0; --idx2)
		{
			if ( ')' == rnode.regex[idx2] && true == rnode.isMeta[idx2 - 1] )
			{
				++flag;
			}
			else if ( '(' == rnode.regex[idx2] && true == rnode.isMeta[idx2 - 1] )
			{
				if (1 == flag)
				{
					preBracket = idx2;
					break;
				}
				--flag;
			}
		}

		strTemp.assign(rnode.regex.begin() + preBracket + 1, rnode.regex.begin() + divide);

		strPrev.assign(rnode.regex.begin(), rnode.regex.begin() + preBracket);
		strPrev += strTemp;
		InitNode(strPrev, node);
		rGroup.push_back(node);

		strNext.assign(rnode.regex.begin() + divide + 1, rnode.regex.end());
		
		strNext = strTemp + strNext;
		InitNode(strNext, node);
		rGroup.push_back(node);
	}
}


/// @brief		如果正则表达式有深度为0的"{"和"}"字符，则从该位置分成两个
COMPILERHDR void splitNodeBrace( NODE &rnode, ulong divide, std::vector<NODE> &rGroup )
{
	NODE node;
	std::string strPrev;
	std::string strNext;
	std::string strTemp;

	/// @brief		将字符转化为数字
	ulong timePos;
	for (ulong i = divide + 1; ; ++i)
	{
		timePos = i;
		if ((',' == rnode.regex[i] || '}' == rnode.regex[i]) && true == rnode.isMeta[i])
		{
			break;
		}
	}
	std::string strtime;
	strtime.assign(rnode.regex.begin() + divide + 1, rnode.regex.begin() + timePos + 1);
	ulong times = rnode.regex[divide + 1] - CHARTONUM;             
	for (ulong i = strtime.size() - 1; i >= 0; --i)
	{
		ulong temp = strtime[i] - CHARTONUM;
		
		times += temp * (std::pow(10, strtime.size()- 1 - i));
	}

	/// @brief		寻找下一个'}'的位置
	ulong nextBrace;
	for(ulong idx7 = divide; ;++idx7)
	{
		if ( '}' == rnode.regex[idx7] && true == rnode.isMeta[idx7])
		{
			nextBrace = idx7;
			break;
		}
	}

	/// @brief		如果大括号前不是小括号则进行以下操作
	if (  ')' != rnode.regex[divide -1])
	{
		strPrev.assign(rnode.regex.begin(), rnode.regex.begin() + divide - 1);

		if ((rnode.isMeta[divide - 1]) == false)
		{
			std::string strTemp(K - 1, rnode.regex[divide - 1]);
			strPrev += strTemp;

			strNext.assign(rnode.regex.begin() + nextBrace + 1, rnode.regex.end());
			strNext = strTemp + strNext;
		}
		else
		{
			strNext.assign(rnode.regex.begin() + nextBrace + 1, rnode.regex.end());
		}
		InitNode(strPrev, node);
		rGroup.push_back(node);

		InitNode(strNext, node);
		rGroup.push_back(node);
	}

	// @brief		如果大括号前是小括号则进行以下操作
	else if(true == rnode.isMeta[divide - 1])
	{
		NODE node;

		// @brief		 查找与之匹配的"("，注意嵌套情况
		ulong preBracket;
		ulong flag = 1;
		for (ulong idx9 = divide - 2; ; --idx9 )
		{
			if ( ')' == rnode.regex[idx9] && true == rnode.isMeta[idx9])
			{
				++flag;
			}
			else if ( '(' == rnode.regex[idx9] && true == rnode.isMeta[idx9])
			{
				if (1 == flag)
				{
					preBracket = idx9;
					break;
				}
				--flag;
			}
		}

		strPrev.assign(rnode.regex.begin(), rnode.regex.begin() + preBracket);
		strTemp.assign(rnode.regex.begin() + preBracket, rnode.regex.begin() + divide);
		for (ulong cnt = 0; cnt != times - 1; ++cnt)
		{
			strTemp += strTemp;
		}
		strPrev = strTemp + strPrev;
		InitNode(strPrev, node);
		rGroup.push_back(node);

		strNext.assign(rnode.regex.begin() + nextBrace + 1, rnode.regex.end());
		strNext = strTemp + strNext;
		InitNode(strNext, node);
		rGroup.push_back(node);
	}
}


/// @brief		如果将正则表达式中有深度为1的小括号且对应的小括号中有深度为1的"|"，则将其从"|"处分成两个
COMPILERHDR void splitNodeBraket( NODE &rnode, ulong divide, std::vector<NODE> &rGroup )
{
	NODE node;
	std::string strPrev;
	std::string strNext;
	std::string strTemp1;
	std::string strTemp2;

	// @brief		 查找与之匹配的")"，注意嵌套情况
	ulong flag = 1;
	ulong nextBracket;
	for (ulong idx = divide + 1; ; ++idx)
	{
		if ('(' == rnode.regex[idx])
		{
			++flag;
		}
		else if (')' == rnode.regex[idx])
		{
			if (1 == flag)
			{
				nextBracket = idx;
				break;
			}
			--flag;
		}
	}

	flag = 1;
	ulong idx;
	for (idx = divide + 1; idx != nextBracket ; ++idx)
	{
		if ( '(' == rnode.regex[idx])
		{
			++flag;
		}
		else if ( ')' == rnode.regex[idx])
		{
			--flag;
		}
		else if ( '|' == rnode.regex[idx] && 1 == flag)
		{
			strPrev.assign(rnode.regex.begin(), rnode.regex.begin() + divide);
			strTemp1.assign(rnode.regex.begin() + divide, rnode.regex.begin() + idx);
			strTemp1 += ')';

			strTemp2 += '(';
			std::string str(rnode.regex.begin() + idx + 1, rnode.regex.begin() + nextBracket + 1);
			strTemp2 += str;

			strNext.assign(rnode.regex.begin() + nextBracket + 1, rnode.regex.end());

			std::string strPart1;
			std::string strPart2;
			strPart1 = strPrev + strTemp1 + strNext;
			strPart2 = strPrev +strTemp2 + strNext;

			InitNode(strPart1, node);
			rGroup.push_back(node);

			InitNode(strPart2, node);
			rGroup.push_back(node);
			return;
		}
	}
	if (idx == nextBracket)
	{
		std::string str = rnode.regex;
		str.erase(nextBracket, 1);
		str.erase(divide, 1);
		InitNode(str, node);
		rGroup.push_back(node);
	}
}


/// @brief		遍历正则表达式，进行分裂操作
COMPILERHDR ulong split( NODE &fnode, std::vector<NODE> &resultGroup)
{
	// @brief		 查找深度为0的"|"，如果存在则进行相应处理后返回，否则继续向下执行
	for (ulong idx = 0; idx != fnode.regex.size(); ++idx)
	{
		char ch = fnode.regex[idx];
		bool isM = fnode.isMeta[idx];
		ulong dp = fnode.deepth[idx];
		if( '|' == ch && true == isM && 0 == dp)                       
		{
			splitNodeOr(fnode, idx, resultGroup);
			return INTER;                                                    
		}
	}

	// @brief		查找深度为0的"*"和"?"，如果存在则进行相应处理后返回，否则继续向下执行
	for (ulong idx = 0; idx != fnode.regex.size(); ++idx)
	{
		char ch = fnode.regex[idx];
		bool isM = fnode.isMeta[idx];
		ulong dp = fnode.deepth[idx];
		if( ('?' == ch || '*' == ch) && true == isM && 0 == dp )          
		{
			splitNodeStar(fnode, idx, resultGroup);
			return UNION;                                                     
		}
	}

	// @brief		查找深度为0的"+"，如果存在则进行相应处理后返回，否则继续向下执行
	for (ulong idx = 0; idx != fnode.regex.size(); ++idx)
	{
		char ch = fnode.regex[idx];
		bool isM = fnode.isMeta[idx];
		ulong dp = fnode.deepth[idx];
		if( '+' == ch && true == isM && 0 == dp)                        
		{
			splitNodePlus(fnode, idx, resultGroup);
			return UNION;                                                 

		}
	}

	// @brief		查找深度为0的"{"，如果存在则进行相应处理后返回，否则继续向下执行
	for (ulong idx = 0; idx != fnode.regex.size(); ++idx)
	{
		char ch = fnode.regex[idx];
		bool isM = fnode.isMeta[idx];
		ulong dp = fnode.deepth[idx];
		if( '{' == ch && true == isM && 0 == dp)                         
		{
			splitNodeBrace(fnode, idx, resultGroup);
			return UNION;
		}
	}

	// @brief		查找深度为1的"（"，如果存在则进行相应处理后返回，否则继续向下执行
	for (ulong idx = 0; idx != fnode.regex.size(); ++idx)
	{
		char ch = fnode.regex[idx];
		bool isM = fnode.isMeta[idx];
		ulong dp = fnode.deepth[idx];
		if ( '(' == ch && true == isM && 1 == dp)                                                   
		{
			splitNodeBraket(fnode, idx, resultGroup);
			return INTER;
		}
	}

	// @brief		不存在以上字符，说明该字符串第一遍处理已经完成，返回OVER
	return OVER;
}


/// @brief		对两组Signatures求交集
COMPILERHDR CUnsignedArray InterOp(CUnsignedArray &r1, CUnsignedArray &r2)
{
	CUnsignedArray r;
	ulong idx;
	ulong idx1;
	r1.Unique();
	r2.Unique();
	for (idx = 0; idx != r1.Size(); ++idx)
	{
		for (idx1 = 0; idx1 != r2.Size(); ++idx1)
		{
			if (r1[idx] == r2[idx1])
			{
				r.PushBack(r1[idx]);
			}
		}
	}
	return r;
}


/// @brief		对两组Signatures求并集
COMPILERHDR CUnsignedArray UnionOp(CUnsignedArray &r1, CUnsignedArray &r2)
{
	CUnsignedArray r;
	ulong idx;
	r1.Unique();
	r2.Unique();
	for (idx = 0; idx != r1.Size(); ++idx)
	{
		r.PushBack(r1[idx]);
	}
	for (idx = 0; idx != r2.Size(); ++idx)
	{
		r.PushBack(r2[idx]);
	}
	return r;
}


/// @brief		对一个初始化成NODE对象的字符串递归的提取Signatures
COMPILERHDR CUnsignedArray ExtrSig(NODE nodeOrigin)                   
{
	ulong m;
	CUnsignedArray r;
	CUnsignedArray r1;
	CUnsignedArray r2;
	std::vector<NODE> nodeSplit;
	m = split(nodeOrigin, nodeSplit);

	/// @brief		递归终止条件 
	if (OVER == m)                                                   
	{ 

		/// @brief		根据编译需要，前后要分别要有"/和/"字符, 而在split()操作后对中间截断的位置加入这些字符
		ulong len = nodeOrigin.regex.size();
		if (nodeOrigin.regex[0] != '"'  || nodeOrigin.regex[1] != '/' )
		{
			nodeOrigin.regex.insert(nodeOrigin.regex.begin(), '/');
			nodeOrigin.regex.insert(nodeOrigin.regex.begin(), '"');
		}
		if (nodeOrigin.regex[len - 2] != '/'  || nodeOrigin.regex[len - 1] != '"' )
		{
			nodeOrigin.regex.insert(nodeOrigin.regex.end(), '/');
			nodeOrigin.regex.insert(nodeOrigin.regex.end(), '"');
		}

		CDllString dllstr(nodeOrigin.regex.c_str());                             
		CPcreOption pcreOption;
		pcreOption.FromPattern(dllstr);

		/// @brief		编译pcre并将中间结果放入pcRes
		CByteArray pcRes;                             
		pcreOption.Precompile(pcRes);

		/// @brief		将剔除元字符后的结果放入strResult
		std::vector<CByteArray> strResult;
		ExtractSequence(pcRes, strResult);

		for (std::vector<CByteArray>::iterator iter = strResult.begin(); iter != strResult.end(); ++iter)
		{
			ExtractSignatures(*iter, r);
		}

		// 测试结果
		//char *pChar = (char *)(&r[0]);
		//std::cout << r.Size() << std::endl;

		return r;

	}
	else
	{
		r1 = ExtrSig(nodeSplit[0]);
		if ( 2 == nodeSplit.size() )
		{
			r2 = ExtrSig(nodeSplit[1]);
		}
		if ( INTER == m)
		{
			if (2 == nodeSplit.size())
			{
				r = InterOp(r1, r2);
			}
			else r = r1;
			return r;
		}
		else
		{
			if (2 == nodeSplit.size())
			{
				r = UnionOp(r1, r2);
			}
			else r = r1;
			return r;
		}
	}
}


