/*!
* @file			pcrematch.h
* @author		Lab 435, Xidian University
* @brief		������PCREƥ�亯����
* @copyright	����Ŀ�������Ȩ���С�δ����ɣ����ø��ơ����ġ����롢�ַ���
*/

#pragma once

#include <hwprj\pcreopt.h>

#ifndef PCREMATCHHDR_DS
#define PCREMATCHHDR __declspec(dllimport)
#else
#define PCREMATCHHDR __declspec(dllexport)
#endif

/*! @addtogroup groupCompiler
*  @{
*/

/*!
* @brief		�ж�PCRE�����ݰ��Ƿ�ƥ��
* @param[in]	src �������ݰ��������ֽ���
* @param[in]	length ���ݰ�����
* @param[in]	Regex ����PCRE
* @param[out]	Pos ƥ��λ��
* @retval		true PCRE�����ݰ�ƥ��ɹ�
* @retval		false PCRE�����ݰ�ƥ��ʧ��
*/
PCREMATCHHDR bool PcreMatch(const char* src,
							int length,
							CPcreOption &pcreopt,
							int &Pos);

/*!
@}
*/
