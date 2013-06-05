#include "../MatchPacket/MatchPkt.h"
#include "../PMatchPkt/PMatchPkt.h"
#include <fstream>
#include <string>

#include <iostream>
#include <tchar.h>

void GetMchRule(std::vector<u_char> &pkt, REGRULESMAP &rulesmap, std::vector<size_t> &rules)
{
	SIGSMAP sigmap = rulesmap.sigmap;
	SIGNATURE sig;
	u_char csig[4];
	for(std::vector<u_char>::iterator iter = pkt.begin(); iter + 3 != pkt.end(); ++iter)
	{
		for(size_t i = 0; i < 4; ++i)
		{
			csig[i] = tolower(*(iter + i));
		}
		sig = *(SIGNATURE *)csig;
		if(sigmap.count(sig))
		{
			rules.insert(rules.end(), sigmap[sig].begin(), sigmap[sig].end());
		}
	}
}

void HandleOneFile(std::string &oneFile, REGRULESMAP &rulesmap, std::ofstream &matchresult)
{
	std::vector<std::vector<u_char>> oneFilePkt;
	LoadCapFile(oneFile.c_str(), &oneFilePkt);

	//u_char p[] = {0, 0, 'f', 'g', 0, 'a', 'b', 'C', 'd', 0, 'd', 'e', 235, 0, 42, 'A', 123, 'B', 40, '1', '2', 93, 63, 'a', 'b', 'c', 'd', 'e', 'a'};
	//std::vector<u_char> tmpvec(p, p + sizeof(p));
	//oneFilePkt.push_back (tmpvec);

	for(std::vector<std::vector<u_char>>::iterator allIter = oneFilePkt.begin(); allIter != oneFilePkt.end(); ++allIter)
	{
		matchresult << allIter - oneFilePkt.begin() << "\t";
		std::vector<size_t> rules;
		GetMchRule(*allIter, rulesmap, rules);
		//std::vector<size_t> matchedSid;//保存匹配上的规则的sid
		for(std::vector<size_t>::iterator rIt = rules.begin(); rIt != rules.end(); ++rIt)
		{
			bool flag = TradithinalMatch(*allIter, rulesmap.result[*rIt].regrule);
			if(flag)
			{
				//匹配成功
				matchresult << rulesmap.result[*rIt].m_nSid << std::endl << "\t";
				//matchedSid.push_back(rulesmap.result[*rIt].m_nSid);
			}
		}

		matchresult << std::endl;
	}
	oneFilePkt.clear();
}

void HandleAllFile(const std::string &path, std::ofstream &matchresult)
{
	REGRULESMAP rulesmap;
	MchCompile(_T("../../input/allrules.rule"), &rulesmap);

	WIN32_FIND_DATAA wfda;
	const std::string ext = "*.*";
	std::string str = path + std::string("\\");
	std::string pat = str + ext;
	HANDLE hff = ::FindFirstFileA(pat.c_str(), &wfda);
	if(hff == INVALID_HANDLE_VALUE)
	{
		return;
	}

	for(BOOL br = TRUE; br == TRUE; br = FindNextFileA(hff, &wfda))
	{
		if(wfda.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if (wfda.cFileName[0] != '.')
			{
				HandleAllFile(str + std::string(wfda.cFileName), matchresult);
			}
		}
		else
		{
			std::string &temp = str + std::string(wfda.cFileName);
			std::string &ext1 = temp.substr(temp.size() - 4, 4);
			if(ext1 == ".cap")
			{
				HandleOneFile(temp, rulesmap, matchresult);
				std::cout << temp << std::endl;
			}
		}
	}

	matchresult.close();
}



void main(size_t argc, char* argv[])
{
	//std::ofstream tempout("E:\\test\\temp.cap", std::ofstream::binary);
	//std::ifstream tempin("E:\\allPkt\\01\\1_71_u_2.cap00_up.cap", std::ifstream::binary);
	//std::string str;

	//for(size_t i = 0; i < 10; ++i)
	//{
	//	char buff[1000];
	//	tempin.read(buff, 1000);
	//	tempout.write(buff, 1000);
	//}

	//tempout.close();
	//tempin.close();

	std::string path = "E:\\allPkt";
	std::ofstream matchresult("E:\\test\\matchresult.txt", std::ofstream::out);
	HandleAllFile(path, matchresult);
}