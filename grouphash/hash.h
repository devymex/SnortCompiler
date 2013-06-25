/*!
* @file			groups.h
* @author		Lab 435, Xidian University
* @brief		定义了GROUPHASH的数据结构
* @copyright	本项目开发组版权所有。未经许可，不得复制、更改、编译、分发。
*/

#include <map>
#include <unordered_map>
#include <hwprj\common.h>

typedef std::map<SIGNATURE, std::vector<ulong>> SIGNATUREMAP;
typedef std::map<ulong, std::vector<SIGNATURE>> IDMAP;
typedef std::unordered_map<ulong, std::vector<ulong>> RESULTMAP;

struct GROUPHASH
{
	std::vector<SIGNATURE> vecSigs;
	SIGNATURE currSig;
	ulong mergeDfaId;
	std::vector<ulong> vecDfaIds;
};
