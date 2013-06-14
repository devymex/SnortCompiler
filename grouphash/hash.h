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
