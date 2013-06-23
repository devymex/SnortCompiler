/*!
* @file		dfaarray.h
**
* @author		Lab 435, Xidian University
**
* @brief			Declaration of the CDfaArray class
**
* Store multiple CDfa
**
*/

#pragma once

#include <hwprj\dfa.h>

#ifndef DFAARYHDR_DS
class CDFAVEC;
#define DFAARYHDR __declspec(dllimport)
#else
#define DFAARYHDR __declspec(dllexport)
#endif

class DFAARYHDR CDfaArray
{
public:
	CDfaArray();
	CDfaArray(const CDfaArray& other);
	virtual ~CDfaArray();

	CDfaArray&		operator =	(const CDfaArray &other);
	CDfa&			operator []	(ulong ulIdx);
	const CDfa&		operator []	(ulong ulIdx) const;

	ulong			Size() const;
	void			Clear();
	void			Reserve(ulong ulCount);
	void			Resize(ulong ulSize);
	void			PushBack(const CDfa &dfa);

protected:
	CDFAVEC *m_pdfaTbl;
};
