/*!
* @file				grouping.h
* @author			Lab 435, Xidian University
* @brief			�����˷��麯��Grouping��
* @copyright		����Ŀ�������Ȩ���С�δ����ɣ����ø��ơ����ġ����롢�ַ���
*/

#pragma once

#include <hwprj\compres.h>
#include <hwprj\groupres.h>

#ifndef GROUPINGHDR_DS
#define GROUPINGHDR __declspec(dllimport)
#else
#define GROUPINGHDR __declspec(dllexport)
#endif

/*! @addtogroup groupGrouphash
*  @{
*/
/// @brief		��DFA���з��飬������ͬsignature��DFA��Ϊһ�顣
/// @param[in]	res ������PCRE������ɹ���DFA����һЩ������Ϣ(����ÿ��DFA��Signature��)��
/// @param[out]	groupRes ��������
GROUPINGHDR void Grouping(class CCompileResults &res, CGroupRes &groupRes);

/*!
@}
*/
