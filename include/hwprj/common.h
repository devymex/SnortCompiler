/*!
* @file			common.h
* @author		Lab 435, Xidian University
* @brief		Դ�����ĵ���ͨ�û����������͵Ķ��壬�����Ķ���ȡ�
* @copyright	����Ŀ�������Ȩ���С�δ����ɣ����ø��ơ����ġ����롢�ַ���
*/

#pragma once

/*!
* @mainpage ���˵����
*
* @section copy_sec �汾����
* @b SnortCompiler�����Դ�����Լ�����ĵ��İ�Ȩ�鱾��Ŀ�����У���δ����ɵ�����£�
* @b �κ��˲������κ���ʽ���ơ����ġ�����ͷַ��������Դ������κ�һ�����֡�

* @section intro_sec �������
* ������Զ�̬���ӿ����ʽ�ṩ��Snort���򼯵Ľ��������롢����͹�ϣ��������ݽṹ���㷨��
*
* @section module_sec ʹ�÷���

* @subsection requirement һ��Ҫ��
* @li ����ϵͳ��ֻ����Microsoft Windows XP�����ϰ汾�Ĳ���ϵͳ���б�������С�
* @li ����������ֻ����Microsoft Visual Studio 2012����������ִ�б��롣
* @li ����Ҫ������Ҫ�����پ���2.0GHz���ϵ����봦�������Լ�1GiB���ϵĿ����ڴ�ռ�
* @li �洢Ҫ��֧�ֽ��������д��Ӳ�̣����豣֤Ӳ�̵�ʣ��ռ������Ҫд������ݴ�С��

* @subsection directory ����Ŀ¼�ṹ
* <div class="generic_text_mapping_table">
* <table>
* <tr>
* <th align="left">.\\include</th>
* <td>������ṩ�Ľӿ�ͷ�ļ���</td>
* </tr>
* <tr>
* <th align="left">.\\bin</th>
* <td>������������ɵĶ������ļ�������.dll��.lib�ļ���</td>
* </tr>
* <tr>
* <th align="left">.\\common</th>
* <td>@ref groupCommon Դ���롣</td>
* </tr>
* <tr>
* <th align="left">.\\automaton</th>
* <td>@ref groupAutomaton Դ���롣</td>
* </tr>
* <tr>
* <th align="left">.\\compiler</th>
* <td>@ref groupCompiler Դ���롣</td>
* </tr>
* <tr>
* <th align="left">.\\grouphash</th>
* <td>@ref groupGrouphash Դ���롣</td>
* </tr>
* </table>
* </div>
*
* @subsection compile ��������
* ��Visual Studio 2012�д򿪽�������ļ���λ��.\\SnortCompiler.sln��ֱ�ӱ��뼴�ɡ�
*
* @subsection call �ġ�����
* 1. ������ṩ��.\\includeĿ¼��Ϊ��������İ���Ŀ¼��
* 2. ������ṩ��.\\libĿ¼��Ϊ��������Ŀ�Ŀ¼��
* 3. �ڵ��ó���������֮�󣬽�����ṩ������*.dll�ļ����Ƶ����ó���Ĺ���·���С�
*
* @section documentation �ĵ�
* Ŀǰ�ѻ�����ɽӿ��ࡢ�������ꡢȫ�ֱ��������Ͷ�����ĵ���д��ʵ�ֲ��ֵ��ĵ����ڱ�д�У�
* ʵ�ֲ�����ʹ�õ��㷨����������ĵ���ʽ�ṩ��@n
* 
*/

/*!
* @addtogroup groupCommon
* @{
*/

/*!
* ͨ�û�����������
*/
typedef unsigned char			byte;
typedef unsigned long			ulong;
typedef unsigned short			ushort;
typedef char*					pstr;
typedef const char*				pcstr;
typedef ushort					STATEID;
typedef ulong					DFAID;
typedef ulong					SIGNATURE;

/*!
* ȫ�ֳ���
*/
#define SC_MAXDFASIZE			255
#define SC_DFACOLCNT			256
#define SC_STATELIMIT			500
#define SC_CHAINRESERV			8
#define SC_NFAROWRESERV			500

#ifndef null
#define null					0
#endif

/*!
@}
*/
