#include "stdafx.h"
#include "pcre\pcre.h"
#include "p2nmain.h"
#include <hwprj\pcrematch.h>

PCREMATCHHDR bool PcreMatch(const char* src, int length, std::string Regex, int &Pos)
{
	pcre *re;
	const char *error;
	int erroffset;
	int ovector[OVECCOUNT];
	int rc, i;

	int iBegPos;
	int iEndPos;
	std::string Pcre;
	std::string Behind;

	iBegPos = Regex.find("/", 0);
	iEndPos = Regex.find_last_of("/");
	Pcre = Regex.substr(iBegPos + 1, iEndPos - iBegPos - 1);
	Behind = Regex.substr(iEndPos + 1, Regex.find_last_of("\"") - iEndPos - 1);

	int options = 0;
	if (Behind.find("s", 0) != -1)
	{
		options |= PCRE_DOTALL;
	}
	if (Behind.find("m", 0) != -1)
	{
		options |= PCRE_MULTILINE;
	}
	if (Behind.find("i", 0) != -1)
	{
		options |= PCRE_CASELESS;
	}

	const char* pattern = Pcre.c_str();

	//printf("string : %s\n", src);
	//printf("pattern: \"%s\"\n", pattern);

	re = pcre_compile(pattern, options, &error, &erroffset, NULL);

	if (re == NULL)
	{
		//printf("pcre compilation failed at offset %d: %s\n", erroffset, error);
		return false;
	}

	int wscount = 100;
	int *workspace = new int[wscount];
	CHECKALLOC(workspace);

	//rc = pcre_exec(re, NULL, src, length, 0, 0, ovector, OVECCOUNT);
	rc = pcre_dfa_exec(re, NULL,src, length, 0, 0, ovector, OVECCOUNT, workspace, wscount);
	delete []workspace;
	if (rc < 0)
	{
		//if (rc == PCRE_ERROR_NOMATCH) printf("Sorry, no match ...\n");
		//else printf("Matching error %d\n", rc);
		free(re);
		//system("pause");
		return false;
	}

	//printf("\nOK, has matched ...\n\n");

	for (i = 0; i < rc; i++)
	{
		//char *substring_start = src + ovector[2*i];
		//int substring_length = ovector[2*i+1] - ovector[2*i];
		//printf("%2d: %.*s\n", i, substring_length, substring_start);
		Pos = ovector[2*i+1];
	}

	free(re);

	return true;
}