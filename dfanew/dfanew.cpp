#include "stdafx.h"
#include "dfanew.h"
#include "CreDfa.h"

DFANEWSC CDfanew::CDfanew()
{
	m_pDfa = new std::vector<CDfaRow>;
	m_TermSet = new std::vector<TERMSET>;
}

DFANEWSC CDfanew::~CDfanew()
{
	delete m_pDfa;
	delete m_TermSet;
}

DFANEWSC CDfanew::CDfanew(const CDfanew &other)
{
	m_pDfa = new std::vector<CDfaRow>;
	m_TermSet = new std::vector<TERMSET>;
	*this = other;
}

DFANEWSC CDfanew& CDfanew::operator=(const CDfanew &other)
{
	Clear();
	m_nId = other.m_nId;
	m_nColNum = other.m_nColNum;
	m_StartId = other.m_StartId;
	CopyMemory(m_pGroup, other.m_pGroup, DFACOLSIZE * sizeof(BYTE));
	*m_pDfa = *other.m_pDfa;
	*m_TermSet = *other.m_TermSet;
	return *this;
}

DFANEWSC STATEID CDfanew::Size() const
{
	return (STATEID)m_pDfa->size();
}

DFANEWSC CDfaRow& CDfanew::operator[](STATEID index)
{
	return (*m_pDfa)[index];
}

DFANEWSC const CDfaRow& CDfanew::operator[](STATEID index) const
{
	return (*m_pDfa)[index];
}

DFANEWSC void CDfanew::Init(BYTE *pGroup)
{
	Clear();
	std::vector<BYTE> tmpGroup;
	std::copy(pGroup, pGroup + DFACOLSIZE, std::back_inserter(tmpGroup));
	std::sort(tmpGroup.begin(), tmpGroup.end());
	tmpGroup.erase(std::unique(tmpGroup.begin(), tmpGroup.end()), tmpGroup.end());
	if (tmpGroup.back() == tmpGroup.size() - 1)
	{
		m_nColNum = tmpGroup.size();
		CopyMemory(m_pGroup, pGroup, DFACOLSIZE * sizeof(BYTE));
	}
	else
	{
		std::cerr << "Group error!" << std::endl;
	}
}

DFANEWSC void CDfanew::Clear()
{
	
}

DFANEWSC size_t CDfanew::FromNFA(CNfa &nfa, NFALOG *nfalog, size_t Count)
{
	return 0;
}

DFANEWSC size_t CDfanew::Minimize(CDfanew &oneDfaTab, CDfanew &minDfaTab)
{
		//error: DFA is empty 
	if (oneDfaTab.Size() == 0)
	{
		return size_t(-1);
	}

	// Extract terminal states from a dfa, initialize partition with terminal states and normal states
	std::list<std::list<STATEID>> Partition(1);
	std::list<STATEID> FinalStas;
	for (STATEID i = 0; i < oneDfaTab.Size(); ++i)
	{
		if ((oneDfaTab[i].GetFlag() & oneDfaTab[i].TERMINAL) != 0)
		{
			FinalStas.push_back(i);
			Partition.push_front(std::list<STATEID>());
			Partition.front().push_back(i);
		}
		else
		{
			Partition.back().push_back(i);
		}
	}

	//error: terminal states or normal states are empty
	if (Partition.size() == 1 || Partition.back().empty())
	{
		return size_t(-2);
	}

	//generate positive traverse table
	size_t row = oneDfaTab.Size();
	size_t col = oneDfaTab.GetColNum();
	std::vector<STATEID> *pPosTab = new std::vector<STATEID>[row * col];
	for (STATEID i = 0; i < row; ++i)
	{
		for (STATEID j = 0; j < col; ++j)
		{
			STATEID nDest = (STATEID)oneDfaTab[i][j];
			if (nDest != STATEID(-1))
			{
				pPosTab[i * col + j].push_back(nDest);
			}
		}
	}

	std::vector<BYTE> reachable(oneDfaTab.Size(), 0);
	std::list<STATEID> StartStas;
	StartStas.push_back(0);

	//record states that will be visited from positive traverse table
	RemoveUnreachable(pPosTab, StartStas, col, reachable);
	delete []pPosTab;

	//generate reverse traverse table	
	std::vector<STATEID> *pRevTab = new std::vector<STATEID>[row * col];
	for (STATEID i = 0; i < row; ++i)
	{
		for (STATEID j = 0; j < col; ++j)
		{
			STATEID nDest = (STATEID)oneDfaTab[i][j];
			if (nDest != STATEID(-1))
			{
				pRevTab[nDest * col + j].push_back(STATEID(i));
			}
		}
	}

	//record states that will be visited from reverse traverse table
	RemoveUnreachable(pRevTab, FinalStas, col, reachable);

	//remove unreachable states, generate new DFA
	CDfa tmpDfa;
	MergeReachable(oneDfaTab, reachable, tmpDfa);

	//divide nondistinguishable states
	PartitionNonDisState(tmpDfa, pRevTab, Partition);

	//DFA minization
	MergeNonDisStates(tmpDfa, Partition, minDfaTab);

	minDfaTab.SetId(oneDfaTab.GetId());

	for (STATEID j = 0; j < minDfaTab.Size(); ++j)
	{
		std::cout << (int)j << ": ";
		for (STATEID k = 0; k < minDfaTab.GetColNum(); ++k)
		{
			if(minDfaTab[j][k] != STATEID(-1))
			{
				std::cout << "(" << (int)k << "," << (int)minDfaTab[j][k]<< ")";
			}

		}
		std::cout << std::endl;
	}


	delete []pRevTab;
	return 0;
}

DFANEWSC size_t CDfanew::GetGroupCount() const
{
	return m_nColNum;
}

DFANEWSC BYTE CDfanew::Char2Group(BYTE nIdx)
{
	return m_pGroup[nIdx];
}

DFANEWSC const BYTE* CDfanew::GetGroup() const
{
	return m_pGroup;
}

DFANEWSC STATEID CDfanew::GetStartId() const
{
	return m_StartId;
}

DFANEWSC void CDfanew::SetId(size_t id)
{
	m_nId = id;
}

DFANEWSC size_t CDfanew::GetId()
{
	return m_nId;
}

DFANEWSC size_t CDfanew::Process(BYTE *ByteStream, size_t len, CStateSet &StaSet)
{
	return 0;
}

DFANEWSC void CDfanew::GetAcceptedId(STATEID id, CVectorNumber &dfaIds)
{
	
}
