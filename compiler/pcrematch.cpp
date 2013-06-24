#include "stdafx.h"
#include "pcre\pcre.h"
#include "p2nmain.h"
#include <hwprj\pcrematch.h>
#include <hwprj\pcreopt.h>

PCREMATCHHDR bool PcreMatch(const char* src, int length,  CPcreOption &pcreopt, int &Pos)
{
	pcre *re;
	const char *error;
	int erroffset;
	int ovector[OVECCOUNT];
	int rc, i;

	int options = 0;
	if (pcreopt.HasFlags(CPcreOption::PF_s))
	{
		options |= PCRE_DOTALL;
	}
	if (pcreopt.HasFlags(CPcreOption::PF_m))
	{
		options |= PCRE_MULTILINE;
	}
	if (pcreopt.HasFlags(CPcreOption::PF_i))
	{
		options |= PCRE_CASELESS;
	}
	if(pcreopt.HasFlags(CPcreOption::PF_x))
	{
		options |= PCRE_EXTENDED;
	}
	const char* pattern = pcreopt.GetPcreString().Data();

	//printf("string : %s\n", src);
	//printf("pattern: \"%s\"\n", pattern);

	re = pcre_compile(pattern, options, &error, &erroffset, null);

	if (re == null)
	{
		//printf("pcre compilation failed at offset %d: %s\n", erroffset, error);
		return false;
	}

	int wscount = 10000;
	int *workspace = null;
	try
	{
		workspace = new int[wscount];
	}
	catch (std::exception &e)
	{
		TTHROW(e.what());
	}

	//rc = pcre_exec(re, NULL, src, length, 0, 0, ovector, OVECCOUNT);
	rc = pcre_dfa_exec(re, null,src, length, 0, 0, ovector, OVECCOUNT, workspace, wscount);
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