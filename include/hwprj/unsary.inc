inline CUnsignedArray::CUnsignedArray()
	: m_pAry(null), m_ulSize(0), m_ulCap(0)
{
}

inline CUnsignedArray::CUnsignedArray(const CUnsignedArray &other)
	: m_pAry(null), m_ulSize(0), m_ulCap(0)
{
	m_pAry = Alloc(other.m_ulSize);
	m_ulCap = other.m_ulSize;
	m_ulSize = other.m_ulSize;
	if (m_ulSize != 0)
	{
		CopyFrom(other.m_pAry);
	}
}

inline CUnsignedArray::~CUnsignedArray()
{
	Free(m_pAry);
}

// Overided operators
inline CUnsignedArray& CUnsignedArray::operator = (
	const CUnsignedArray &other)
{
	if (this != &other)
	{
		if (m_ulCap < other.m_ulSize)
		{
			ulong *pNewBuf = Alloc(other.m_ulSize);
			Free(m_pAry);
			m_pAry = pNewBuf;
			m_ulCap = other.m_ulSize;
		}
		m_ulSize = other.m_ulSize;
		if (m_ulSize != 0)
		{
			CopyFrom(other.m_pAry);
		}
	}
	return *this;
}

inline ulong& CUnsignedArray::operator [] (ulong nIdx)
{
	TASSERT(nIdx < m_ulSize);
	return m_pAry[nIdx];
}

inline const ulong& CUnsignedArray::operator [] (ulong nIdx) const
{
	TASSERT(nIdx < m_ulSize);
	return m_pAry[nIdx];
}

inline bool CUnsignedArray::operator == (const CUnsignedArray &other) const
{
	if (m_ulSize != other.m_ulSize)
	{
		return false;
	}
	for (ulong i = 0; i < m_ulSize; ++i)
	{
		if (m_pAry[i] != other.m_pAry[i])
		{
			return false;
		}
	}
	return true;
}

inline bool CUnsignedArray::operator <	(const CUnsignedArray &other) const
{
	ulong ulMinSize = m_ulSize;
	if (ulMinSize > other.m_ulSize)
	{
		ulMinSize = other.m_ulSize;
	
	}
	for (ulong i = 0; i < ulMinSize; ++i)
	{
		if (m_pAry[i] != other.m_pAry[i])
		{
			return m_pAry[i] < other.m_pAry[i];
		}
	}
	return m_ulSize < other.m_ulSize;
}

// Access member
inline ulong CUnsignedArray::Size() const
{
	return m_ulSize;
}

inline ulong& CUnsignedArray::Back()
{
	TASSERT(m_ulSize > 0);
	return m_pAry[(long)m_ulSize - 1];
}

inline ulong* CUnsignedArray::Data()
{
	TASSERT(m_pAry != null);
	return m_pAry;
}

inline const ulong* CUnsignedArray::Data() const
{
	TASSERT(m_pAry != null);
	return m_pAry;
}

inline void CUnsignedArray::Clear()
{
	m_ulSize = 0;
}

inline void CUnsignedArray::PopBack()
{
	if (m_ulSize == 0)
	{
		TTHROW(TI_OUTOFRANGE);
	}
	--m_ulSize;
}

inline void CUnsignedArray::PushBack(ulong ulVal)
{
	TASSERT(m_ulCap >= m_ulSize);
	for (; m_ulCap <= m_ulSize; )
	{
		if (m_ulCap == 0)
		{
			Realloc(1);
		}
		else
		{
			Realloc(m_ulCap * 2);
		}
	}
	m_pAry[m_ulSize] = ulVal;
	++m_ulSize;
}

inline void CUnsignedArray::Reserve(ulong nCap)
{
	if (nCap > m_ulCap)
	{
		Realloc(nCap);
	}
}

inline void CUnsignedArray::Resize(ulong ulSize)
{
	TASSERT(m_ulCap >= m_ulSize);
	if (m_ulCap < ulSize)
	{
		Realloc(ulSize);
	}
	m_ulSize = ulSize;
}

inline void CUnsignedArray::Resize(ulong ulSize, ulong ulVal)
{
	TASSERT(m_ulCap >= m_ulSize);
	if (m_ulCap < ulSize)
	{
		Realloc(ulSize);
	}
	for (ulong i = m_ulSize; i < ulSize; ++i)
	{
		m_pAry[i] = ulVal;
	}
	m_ulSize = ulSize;
}

// Algorithms
inline void CUnsignedArray::Fill(ulong _Val)
{
	for (ulong i = 0; i < m_ulSize; ++i)
	{
		m_pAry[i] = _Val;
	}
}

inline ulong CUnsignedArray::Find(ulong _Val) const
{
	ulong ulIdx = ulong(-1);
	for (ulong i = 0; i < m_ulSize; ++i)
	{
		if (m_pAry[i] == _Val)
		{
			return i;
		}
	}
	return ulIdx;
}

inline ulong* CUnsignedArray::Alloc(ulong ulCap)
{
	ulong *pNewBuf = null;
	if (ulCap != 0)
	{
		pNewBuf = new ulong[ulCap];
	}
	return pNewBuf;
}

inline void CUnsignedArray::Free(ulong *pBuf)
{
	delete []pBuf;
}

inline void CUnsignedArray::Realloc(ulong ulCap)
{
	if (m_ulCap != ulCap)
	{
		ulong *pOldBuf = m_pAry;

		m_pAry = Alloc(ulCap);
		m_ulCap = ulCap;

		if (m_ulSize > m_ulCap)
		{
			m_ulSize = m_ulCap;
		}
		if (m_ulSize != 0)
		{
			CopyFrom(pOldBuf);
		}
		Free(pOldBuf);
	}
}
