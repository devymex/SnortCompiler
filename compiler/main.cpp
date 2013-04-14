#include "stdafx.h"
#include "../ruleldr/parserule.h"

void CALLBACK recv(const CSnortRule &snortRule)
{
}
int _tmain(int nArgc, TCHAR **ppArgs)
{
	ParseRule(ppArgs[1], recv);
	return 0;
}