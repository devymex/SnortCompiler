/*!
* @file			stdafx.h
* @author		Lab 435, Xidian University
* @brief		common����Ԥ����ͷ
* @copyright	����Ŀ�������Ȩ���С�δ����ɣ����ø��ơ����ġ����롢�ַ���
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
