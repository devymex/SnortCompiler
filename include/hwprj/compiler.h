/*!
* @file				compiler.h
* @author			Lab 435, Xidian University
* @brief			定义了编译器接口的相关结构体和函数。
* @copyright	本项目开发组版权所有。未经许可，不得复制、更改、编译、分发。
*/

#pragma once

#include <hwprj\compres.h>
#include <vector>

#ifndef COMPILERHDR_DS
#define COMPILERHDR __declspec(dllimport)
#else
#define COMPILERHDR __declspec(dllexport)
#endif

/*! @addtogroup groupCompiler
*  @{
*/

/*!
* @brief		定义了解析SNORT规则时的标记。
*/
struct PARSEFLAG {
	typedef ulong TYPE;

	/// 解析成功。
	static const TYPE PARSE_SUCCESS		= 0;

	/// 规则包含byte_test或byte_jump选项，无法处理。
	static const TYPE PARSE_HASBYTE		= (1 << 0);

	/// 规则包含'!'（逻辑非）的选项，无法处理。
	static const TYPE PARSE_HASNOT		= (1 << 1);

	/// 规则解析出错。
	static const TYPE PARSE_ERROR		= (1 << 2);

	/// 规则不含任何可编译的选项。
	static const TYPE PARSE_EMPTY		= (1 << 3);
};


/*!
* @brief		解析SNORT规则后产生的结果数据结构。
*/
struct PARSERESULT
{
	/// @brief		规则解析后产生的结构化规则对象。
	CRegRule regRule;

	/// @brief		该规则的sid。
	ulong ulSid;

	/// @brief		解析该规则时产生的标记。
	/// @see		@ref PARSEFLAG
	PARSEFLAG::TYPE ulFlag;
};

/// @brief		解析规则使用的回调函数。
/// @param[in]	parseRes 解析结果数据。
/// @param[in]	lpUser 调用者指定的自定义参数。
typedef void (__stdcall *RECIEVER)(const PARSERESULT &parseRes, void *lpUser);

/// @brief		解析一个Snort规则文件，并通过回调函数给出结果。
/// @param[in]	pFileName 输入规则文件名。
/// @param[in]	recv 回调函数的地址。
///	@param[in]	lpUser 自定义参数，将被传入回调函数。
COMPILERHDR void ParseRuleFile(const char *pFileName, RECIEVER recv, void *lpUser);

/// @brief		编译一个Snort规则文件，生成编译结果集。
/// @param[in]	pFileName 输入规则文件名。
/// @param[out]	compRes 编译结果集对象。
/// @remark		将规则中的content、uricontent、pcre选项编写为PCRE链将PCRE链编译为DFA，
///				并提取连续4字节长度signature。
COMPILERHDR void CompileRuleFile(const char *pFileName, CCompileResults &compRes);

COMPILERHDR void ExtractSequence(const CByteArray &pcResult, std::vector<CByteArray> &seqAry);

COMPILERHDR void ExtractSignatures(const CByteArray &seqAry, CUnsignedArray &sigs);

/// @brief		利用PCRE选项的预编译数据，编译生成NFA。
/// @param[in]	pcResult PCRE选项的预编译数据。
/// @param[in]	bFromBeg 指定该PCRE是否需要从数据包首部开始匹配。
/// @param[out]	nfa 一个CNfa类型的nfa。
COMPILERHDR void CodeToNFA(const CByteArray &pcResult, bool bFromBeg, CNfa &nfa);

/*!
@}
*/




/// @brief		2013.8.8，分两遍处理正则表达式的算法

#define INTER			0
#define UNION			1 
#define OVER			2
#define CHARTONUM		48

typedef struct {
	/// @brief		构造一个NODE对象
	void Init(ulong size, std::string str = null)
	{
		regex.clear();
		regex = str;
		pChar = new char[size];
		isMeta = new bool[size];
		deepth = new ulong[size];
		deep = 0;
	}

	/// @brief		正则表达式字符串
	std::string regex;

	/// @brief		正则表达式字符串的每一个字符依次放入
	char *pChar;  

	/// @brief		判断对应字符是否为元字符，普通字符为0， 元字符为1
	bool *isMeta;   

	/// @brief		记录对应字符深度
	ulong *deepth; 

	/// @brief		记录当前字符深度
	ulong deep;

}NODE;

/// @brief		将一个正则表达式初始化成一个NODE对象
COMPILERHDR void InitNode(std::string &fstr, NODE &fnode);  

/// @brief		如果正则表达式中有深度为0的"|"，则从该位置分成两组
COMPILERHDR void splitNodeOr( NODE &rnode, ulong divide, std::vector<NODE> &rGroup);

/// @brief		如果正则表达式有深度为0的"*"、"?"字符，则从该位置分成两个
COMPILERHDR void splitNodeStar( NODE &rnode, ulong divide, std::vector<NODE> &rGroup);

/// @brief		如果正则表达式有深度为0的"+"字符，则从该位置分成两个
COMPILERHDR void splitNodePlus( NODE &rnode, ulong divide, std::vector<NODE> &rGroup ); 

/// @brief		如果正则表达式有深度为0的"{"和"}"字符，则从该位置分成两个
COMPILERHDR void splitNodeBrace( NODE &rnode, ulong divide, std::vector<NODE> &rGroup );

/// @brief		如果将正则表达式中有深度为1的小括号且对应的小括号中有深度为1的"|"，则将其从"|"处分成两个
COMPILERHDR void splitNodeBraket( NODE &rnode, ulong divide, std::vector<NODE> &rGroup );

/// @brief		遍历正则表达式，进行分裂操作
COMPILERHDR ulong split( NODE &fnode, std::vector<NODE> &resultGroup);

/// @brief		对两组Signatures求交集
COMPILERHDR CUnsignedArray InterOp(CUnsignedArray &r1, CUnsignedArray &r2);

/// @brief		对两组Signatures求并集
COMPILERHDR CUnsignedArray UnionOp(CUnsignedArray &r1, CUnsignedArray &r2);

/// @brief		对一个初始化成NODE对象的字符串递归的提取Signatures
COMPILERHDR CUnsignedArray ExtrSig(NODE nodeOrigin);

