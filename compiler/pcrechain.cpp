#include "stdafx.h"
#include <hwprj\pcrechain.h>

SNORTRULEHDR CPcreChain::CPcreChain()
{
	try
	{
		m_pPcreVec = new PCREVEC;
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

SNORTRULEHDR CPcreChain::CPcreChain(const CPcreChain &other)
	: m_sigs(other.m_sigs)
{
	TASSERT(other.m_pPcreVec != null);
	try
	{
		m_pPcreVec = new PCREVEC(*other.m_pPcreVec);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
	this->m_sigs = other.m_sigs;
}

SNORTRULEHDR CPcreChain::~CPcreChain()
{
	delete m_pPcreVec;
}

SNORTRULEHDR CPcreChain& CPcreChain::operator = (const CPcreChain &other)
{
	TASSERT(other.m_pPcreVec != null);
	if (this != &other)
	{
		try
		{
			*this->m_pPcreVec = *other.m_pPcreVec;
		}
		catch (std::exception &e)
		{
			TTHROW(e.what());
		}
		this->m_sigs = other.m_sigs;
	}

	return *this;
}

SNORTRULEHDR CPcreOption& CPcreChain::operator[](ulong nIdx)
{
	return (*m_pPcreVec)[nIdx];
}

SNORTRULEHDR const CPcreOption& CPcreChain::operator[](ulong nIdx) const
{
	return (*m_pPcreVec)[nIdx];
}

SNORTRULEHDR ulong CPcreChain::Size() const
{
	return m_pPcreVec->size();
}

SNORTRULEHDR void CPcreChain::Clear()
{
	m_pPcreVec->clear();
	m_sigs.Clear();
}

SNORTRULEHDR CPcreOption& CPcreChain::Back() const
{
	return m_pPcreVec->back();
}

SNORTRULEHDR void CPcreChain::PushBack(const CPcreOption &pcreStr)
{
	try
	{
		m_pPcreVec->push_back(pcreStr);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

SNORTRULEHDR void CPcreChain::Resize(ulong nSize)
{
	try
	{
		m_pPcreVec->resize(nSize);
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}
}

SNORTRULEHDR CUnsignedArray& CPcreChain::GetSigs()
{
	return m_sigs;
}

SNORTRULEHDR const CUnsignedArray& CPcreChain::GetSigs() const
{
	return m_sigs;
}
