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
* @addtogroup groupCommon
* @{
*/

/*!
* @brief	�쳣�ࡣ
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

	/// @brief		��÷����쳣�Ĵ����кš�
	ulong Line()
	{
		return m_ulLine;
	}

	/// @brief		����쳣��Ϣ�ַ�����
	pcstr What()
	{
		return m_pMsg;
	}

	/// @brief		��÷����쳣�Ĵ����ļ�����
	pcstr File()
	{
		return m_pFile;
	}

private:
	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	pcstr m_pFile;

	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	pcstr m_pMsg;

	/// @brief		˽�г�Ա�������ڲ�ʹ�á�
	ulong m_ulLine;
};


/// @breif		ͨ���쳣��Ϣ�ַ����ĺ궨�塣
TRACEHDR extern pcstr TI_BADALLOC;		/// �ڴ�������
TRACEHDR extern pcstr TI_NOTFOUND;		/// ָ��������δ�ҵ���
TRACEHDR extern pcstr TI_OUTOFRANGE;	/// Խ����ʡ�
TRACEHDR extern pcstr TI_INVALIDDATA;	/// ��������ݴ���
TRACEHDR extern pcstr TI_BADPARAM;		/// ��������
TRACEHDR extern pcstr TI_UNSUPPORT;		/// �ò�����δ��֧�֡�
TRACEHDR extern pcstr TI_NOTALLOW;		/// �ò����ѱ���ֹ��
TRACEHDR extern pcstr TI_INCOMPATIBLE;	/// ָ�������ݲ����ݡ�

/// @brief		�׳�һ��CTrace�쳣��������ǰ�Ĵ����ļ����к��Լ�������Ϣ��
/// @param		m �쳣��Ϣ�ַ������ɴ�ͨ���쳣��Ϣ�ַ����б���ѡ��
#define TTHROW(m) throw CTrace(__FILE__, __LINE__, m);

#ifdef _DEBUG
/// @brief		���Ժ�
/// @param		s ������ʽ
/// @remark		��sΪ�������κβ����������׳���sΪ��Ϣ�ַ������쳣��
#define TASSERT(s)	if (!(s)) {TTHROW("!("#s")");}
/// @brief		��Ϻ�
/// @param		s ������ʽ
/// @remark		��sΪ�������κβ����������׳���sΪ��Ϣ�ַ������쳣��
#define TVERIFY(s)	if (!(s)) {TTHROW("!("#s")");}
#else
/// @brief		���Ժꡣ
/// @param		s ������ʽ��
/// @remark		��Debugģʽ�²����κβ�����
#define TASSERT(s)	;
/// @brief		���Ժꡣ
/// @param		s ������ʽ��
/// @remark		��Debugģʽ�½�ִ��s��
#define TVERIFY(s)	(s);
#endif

/*!
@}
*/
