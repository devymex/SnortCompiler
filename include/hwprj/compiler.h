/*!
* @file				compiler.h
**
* @author			Lab 435, Xidian University
**
* @brief			Primary file of compiler
**
* Functions declaration of access enter
**
*/

#pragma once

#include <hwprj\compres.h>

#ifndef COMPILERHDR_DS
#define COMPILERHDR __declspec(dllimport)
#else
#define COMPILERHDR __declspec(dllexport)
#endif

/*! @addtogroup groupCompiler
*  @{
*/

/*!
* @brief		�����˽���SNORT����ʱ�ı��
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
* @brief		����SNORT���������Ľ�����ݽṹ
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
/// @param		pFileName ��������ļ�����
/// @param		recv �ص������ĵ�ַ��
///	@param		lpUser �Զ����������������ص�������
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
