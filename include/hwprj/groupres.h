/*
**	@file		groupres.h
**
**	@author		Lab 435, Xidian University
**
**	@brief		Definition of the CGroupRes class
**
**	log the result of sectionalization,include DFA set,
**	mapping relation between DfaId and sid and group set
**
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
public:
	/*	��������GetDfaTable
	**	���ܣ���ȡDFA����
	*/
	CDfaArray &GetDfaTable();

	/*	��������GetSidDfaIds
	**	���ܣ���ȡ����sid��DfaId�Ķ�Ӧ��ϵ
	*/
	CSidDfaIds &GetSidDfaIds();

	/*	��������GetGroups
	**	���ܣ���ȡ������
	*/
	CGroups & GetGroups();

	const CDfaArray &GetDfaTable() const;
	const CSidDfaIds &GetSidDfaIds() const;
	const CGroups &GetGroups() const;

	/*	��������WriteToFile
	**	������filename-����ļ�·��
	**	���ܣ�������������ļ�
	*/
	ulong WriteToFile(const char *filename);

	/*	��������ReadFromFile
	**	������filename-�����ļ�·��
	**	���ܣ���д���ļ��еķ���������
	*/
	ulong ReadFromFile(const char *filename);
};