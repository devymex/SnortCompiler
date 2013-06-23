/*!
* @file			trace.h
* @author		Lab 435, Xidian University
* @brief		������CTrace�༰�䵼������
* @copyright	����Ŀ�������Ȩ���С�δ����ɣ����ø��ơ����ġ����롢�ַ���
*/

#pragma once
#include <hwprj\common.h>

#ifndef TRACEHDR_DS
#define TRACEHDR __declspec(dllimport)
#else
#define TRACEHDR __declspec(dllexport)
#endif

/*!
* @brief	�쳣��
* @remark	Ӧͨ������ʹ�ô��࣬�÷�����غ궨���˵����
*/
class TRACEHDR CTrace
{
public:
	/// @brief		���캯����
	CTrace(pcstr pFile, ulong ulLine, pcstr pMsg)
		: m_pFile(pFile), m_pMsg(pMsg), m_ulLine(ulLine)
	{
	}
	ulong Line()
	{
		return m_ulLine;
	}
	pcstr What()
	{
		return m_pMsg;
	}
	pcstr File()
	{
		return m_pFile;
	}

private:
	/// @brief		˽�г�Ա�������ڲ�ʹ��
	pcstr m_pFile;

	/// @brief		˽�г�Ա�������ڲ�ʹ��
	pcstr m_pMsg;

	/// @brief		˽�г�Ա�������ڲ�ʹ��
	ulong m_ulLine;
};

TRACEHDR extern pcstr TI_BADALLOC;
TRACEHDR extern pcstr TI_NOTFOUND;
TRACEHDR extern pcstr TI_OUTOFRANGE;
TRACEHDR extern pcstr TI_INVALIDDATA;
TRACEHDR extern pcstr TI_BADPARAM;
TRACEHDR extern pcstr TI_UNSUPPORT;
TRACEHDR extern pcstr TI_NOTALLOW;
TRACEHDR extern pcstr TI_INCOMPATIBLE;

/// @brief		�׳�һ��CTrace�쳣��������ǰ�Ĵ����ļ����к��Լ�������Ϣ��
#define TTHROW(m) throw CTrace(__FILE__, __LINE__, m);

#ifdef _DEBUG
/// @brief		���Ժ�
#define TASSERT(s)	if (!(s)) {TTHROW("!("#s")");}
/// @brief		��Ϻ�
#define TVERIFY(s)	if (!(s)) {TTHROW("!("#s")");}
#else
#define TASSERT(s)	;
#define TVERIFY(s)	(s);
#endif


