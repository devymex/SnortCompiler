/*!
* @file			stdafx.h
* @author		Lab 435, Xidian University
* @brief		grouphash包的预编译头
* @copyright	本项目开发组版权所有。未经许可，不得复制、更改、编译、分发。
*/

/*!
* @defgroup		groupGrouphash 分组与哈希模块
* @remark		实现了分组哈希的相关算法和数据结构。
*/

#include <tchar.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <deque>
#include <fstream>
#include <map>
#include <windows.h>
#include <numeric>
#include <string>
#include <sstream>
#include <unordered_map>

#include <hwprj\trace.h>
#include <hwprj\ctimer.h>
#include <hwprj\dfa.h>
#include <hwprj\dfaarray.h>
#include <hwprj\compres.h>
#include <hwprj\logger.h>

#define GROUPSHDR_DS
#define GROUPRESHDR_DS
#define GROUPINGHDR_DS

#define HASHMAPHDR_DS

typedef std::vector<struct ONEGROUP> GROUPVEC;
