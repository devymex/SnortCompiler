#include <iostream>
#include <fstream>
#include <string>
#include "../pcre2nfa/pcre2nfa.h"

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
	const char* a = "/BM/";
	CNfa nfa;
	PcreToNFA(a, nfa);
	for (size_t i = 0; i < nfa.Size(); ++i)
	{
		for (size_t j = 0; j < CHARSETSIZE; ++j)
		{
			for (size_t k = 0; k < nfa[i][j].Size(); ++k)
			{
				std::cout << "(" << i << "," << j << "," << nfa[i][j][k] << ")" << std::endl;
			}
		}
	}
	//std::vector<std::string> vecPcres;
	//ReadPcres(vecPcres);
	//std::ofstream foutExceed("D:\\Exceed.txt");
	//std::ofstream foutError("D:\\Error.txt");
	//for (std::vector<std::string>::iterator i = vecPcres.begin(); i != vecPcres.end(); ++i)
	//{
	//	CNfa nfa;
	//	size_t res = PcreToNFA(i->c_str(), nfa);
	//	if (res == SC_EXCEED)
	//	{
	//		foutExceed << i - vecPcres.begin() + 1 << std::endl;
	//	}
	//	if (res == SC_ERROR)
	//	{
	//		foutError << i - vecPcres.begin() + 1 << std::endl;
	//	}
	//	std::cout << i - vecPcres.begin() + 1 << std::endl;
	//	
	//	for (size_t j = 0; j < nfa.Size(); ++j)
	//	{
	//		std::cout << j << ": ";
	//		for (size_t k = 0; k < CHARSETSIZE; ++k)
	//		{
	//			for (size_t l = 0; l < nfa[j][k].Size(); ++l)
	//			{
	//				std::cout << "(" << k << "," << nfa[j][k][l] << ")";
	//			}
	//		}
	//		std::cout << std::endl;
	//	}
	//}
	//foutExceed.close();
	//foutError.close();

	system("pause");
}
