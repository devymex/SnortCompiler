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
}

byte DenCpressDfa::getClusterNum(byte row)
{

}

CDfaRow& DenCpressDfa::getCurCluster(byte Cluster)
{

}

std::map<byte,STATEID> getDif(byte row)
{
	
}