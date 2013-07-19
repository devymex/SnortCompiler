#include "stdafx.h"

#include "densityCpress.h"

DenCpressDfa::DenCpressDfa()
{
}

DenCpressDfa::DenCpressDfa(const DenCpressDfa &other)
{
	m_nDfaID = other.m_nDfaID;
	m_Cluster = other.m_Cluster;
	staId2CluId = other.staId2CluId;
	m_Dif = other.m_Dif;
}

DenCpressDfa::~DenCpressDfa()
{
}

DenCpressDfa& DenCpressDfa::operator = (const DenCpressDfa &other)
{
	m_nDfaID = other.m_nDfaID;
	m_Cluster = other.m_Cluster;
	staId2CluId = other.staId2CluId;
	m_Dif = other.m_Dif;
	return *this;
}

byte DenCpressDfa::getClusterNum(byte row)
{
	return staId2CluId[row];
}

CDfaRow& DenCpressDfa::getCurCluster(byte cluster)
{
	return m_Cluster[cluster];
}

std::map<byte,STATEID>& DenCpressDfa::getDif(byte row)
{
	return m_Dif[row];
}


void DenCpressDfa::SetDfaID(ulong ID)
{
	m_nDfaID = ID;
}

void DenCpressDfa::SetStaID2CluID(STATEID stateID, ushort cluId)
{
	
	staId2CluId.insert(std::make_pair(stateID, cluId));
	
}

void DenCpressDfa::AddCluRow(CClusterRow& cluRow)
{
	m_Cluster.PushBack(cluRow);
}

void DenCpressDfa::SetDif(CClusterRow& coreRow, CDfaRow& dfaRow)
{
	for (ulong index = 0; index < dfaRow.Size(); ++index)
	{
		if (dfaRow[index] != coreRow[index])
		{
			m_Dif[m_nDfaID].insert(std::make_pair(dfaRow[index],index));
		}
	}
}