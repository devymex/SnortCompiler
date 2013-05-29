#include "stdafx.h"
#include "hashmapping.h"
#include "../grouping/grouping.h"

size_t hash(const SIGNATURE &oneSig)
{
	return oneSig % 16001;
}

HASHMAPPINGSC void Mapping()
{

}
