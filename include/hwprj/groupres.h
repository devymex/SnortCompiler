/*!
* @file			groupres.h
* @author		Lab 435, Xidian University
* @brief		������CGroupRes�࣬��¼���������ݽ��������㷨
*/

#pragma once

#include <hwprj\common.h>
#include <hwprj\dfaarray.h>
#include <hwprj\siddfaids.h>
#include <hwprj\groups.h>

#ifndef GROUPRESHDR_DS
#define GROUPRESHDR __declspec(dllimport)
#else
#define GROUPRESHDR __declspec(dllexport)
#endif

class GROUPRESHDR CGroupRes
{
protected:
	CDfaArray m_dfaTbl;
	CSidDfaIds m_sidDfaIds; 
	CGroups m_groups;
	ulong nBucketCnt;
public:
	/// @brief		��ȡDFA����
	CDfaArray &GetDfaTable();

	/// @brief		��ȡDFA���ϣ�������
	const CDfaArray &GetDfaTable() const;

	/// @brief		��ȡ����sid��DfaId�Ķ�Ӧ��ϵ
	CSidDfaIds &GetSidDfaIds();

	/// @brief		��ȡ����sid��DfaId�Ķ�Ӧ��ϵ��������
	const CSidDfaIds &GetSidDfaIds() const;

	/// @brief		��ȡ������
	CGroups & GetGroups();

	/// @brief		��ȡ��������������
	const CGroups &GetGroups() const;

	/// @brief		��ȡhash����
	ulong &GetBucketCnt();

	/// @brief		��ȡhash������������
	const ulong &GetBucketCnt() const;

	/// @brief		����hash����,���ڰ�hash����д���ļ�
	/// @param		nBucketSize hash����
	void SetBucketCnt(ulong nBucketSize);
	
	/// @brief		������������ļ�
	/// @param		filename ����ļ�·��
	ulong WriteToFile(const char *filename);

	/// @brief		��д���ļ��еķ���������
	/// @param		filename �����ļ�·��
	ulong ReadFromFile(const char *filename);
};