#include "stdafx.h"

struct _HASH
{
	enum {MAX_SIZE = 20000};
	size_t operator()(const _int64 li)
	{
		return li % MAX_SIZE;
	}
};