/*!
* @file				compiler.h
* @author			Lab 435, Xidian University
* @brief			�����˱������ӿڵ���ؽṹ��ͺ�����
* @copyright	����Ŀ�������Ȩ���С�δ����ɣ����ø��ơ����ġ����롢�ַ���
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
* @brief		�����˽���SNORT����ʱ�ı�ǡ�
*/
struct PARSEFLAG {
	typedef ulong TYPE;

	/// �����ɹ���
	static const TYPE PARSE_SUCCESS		= 0;

	/// �������byte_test��byte_jumpѡ��޷�����
	static const TYPE PARSE_HASBYTE		= (1 << 0);

	/// �������'!'���߼��ǣ���ѡ��޷�����
	static const TYPE PARSE_HASNOT		= (1 << 1);

	/// �����������
	static const TYPE PARSE_ERROR		= (1 << 2);

	/// ���򲻺��κοɱ����ѡ�
	static const TYPE PARSE_EMPTY		= (1 << 3);
};


/*!
* @brief		����SNORT���������Ľ�����ݽṹ��
*/
struct PARSERESULT
{
	/// @brief		�������������Ľṹ���������
	CRegRule regRule;

	/// @brief		�ù����sid��
	ulong ulSid;

	/// @brief		�����ù���ʱ�����ı�ǡ�
	/// @see		@ref PARSEFLAG
	PARSEFLAG::TYPE ulFlag;
};

/// @brief		��������ʹ�õĻص�������
/// @param[in]	parseRes ����������ݡ�
/// @param[in]	lpUser ������ָ�����Զ��������
typedef void (__stdcall *RECIEVER)(const PARSERESULT &parseRes, void *lpUser);

/// @brief		����һ��Snort�����ļ�����ͨ���ص��������������
/// @param[in]	pFileName ��������ļ�����
/// @param[in]	recv �ص������ĵ�ַ��
///	@param[in]	lpUser �Զ����������������ص�������
COMPILERHDR void ParseRuleFile(const char *pFileName, RECIEVER recv, void *lpUser);

/// @brief		����һ��Snort�����ļ������ɱ���������
/// @param[in]	pFileName ��������ļ�����
/// @param[out]	compRes ������������
/// @remark		�������е�content��uricontent��pcreѡ���дΪPCRE����PCRE������ΪDFA��
///				����ȡ����4�ֽڳ���signature��
COMPILERHDR void CompileRuleFile(const char *pFileName, CCompileResults &compRes);

COMPILERHDR void ExtractSequence(const CByteArray &pcResult, std::vector<CByteArray> &seqAry);

COMPILERHDR void ExtractSignatures(const CByteArray &seqAry, CUnsignedArray &sigs);

/// @brief		����PCREѡ���Ԥ�������ݣ���������NFA��
/// @param[in]	pcResult PCREѡ���Ԥ�������ݡ�
/// @param[in]	bFromBeg ָ����PCRE�Ƿ���Ҫ�����ݰ��ײ���ʼƥ�䡣
/// @param[out]	nfa һ��CNfa���͵�nfa��
COMPILERHDR void CodeToNFA(const CByteArray &pcResult, bool bFromBeg, CNfa &nfa);

/*!
@}
*/




/// @brief		2013.8.8�������鴦��������ʽ���㷨

#define INTER			0
#define UNION			1 
#define OVER			2
#define CHARTONUM		48

typedef struct {
	/// @brief		����һ��NODE����
	void Init(ulong size, std::string str = null)
	{
		regex.clear();
		regex = str;
		pChar = new char[size];
		isMeta = new bool[size];
		deepth = new ulong[size];
		deep = 0;
	}

	/// @brief		������ʽ�ַ���
	std::string regex;

	/// @brief		������ʽ�ַ�����ÿһ���ַ����η���
	char *pChar;  

	/// @brief		�ж϶�Ӧ�ַ��Ƿ�ΪԪ�ַ�����ͨ�ַ�Ϊ0�� Ԫ�ַ�Ϊ1
	bool *isMeta;   

	/// @brief		��¼��Ӧ�ַ����
	ulong *deepth; 

	/// @brief		��¼��ǰ�ַ����
	ulong deep;

}NODE;

/// @brief		��һ��������ʽ��ʼ����һ��NODE����
COMPILERHDR void InitNode(std::string &fstr, NODE &fnode);  

/// @brief		���������ʽ�������Ϊ0��"|"����Ӹ�λ�÷ֳ�����
COMPILERHDR void splitNodeOr( NODE &rnode, ulong divide, std::vector<NODE> &rGroup);

/// @brief		���������ʽ�����Ϊ0��"*"��"?"�ַ�����Ӹ�λ�÷ֳ�����
COMPILERHDR void splitNodeStar( NODE &rnode, ulong divide, std::vector<NODE> &rGroup);

/// @brief		���������ʽ�����Ϊ0��"+"�ַ�����Ӹ�λ�÷ֳ�����
COMPILERHDR void splitNodePlus( NODE &rnode, ulong divide, std::vector<NODE> &rGroup ); 

/// @brief		���������ʽ�����Ϊ0��"{"��"}"�ַ�����Ӹ�λ�÷ֳ�����
COMPILERHDR void splitNodeBrace( NODE &rnode, ulong divide, std::vector<NODE> &rGroup );

/// @brief		�����������ʽ�������Ϊ1��С�����Ҷ�Ӧ��С�����������Ϊ1��"|"�������"|"���ֳ�����
COMPILERHDR void splitNodeBraket( NODE &rnode, ulong divide, std::vector<NODE> &rGroup );

/// @brief		����������ʽ�����з��Ѳ���
COMPILERHDR ulong split( NODE &fnode, std::vector<NODE> &resultGroup);

/// @brief		������Signatures�󽻼�
COMPILERHDR CUnsignedArray InterOp(CUnsignedArray &r1, CUnsignedArray &r2);

/// @brief		������Signatures�󲢼�
COMPILERHDR CUnsignedArray UnionOp(CUnsignedArray &r1, CUnsignedArray &r2);

/// @brief		��һ����ʼ����NODE������ַ����ݹ����ȡSignatures
COMPILERHDR CUnsignedArray ExtrSig(NODE nodeOrigin);

