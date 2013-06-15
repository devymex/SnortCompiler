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
	static pcstr TI_BADALLOC;

protected:
	pcstr m_pFile;
	pcstr m_pMsg;
	ulong m_ulLine;
};

#ifdef _DEBUG
#define SCASSERT(s)	if (!(s)) throw CTrace(__FILE__, __LINE__, "!("#s")");
#define SCVERIFY(s)	if (!(s)) throw CTrace(__FILE__, __LINE__, "!("#s")");
#else
#define SCASSERT(s)	;
#define SCVERIFY(s)	(s);
#endif