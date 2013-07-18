#include "stdafx.h"

#include "densityCpress.h"

DenCpressDfa::DenCpressDfa()
{
}

DenCpressDfa::DenCpressDfa(const DenCpressDfa &other)
{
	//m_bClusterNum = other.m_bClusterNum;
	//m_nDfaID = other.m_nDfaID;
}

DenCpressDfa::~DenCpressDfa()
{
}

DenCpressDfa& DenCpressDfa::operator = (const DenCpressDfa &other)
{
	//m_bClusterNum = other.m_bClusterNum;
	//m_nID = other.m_nID;
	//m_Dif = other.m_Dif;
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