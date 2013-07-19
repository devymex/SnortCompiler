#pragma once

#ifndef NEWDFA_DS
#define NEWDFA __declspec(dllimport)
#else
#define NEWDFA __declspec(dllexport)
#endif

//class NEWDFA CNEWDFA
//{
//public:
//private:
//	ulong m_nId;
//	ushort m_usColNum;
//	STATEID m_nStartId;
//
//	DFAROWARY* m_pMatrix;
//	CFinalStates m_FinStas;
//	byte m_pColGroup[SC_DFACOLCNT];
//	CUnsignedArray* m_pRowGroup;
//};