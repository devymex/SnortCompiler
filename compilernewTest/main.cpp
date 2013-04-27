#include <iostream>
#include <tchar.h>
#include <windows.h>
#include <fstream>
#include "../common/common.h"
#include "../dfanew/dfanew.h"
#include "../compilernew/compilernew.h"

void main()
{
	CResNew result;
	//result.ReadFromFile(_T("..\\..\\output\\result.cdt"));

	//CResNew result;
	compilenew(_T("..\\..\\input\\allrules.rule"), result);
	result.WriteToFile(_T("..\\..\\output\\result.cdt"));
	std::ofstream fout("..\\..\\output\\Statistic.txt");
	CDfaTblNew &dfaTbl = result.GetDfaTable();
	for (size_t i = 0; i < dfaTbl.Size(); ++i)
	{
		if (dfaTbl[i].Size() == 0)
		{
			fout << 255 << " " << 0 << " " << 0 << " " << size_t(-1) << " " << 0 << " " << 0 << std::endl;
		}
		else
		{
			fout << (size_t)dfaTbl[i].Size() << " " << dfaTbl[i].GetGroupCount() << " ";
			BYTE tmp[256] = {0};
			for (size_t j = 0; j < dfaTbl[i].Size(); ++j)
			{
				for (size_t k = 0; k < dfaTbl[i].GetGroupCount(); ++k)
				{
					++tmp[dfaTbl[i][j][k]];
				}
			}
			size_t max = 0;
			size_t maxId = -1;
			for (size_t j = 0; j < 256; ++j)
			{
				if (tmp[j] > max)
				{
					max = tmp[j];
					maxId = j;
				}
			}
			size_t total = dfaTbl[i].Size() * dfaTbl[i].GetGroupCount();
			fout << total << " " << maxId << " " << max << " " << max / double(total) << std::endl;
		}
	}
	fout.close();
	fout.clear();
	system("pause");
}
