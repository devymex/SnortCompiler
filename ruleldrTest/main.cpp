#include "../ruleldr/parserule.h"
#include <iostream>
//#include <vector>
//#include <windows.h>

//#ifdef _UNICODE
//#define tstring std::wstring
//#define tcout std::wcout
//#else
//#define tstring std::string
//#define tcout std::cout
//#endif
//
//struct FILEVISITOR
//{
//	std::vector<tstring> &m_Files;
//	FILEVISITOR(std::vector<tstring> &files) : m_Files(files) {}
//	void operator()(tstring &strFilePath)
//	{
//		m_Files.push_back(strFilePath);
//	}
//};
//
//template<typename _Fn>
//void VisitFilesInPath(LPCTSTR lpPath, LPCTSTR lpExt, _Fn pfn)
//{
//	tstring strPath(lpPath);
//	if (strPath[strPath.length() -1] != _T('\\'))
//	{
//		strPath += _T("\\");
//	}
//
//	//Find files in specified path
//	WIN32_FIND_DATA wfd;
//	tstring strPattern = strPath + lpExt;
//	HANDLE hFinder = FindFirstFile(strPattern.c_str(), &wfd);
//	if(hFinder != INVALID_HANDLE_VALUE)
//	{
//		pfn(strPath + wfd.cFileName);
//	}
//
//	for(; FindNextFile(hFinder, &wfd);)
//	{
//		pfn(strPath + wfd.cFileName);
//	}
//	FindClose(hFinder);
//}
//
//void CALLBACK* recv(CSnortRule &snortRule)
//{
//	std::cout << snortRule.Size() << std::endl;
//}
//int main(int nArgs, LPCTSTR *pArgs)
void main()
{
	//if(nArgs != 4)
	//{
	//	return 0;
	//}
	//tstring strInPath(pArgs[1]);
	//tstring strExt(pArgs[2]);
	//tstring strOutPath(pArgs[3]);

	//tstring strPath(pArgs[3]);

	//if (strPath.back() != '\\')
	//{
	//	strPath.push_back('\\');
	//}

	//tstring strFileName(pArgs[1]);

	//if (strFileName.back() != '\\')
	//{
	//	strFileName.push_back('\\');
	//}

	//std::vector<tstring> ruleFiles;
	//VisitFilesInPath(strInPath.c_str(), strExt.c_str(), FILEVISITOR(ruleFiles));

	//for(std::vector<tstring>::iterator rIt = ruleFiles.begin();
	//	rIt != ruleFiles.end(); ++rIt)
	//{
	//	ParseRule(*rIt, recv);
	//}

	
	system("pause");
	//return 0;
}