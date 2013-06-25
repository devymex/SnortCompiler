/*!*
* @file				unsary.h
**
* @author			Lab 435, Xidian University
**
* @brief			impletation of the CUnsignedArray class
**
*/

#include "stdafx.h"

#include <hwprj\unsary.h>
#include <hwprj\trace.h>

UNSARYHDR void CUnsignedArray::CopyFrom(ulong *pBuf)
{
	if (m_ulSize == 1)
	{
		*m_pAry = *pBuf;
	}
	else
	{
		memcpy(m_pAry, pBuf, m_ulSize * sizeof(m_pAry[0]));
	}
}

UNSARYHDR void CUnsignedArray::Sort()
{
	std::sort(m_pAry, m_pAry + m_ulSize);
}

UNSARYHDR void CUnsignedArray::Unique()
{
	Sort();
	m_ulSize = std::unique(m_pAry, m_pAry + m_ulSize) - m_pAry;
}
