#pragma once
#include <hwprj\common.h>

#ifndef TRACEHDR_DS
#define TRACEHDR __declspec(dllimport)
#else
#define TRACEHDR __declspec(dllexport)
#endif

class TRACEHDR CTrace
{
public:
	CTrace(pcstr pFile, ulong ulLine, pcstr pMsg)
		: m_pFile(pFile), m_pMsg(pMsg), m_ulLine(ulLine)
	{
	}
	ulong Line()
	{
		return m_ulLine;
	}
	pcstr What()
	{
		return m_pFile;
	}
	pcstr File()
	{
		return m_pFile;
	}

public:

protected:
	pcstr m_pFile;
	pcstr m_pMsg;
	ulong m_ulLine;
};

TRACEHDR extern pcstr TI_BADALLOC;
TRACEHDR extern pcstr TI_NOTFOUND;
TRACEHDR extern pcstr TI_OUTOFRANGE;
#define TTHROW(m) throw CTrace(__FILE__, __LINE__, m);


#ifdef _DEBUG
#define TASSERT(s)	if (!(s)) TTHROW("!("#s")");
#define TVERIFY(s)	if (!(s)) TTHROW("!("#s")");
#else
#define TASSERT(s)	;
#define TVERIFY(s)	(s);
#endif


