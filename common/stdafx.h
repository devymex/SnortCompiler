/*!
* @file			stdafx.h
* @author		Lab 435, Xidian University
* @brief		common包的预编译头
* @copyright	本项目开发组版权所有。未经许可，不得复制、更改、编译、分发。
*/

/*!
* @defgroup		groupCommon 公用模块
* @brief		定义了公用的基本的数据结构、数据类型以及全局变量等。
*/

#include <Windows.h>
#include <algorithm>
#include <vector>
#include <list>
#include <iostream>
#include <iterator>
#include <fstream>

#define UNSARYHDR_DS
#define DLLSTRHDR_DS
#define TRACEHDR_DS
#define LOGGERHDR_DS
#define BYTEARYHDR_DS

typedef std::string						DLLSTRING;
typedef std::vector<unsigned long>		DLLULONGVEC;
typedef std::vector<class CDllString>	DLLSTRINGARY;
typedef std::ostream					LOGFILE;
typedef std::vector<unsigned char>		DLLBYTEARY;

