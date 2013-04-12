#pragma once
#define CHARSETSIZE 260

#ifndef COMMON_H_
#define COMMONSC __declspec(dllimport)
#else
#define COMMONSC __declspec(dllexport)
#endif

class COMMONSC CNfaRow
{
public:
	CNfaRow();
	~CNfaRow();
	CNfaRow(const CNfaRow &other);
	CNfaRow& operator=(const CNfaRow &other);
	size_t GetSetSize(size_t nChar);
	void GetSet(size_t nChar, size_t *pSet);
	std::vector<size_t> &operator[](size_t nChar);
private:
	std::vector<size_t> *m_pDestSet[CHARSETSIZE];
};

class COMMONSC CNfa
{
public:
	CNfa();
	~CNfa();
	CNfa(const CNfa &other);
	CNfa& operator=(const CNfa &other);
	size_t GetRowNum(void);
	void reserve(size_t _Count);
	void resize(size_t _Newsize);
	size_t size() const;
	CNfaRow &back();
	CNfaRow &operator[](size_t index);
	void pop_back();
private:
	std::vector<CNfaRow> *m_pNfa;
};
