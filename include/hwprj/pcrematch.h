/*!
* @file		pcrematch.h
**
* @author		Lab 435, Xidian University
**
* @brief			Match the packet and pcre library
**
*/

#pragma once

#ifndef PCREMATCHHDR_DS
#define PCREMATCHHDR __declspec(dllimport)
#else
#define PCREMATCHHDR __declspec(dllexport)
#endif

/// @brief		判断PCRE与数据包是否匹配，返回true或者false
/// @param	src-输入数据包二进制字节流，length-数据包长度，Regex-输入PCRE，Pos-匹配位置
PCREMATCHHDR bool PcreMatch(const char* src, int length, std::string Regex, int &Pos);
