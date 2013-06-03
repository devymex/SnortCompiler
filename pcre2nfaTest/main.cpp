#include <iostream>
#include <fstream>
#include <string>
#include "../pcre2nfa/pcre2nfa.h"
#include "../pcre2nfa/match.h"

void ReadPcres(std::vector<std::string> &vecPcres)
{
	std::ifstream fin("C:\\test\\Pcres.txt");
	std::string tmp;
	while (std::getline(fin, tmp))
	{
		vecPcres.push_back(tmp);
	}
}

void main()
{
	std::string Regex = "/a\\x00b/";
	char src [] = {'a', 0, 'b'};
	
	int length = 3;
	int Pos = -1;
	bool bMatch = match(src, length, Regex, Pos);
	std::cout << bMatch << std::endl;

	//const char* a = "/mine\\s(and|or)/";
	//CNfa nfa;
	//CRegChain regchain;
	//PcreToNFA(a, nfa, regchain);
	//for (size_t i = 0; i < nfa.Size(); ++i)
	//{
	//	const CNfaRow &row = nfa[i];
	//	for (size_t j = 0; j < CHARSETSIZE; ++j)
	//	{
	//		size_t nCnt = row.DestCnt(j);
	//		for (size_t k = 0; k < nCnt; ++k)
	//		{
	//			std::cout << "(" << i << "," << j << "," << row.GetDest(j, k) << ")" << std::endl;
	//		}
	//	}
	//}
	//std::vector<std::string> vecPcres;
	//ReadPcres(vecPcres);
	//std::ofstream foutExceed("c:\\Exceed.txt");
	//std::ofstream foutError("c:\\Error.txt");
	//for (std::vector<std::string>::iterator i = vecPcres.begin(); i != vecPcres.end(); ++i)
	//{
	//	CNfa nfa;
	//	CRegChain regchain;
	//	size_t res = PcreToNFA(i->c_str(), nfa, regchain);
	//	if (res == SC_EXCEED)
	//	{
	//		foutExceed << i - vecPcres.begin() + 1 << std::endl;
	//	}
	//	if (res == SC_ERROR)
	//	{
	//		foutError << i - vecPcres.begin() + 1 << std::endl;
	//	}
	//	std::cout << i - vecPcres.begin() + 1 << std::endl;	
	//}
	//foutExceed.close();
	//foutError.close();

	system("pause");
}
