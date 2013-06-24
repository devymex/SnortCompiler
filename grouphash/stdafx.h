/*!
* @file			stdafx.h
* @author		Lab 435, Xidian University
* @brief		grouphash����Ԥ����ͷ
* @copyright	����Ŀ�������Ȩ���С�δ����ɣ����ø��ơ����ġ����롢�ַ���
*/

/*!
* @defgroup		groupGrouphash �������ϣģ��
* @remark		ʵ���˷����ϣ������㷨�����ݽṹ��
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
