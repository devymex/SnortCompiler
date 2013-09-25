/*!
* @file			buildhash.h
* @author		Lab 435, Xidian University
* @brief		������GROUPHASH�࣬�洢�������͹�ϣ���ӳ���ϵ��
* @copyright	����Ŀ�������Ȩ���С�δ����ɣ����ø��ơ����ġ����롢�ַ���
*/

#pragma once

#include <vector>
#include <map>
#include <hwprj\common.h>
#include <hwprj\groupres.h>

#ifndef HASHMAPHDR_DS
#define HASHMAPHDR __declspec(dllimport)
#else
#define HASHMAPHDR __declspec(dllexport)
#endif

/*! @addtogroup groupGrouphash
*  @{
*/

/*!
* @brief		hash��Ľ����Ϣ��
* @remark		�������õķ�����õ�Signature�ͷ���ϲ����DFA��
*/

struct HASHNODE
{
	SIGNATURE m_sig;
	ulong m_nDfaId;
};

/*!
* @brief		��װhash�����ࡣ
*/

struct SIGHASH
{
	ulong nBucketCnt;
	ulong operator()(const SIGNATURE &oneSig)
	{
		//const ulong _FNV_offset_basis = 2166136261U;
		//const ulong _FNV_prime = 16777619U;

		//ulong _Val = _FNV_offset_basis;
		//_Val ^= oneSig;
		//_Val *= _FNV_prime;

		//return (_Val % nBucketCnt);

		return (oneSig % nBucketCnt);
	}
};


HASHMAPHDR extern SIGHASH hash;

typedef std::map<ulong, std::vector<HASHNODE>> HASHRES;

/// @brief		ÿ����������ܹ���������Signature(s)��Ϊÿ������ѡ��
///				��������Signature��������ϣ��
/// @param[in]	groupRes ������
/// @param[in]	HashResMap ��ϣӳ��Ľ��
HASHMAPHDR void HashMapping(CGroupRes &groupRes, HASHRES &HashResMap);

/*!
@}
*/
