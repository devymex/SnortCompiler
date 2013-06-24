/*!
* @file			common.h
* @author		Lab 435, Xidian University
* @brief		源代码文档。通用基本数据类型的定义，常量的定义等。
* @copyright	本项目开发组版权所有。未经许可，不得复制、更改、编译、分发。
*/

#pragma once

/*!
* @mainpage 软件说明书
*
* @section copy_sec 版本声明
* @b SnortCompiler软件、源代码以及相关文档的版权归本项目组所有，在未经许可的情况下，
* @b 任何人不得以任何形式复制、更改、编译和分发本软件及源代码的任何一个部分。

* @section intro_sec 软件介绍
* 本软件以动态链接库的形式提供了Snort规则集的解析、编译、分组和哈希的相关数据结构和算法。
*
* @section module_sec 使用方法

* @subsection requirement 一、要求
* @li 操作系统：只能在Microsoft Windows XP及以上版本的操作系统中中编译和运行。
* @li 开发环境：只能在Microsoft Visual Studio 2012开发环境中执行编译。
* @li 运行要求：运行要求至少具有2.0GHz以上的中央处理器，以及1GiB以上的可用内存空间
* @li 存储要求：支持将结果数据写入硬盘，但需保证硬盘的剩余空间大于需要写入的数据大小。

* @subsection directory 二、目录结构
* <div class="generic_text_mapping_table">
* <table>
* <tr>
* <th align="left">.\\include</th>
* <td>软件包提供的接口头文件。</td>
* </tr>
* <tr>
* <th align="left">.\\bin</th>
* <td>软件包编译生成的二进制文件，包括.dll和.lib文件。</td>
* </tr>
* <tr>
* <th align="left">.\\common</th>
* <td>@ref groupCommon 源代码。</td>
* </tr>
* <tr>
* <th align="left">.\\automaton</th>
* <td>@ref groupAutomaton 源代码。</td>
* </tr>
* <tr>
* <th align="left">.\\compiler</th>
* <td>@ref groupCompiler 源代码。</td>
* </tr>
* <tr>
* <th align="left">.\\grouphash</th>
* <td>@ref groupGrouphash 源代码。</td>
* </tr>
* </table>
* </div>
*
* @subsection compile 三、编译
* 在Visual Studio 2012中打开解决方案文件（位于.\\SnortCompiler.sln）直接编译即可。
*
* @subsection call 四、调用
* 1. 将软件提供的.\\include目录作为解决方案的包含目录。
* 2. 将软件提供的.\\lib目录作为解决方案的库目录。
* 3. 在调用程序编译完成之后，将软件提供的所有*.dll文件复制到调用程序的工作路径中。
*
* @section documentation 文档
* 目前已基本完成接口类、函数、宏、全局变量、类型定义的文档编写。实现部分的文档正在编写中，
* 实现部分所使用的算法将以另外的文档形式提供。@n
* 
*/

/*!
* @addtogroup groupCommon
* @{
*/

/*!
* 通用基本数据类型
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
* 全局常量
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
