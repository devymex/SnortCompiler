#include <iostream>
#include <tchar.h>
#include <windows.h>
#include "../compiler/compiler.h"

void main()
{
	CRes result;
	compile(_T("..\\..\\input\\test.rules"), result);
	result.WriteToFile(_T("..\\result.cdt") );

	system("pause");
}