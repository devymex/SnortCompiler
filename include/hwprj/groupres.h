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
	/*	函数名：GetDfaTable
	**	功能：获取DFA集合
	*/
	CDfaArray &GetDfaTable();

	/*	函数名：GetSidDfaIds
	**	功能：获取规则sid与DfaId的对应关系
	*/
	CSidDfaIds &GetSidDfaIds();

	/*	函数名：GetGroups
	**	功能：获取分组结果
	*/
	CGroups & GetGroups();

	const CDfaArray &GetDfaTable() const;
	const CSidDfaIds &GetSidDfaIds() const;
	const CGroups &GetGroups() const;

	/*	函数名：WriteToFile
	**	参数：filename-输出文件路径
	**	功能：将分组结果输出文件
	*/
	ulong WriteToFile(const char *filename);

	/*	函数名：ReadFromFile
	**	参数：filename-输入文件路径
	**	功能：将写入文件中的分组结果读入
	*/
	ulong ReadFromFile(const char *filename);
};