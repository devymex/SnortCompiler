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
	ulong nSigCnt;

	//FNV Hash Function
	int FNVHash(const unsigned long &oneSig, const int &blockCnt)
	{
		const unsigned long _FNV_offset_basis = 2166136261U;
		const unsigned long _FNV_prime = 16777619U;

		unsigned long hash = _FNV_offset_basis;
		hash ^= oneSig;
		hash *= _FNV_prime;

		return (hash % blockCnt);
	}

	// AP Hash Function
	int APHash(const unsigned long &oneSig, const int &blockCnt)
	{
		unsigned long hash = 0;
		for (int j = 0; j < 4; ++j)
		{
			if ((j & 1) == 0)
			{
				hash ^= ((hash << 7) ^ *(((unsigned char*)&oneSig) + j) ^ (hash >> 3));
			}
			else
			{
				hash ^= (~((hash << 11) ^ *(((unsigned char*)&oneSig) + j) ^ (hash >> 5)));
			}
		}
		hash = hash & 0x7FFFFFFF;
		return (hash % blockCnt);
	}

	int BlockHash(const unsigned long &oneSig, const int sigCnt)
	{
		int blockCnt = 256;
		int sig1block = sigCnt / blockCnt;
		if (sigCnt % blockCnt != 0)
		{
			++sig1block;
		}
		blockCnt = sigCnt / sig1block;
		if (sigCnt % sig1block != 0)
		{
			++blockCnt;
		}
		int slot1block = sig1block * 3;

		int blockIdx = APHash(oneSig, blockCnt);
		int slotIdx = FNVHash(oneSig, slot1block);

		return (blockIdx * slot1block + slotIdx);
	}

	ulong operator()(const SIGNATURE &oneSig)
	{
		//const ulong _FNV_offset_basis = 2166136261U;
		//const ulong _FNV_prime = 16777619U;

		//ulong _Val = _FNV_offset_basis;
		//_Val ^= oneSig;
		//_Val *= _FNV_prime;

		//return (_Val % nBucketCnt);
		return BlockHash(oneSig, nSigCnt);
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
