#include "../becchi_pro/stdinc.h"
#include "../becchi_pro/parser.h"
#include "../becchi_pro/nfa.h"
#include "../becchi_pro/dfa.h"

#include <hwprj\compiler.h>
#include <hwprj\snortrule.h>
#include <hwprj\nfa.h>

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <tchar.h>
#include <Windows.h>

int VERBOSE = 0;
int DEBUG = 0;
FILE *ruleset;
regex_parser *parser;
int REGEXNUM;
int THRESHOLD;
int groupcnt = 0;


//unsigned long **DFAdata;

static void usage()
{
	fprintf(stderr,"\n");
	fprintf(stderr, "Usage: regex --parse|-p <regex_file> -t <DFA limit> [Options]\n"); 
	fprintf(stderr, "\nOptions:\n");
	fprintf(stderr, "       --debug|-d    enhanced verbosity level\n");
	fprintf(stderr, "\n");
	exit(0);
}

/* configuration */
static struct conf {
	char *regex_file;
	bool debug;
} config;

/* initialize the configuration */
void init_conf(){
	config.regex_file=NULL;
	config.debug=false;
}

/* print the configuration */
void print_conf(){
	fprintf(stderr,"\nCONFIGURATION: \n");
	if (config.regex_file) fprintf(stderr, "- RegEx file: %s\n",config.regex_file);
	if (config.debug) fprintf(stderr,"- debug mode\n");
}

/* parse the main call parameters */
static int parse_arguments(int argc, char **argv)
{
	int i=1;
	if (argc < 2) {
		usage();
		return 0;
	}
	while (i<argc){
		if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--debug") == 0){
			config.debug=true;
		}
		else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--parse") == 0){
			i++;
			if(i==argc){
				fprintf(stderr,"Regular expression file name missing.\n");
				return 0;
			}
			config.regex_file=argv[i];
		}
		else if (strcmp(argv[i], "-t") == 0){
			i++;
			if(i==argc){
				fprintf(stderr,"DFA limit missing.\n");
				return 0;
			}
			THRESHOLD = atoi(argv[i]); 
			//printf(">> DFA limit is %d\n",THRESHOLD);
		}
		else {
			fprintf(stderr,"Ignoring invalid option %s\n",argv[i]);
		}
		i++;
	}
	return 1;
}

/* check that the given file can be read/written */
void check_file(char *filename, char *mode){
	FILE *file=fopen(filename,mode);
	if (file==NULL){
		fprintf(stderr,"Unable to open file %s in %c mode",filename,mode);
		fatal("\n");
	}
	else {
		fclose(file);
	}
}

/*
 * Please write your code HERE !!!
 *
 * You can call the function "unsigned long parse_regex_group(FILE *file, int group[])"
 * to get the number of DFA states corresponding to the given group of regular expressions.
 * The argument "file" is the file of regular expression ruleset, and the argument
 * "group" is the set of regular expression indexes.
 *
 * Note that:
 * group[0] is the number of regular expressions in the group array, and
 * group[1], group[2] ... group[num] are filled with the regular expression indexes.
 *
 * Before each evaluation, the group must be cleared first, and the evaluation
 * of the regular expreesion indexes must strictly start from the group[1].
 *
 */

/* BEGIN USER CODE */
void ReadRegexs(std::string filename, std::vector<std::string> &regset)
{
	std::ifstream fin(filename);

	if (fin.is_open())
	{
		for (std::string re; getline(fin, re); )
		{
			regset.push_back(re);
		}		
	}

	fin.close();
}

void display(CDfa &newdfa)
{
	for(ulong i = 0; i != newdfa.Size(); ++i)
	{
		std::map<STATEID, ulong> rowStateCnt;
		for(ulong j = 0; j != newdfa.GetGroupCount(); ++j)
		{
			rowStateCnt[newdfa[i][j]]++;
		}
		STATEID maxId = 0;
		for (std::map<STATEID, ulong>::iterator j = rowStateCnt.begin(); j != rowStateCnt.end(); ++j)
		{
			if (j->second > rowStateCnt[maxId])
			{
				maxId = j->first;
			}
		}
		std::cout << "state # "<< i << ", maxId: " << (ulong)maxId << ", ";
		for(ulong j = 0; j != newdfa.GetGroupCount(); ++j)
		{
			if (newdfa[i][j] != maxId)
			{
				std::cout << "< " << j << "," <<  (ulong)newdfa[i][j] << " >" ;
			}
		}
		std::cout << std::endl;
	}
}

//
//void PrintDfaToGv(CDfa &newdfa, const char* fileName)
//{
//	std::ofstream fout(fileName);
//	fout << "digraph G {" << std::endl;
//	fout << "S -> " << (ulong)newdfa.GetStartId() << std::endl;
//
//	for(ulong i = 0; i != newdfa.Size(); ++i)
//	{
//		std::map<STATEID, ulong> rowStateCnt;
//		for(ulong j = 0; j != newdfa.GetGroupCount(); ++j)
//		{
//			rowStateCnt[newdfa[i][j]]++;
//		}
//		STATEID maxId = 0;
//		for (std::map<STATEID, ulong>::iterator j = rowStateCnt.begin(); j != rowStateCnt.end(); ++j)
//		{
//			if (j->second > rowStateCnt[maxId])
//			{
//				maxId = j->first;
//			}
//		}
//		//for(ulong j = 0; j != newdfa.GetGroupCount(); ++j)
//		//{
//		//	if (newdfa[i][j] != maxId)
//		//	{
//		//		fout << i << " -> " << (ulong)newdfa[i][j] << " [label=\"" << j << "\"];" << std::endl;
//		//	}
//		//	else if (maxId != (STATEID)-1)
//		//	{
//		//		fout << i << " -> "  << (ulong)maxId << " [label=\"" << j << "\"];" << std::endl;
//		//	}
//		//}
//
//		fout << "s"<< i << ", maxId: " << (ulong)maxId << ", ";	
//		for(ulong j = 0; j != newdfa.GetGroupCount(); ++j)
//		{
//			if (newdfa[i][j] != maxId)
//			{
//				fout << "<" << j << "," <<  (ulong)newdfa[i][j] << "> " ;
//			}
//		}
//		fout << std::endl;
//	}
//	for (ulong i = 0; i < newdfa.Size(); ++i)
//	{
//		if (newdfa[i].GetFlag() & CDfaRow::TERMINAL)
//		{
//			fout << (ulong)i << " [peripheries=2];" << std::endl;
//		}
//	}
//	fout << "}" << std::endl;
//	fout.close();
//}


NFA* CreatNFA(const char* re)
{
	NFA* nfa = new NFA(); 
	NFA *non_anchored = nfa->add_epsilon(); // for .* RegEx
	NFA *anchored = nfa->add_epsilon(); // for anchored RegEx (^)

	parser->parse_re(nfa, re);

	//handle -m modifier
	if (parser->m_modifier && (!anchored->get_epsilon()->empty() || !anchored->get_transitions()->empty())){
		non_anchored->add_transition('\n',anchored);
		non_anchored->add_transition('\r',anchored);
	}

	//delete non_anchored, if necessary
	if (non_anchored->get_epsilon()->empty() && non_anchored->get_transitions()->empty()){
		nfa->get_epsilon()->remove(non_anchored);
		delete non_anchored;
	}
	else{
		non_anchored->add_any(non_anchored);
	}

	return nfa->get_first();
}

struct COMPARECHARGROUP
{
	bool operator()(std::vector<byte> &x, std::vector<byte> &y)
	{
		return (x.front() < y.front());
	}
};

struct COMPARESTATOGROUP
{
	bool operator()(std::vector<byte> &x, std::vector<byte> &y)
	{
		return (x == y);
	}
};

void GetChargroup(CDfa &dfa, STATEID &sta, std::vector<std::vector<byte>> &chargroup)
{
	CDfaRow &cur = dfa[sta];

	std::map<STATEID, std::vector<byte>> sta2charset;
	for (ulong col = 0; col < dfa.GetGroupCount(); ++col)
	{
		sta2charset[cur[col]].push_back(col);
	}

	for (std::map<STATEID, std::vector<byte>>::iterator iMap = sta2charset.begin(); iMap != sta2charset.end(); ++iMap)
	{
		chargroup.push_back(iMap->second);
	}
}

ulong EqualDFA(CDfa &dfa1, std::vector<byte> &visited1, STATEID sta1, CDfa &dfa2, std::vector<byte> &visited2, STATEID sta2)
{
	visited1[sta1] = 1;
	visited2[sta2] = 1;

	std::vector<std::vector<byte>> sta2chargroup1;
	std::vector<std::vector<byte>> sta2chargroup2;

	GetChargroup(dfa1, sta1, sta2chargroup1);
	GetChargroup(dfa2, sta2, sta2chargroup2);

	if (sta2chargroup1.size() == sta2chargroup2.size())
	{
		std::sort(sta2chargroup1.begin(), sta2chargroup1.end(), COMPARECHARGROUP());
		std::sort(sta2chargroup2.begin(), sta2chargroup2.end(), COMPARECHARGROUP());
		bool flag = std::equal(sta2chargroup1.begin(), sta2chargroup1.end(),
			sta2chargroup2.begin(), COMPARESTATOGROUP());
		//std::sort(sta2chargroup1.begin(), sta2chargroup1.end());
		//std::sort(sta2chargroup2.begin(), sta2chargroup2.end());
		//bool flag = std::equal(sta2chargroup1.begin(), sta2chargroup1.end(),
		//	sta2chargroup2.begin());

		if (flag)
		{
			for (ulong i = 0, j = 0; i < sta2chargroup1.size() 
				&& j < sta2chargroup2.size(); ++i, ++j)
			{
				byte char1 = sta2chargroup1[i].front();
				byte char2 = sta2chargroup2[j].front();	
				STATEID cur1 = dfa1[sta1][char1];
				STATEID cur2 = dfa2[sta2][char2];
				if ((cur1 != (STATEID)-1 && visited1[cur1] == 0) 
					&& (cur2 != (STATEID)-1 && visited2[cur2] == 0)
					&& !EqualDFA(dfa1, visited1, cur1, dfa2, visited2, cur2))
				{
					return 0;
				}
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}

	return 1;
}

void FoldDFA(CDfa &curDfa)
{
	CDfa foldDfa;
	byte group[CSIZE];
	for (int i = 0; i < CSIZE; ++i)
	{
		group[i] = (byte)i;
	}

	foldDfa.Init(group);
	foldDfa.Reserve(300);
	foldDfa.SetStartId(curDfa.GetStartId());
	for (ulong i = 0; i < curDfa.Size(); ++i)
	{
		foldDfa.PushBack(CDfaRow(CSIZE));
		for (ulong j = 0; j < CSIZE; ++j)
		{
			byte z = curDfa.Char2Group((byte)j);
			foldDfa[i][j] = curDfa[i][z];
		}
	}

	curDfa = foldDfa;
}

ulong CompDfa(CDfa &OwnDfa, CDfa &BeDfa)
{
	ulong Result = 0;
	std::vector<byte> visited1(OwnDfa.Size());
	std::fill(visited1.begin(), visited1.end(), 0);
	std::vector<byte> visited2(BeDfa.Size());
	std::fill(visited2.begin(), visited2.end(), 0);
	if (OwnDfa.Size() == BeDfa.Size())
	{
		if(EqualDFA(OwnDfa, visited1, OwnDfa.GetStartId(), BeDfa, visited2, BeDfa.GetStartId()))
		{
			Result = 1;
		}
	}
	//else
	//{
	//	std::cout << (ulong)OwnDfa.Size() << ", " << (ulong)BeDfa.Size() << std::endl;
	//}
	return Result;
}

template<typename _Iter>
void FormatPcre (_Iter pBeg, _Iter pEnd, std::string &bPcre, std::string &oPcre)
{
		_Iter iPcreBeg = std::find(pBeg, pEnd, '/');
		_Iter iPcreEnd = pEnd;
		for(; *iPcreEnd != '/'; --iPcreEnd);

		bPcre = std::string(iPcreBeg + 1, iPcreEnd);
		if (bPcre.back() == '$')
		{
			bPcre.pop_back();
			oPcre = std::string(iPcreBeg, iPcreEnd - 1);
			oPcre.push_back('/');
		}
		else
		{
			oPcre = std::string(iPcreBeg, iPcreEnd + 1);
		}
		oPcre.push_back('s');

		std::cout << bPcre << std::endl;
		std::cout << oPcre << std::endl;
}

ulong CompareWithPcre(const char *pPcre)
{
	std::string Pcre1;
	std::string Pcre2;
	FormatPcre(pPcre, pPcre + strlen(pPcre), Pcre1, Pcre2);
	
	const char* bPcre = Pcre1.c_str();
	const char* oPcre = Pcre2.c_str();

	ulong Result = 0;
	//STATEARY tmp;
	//char* str = ":IP ConaaX-Mailer:EBT ReporterbbbSubjecwq:Vic";

	CNfa nfa1;
	CRegChain regChain;
	if (SC_SUCCESS != PcreToNFA(oPcre, nfa1, regChain.GetSigs()))
	{
		return 2;
	}
	//std::cout << nfa1.Size() << std::endl;
	//PrintDfaToText(nfa1, "..//nfaresult1.txt");
	CDfa OwnDfa;
	if (-1 == OwnDfa.FromNFA(nfa1))
	{
		return 3;
	}
	//std::cout << OwnDfa.Size() << std::endl;
	OwnDfa.Minimize();	
	//PrintDfaToText(OwnDfa,"..\\first.txt");
	FoldDFA(OwnDfa);
	//PrintDfaToGv(OwnDfa,"..\\result1.txt");
	//std::cout << (ulong)OwnDfa.Size() << std::endl;
	//display(OwnDfa);
	//OwnDfa.Process((byte*)str, strlen(str), tmp);
	//std::cout << tmp.Size() << std::endl;
	//std::cout << std::endl;

	NFA* nfa2 = CreatNFA(bPcre);
	nfa2->remove_epsilon();
	nfa2->reduce();
	//nfa2->output();
	//nfa2->analyze(stdout);
	//CNfa tmpnfa;
	//nfa2->nfa2CNfa(tmpnfa);
	//PrintDfaToText(tmpnfa, "..\\nfaresult2.txt");
	DFA* BeDfa = nfa2->nfa2dfa();
	delete nfa2;
	if (BeDfa != NULL)
	{
		BeDfa->minimize();
		CDfa newBeDfa;
		BeDfa->Dfa2CDfaNew(newBeDfa);
		if (CompDfa(OwnDfa, newBeDfa))
		{
			Result = 1;
		}
	}
	//BeDfa->dump();
	//std::cout << BeDfa->size() << std::endl;
	//PrintDfaToGv(newBeDfa, "..\\result2.txt");
	//std::cout << (ulong)newBeDfa.Size() << std::endl;
	//newBeDfa.Process((byte* )str, strlen(str), tmp);
	//std::cout << tmp.Size() << std::endl;
	//std::cout << std::endl;
	//display(newBeDfa);


	return Result;
}

void __stdcall Process(const CSnortRule &rule, LPVOID lpVoid)
{
	CCompileResults &result = *(CCompileResults*)lpVoid;
	std::vector<ulong> NoMatchSids;
	ulong nFlag = rule.GetFlag();
	if (rule.Size() == 0)
	{
		return;
	}
	else if (nFlag & CSnortRule::RULE_HASNOT)
	{
		return;
	}
	else if (nFlag & CSnortRule::RULE_HASBYTE)
	{
		return;
	}	
	else
	{
		CRegRule rr;
		Rule2PcreList(rule, rr);
		static ulong num = 0;
		std::cout << ++num << std::endl;
		for (ulong i = 0; i < rr.Size(); ++i)
		{
			for (ulong j = 0; j < rr[i].Size(); ++j)
			{
				const char *tmp = rr[i][j].GetStr();
				if (tmp != NULL && tmp[0] != '\0')
				{
					switch(CompareWithPcre(tmp))
					{
					case 0:
						std::cout << rule.GetSid() << std::endl;
						//NoMatchSids.push_back(rule.GetSid());
						system("pause");
						continue;
					case 1:
						continue;
					case 2:
						std::cout << "nfa error" << std::endl;
						continue;
					case 3:
						std::cout << "dfa error" << std::endl;
						continue;
					}
				}
			}
		}
	}
	//std::ofstream fout("..\\NoMatchSids.txt", ios::app);
	//for (std::vector<ulong>::iterator i = NoMatchSids.begin(); i != NoMatchSids.end(); ++i)
	//{
	//	fout << "sid: " << *i << std::endl;
	//}
}

/*
 *  MAIN - entry point
 */
int main(int argc, char **argv)
{
	init_conf();

	while(!parse_arguments(argc,argv)) usage();
	//printf(">> file: %s\n",config.regex_file);

	DEBUG=config.debug;
	
	if (config.regex_file==NULL){
		fatal("No data file - please use a regex file\n");
	}
	if (config.regex_file!=NULL){
		check_file(config.regex_file,"r");
	}

	//NFA *nfa=NULL;
	//DFA *dfa=NULL;

	ruleset=fopen(config.regex_file,"r");
	parser=new regex_parser(false,false);

	CCompileResults result;
	CompileFile("..\\allrules.rule", Process, &result);

	//std::vector<std::string> regset;
	//ReadRegexs(argv[3], regset);


	//std::vector<CDfa> dfaset;
	//for (std::vector<std::string>::iterator iReg = regset.begin(); iReg != regset.end(); ++iReg)
	//{
	//	const char* re = iReg->c_str();
	//	NFA* nfa = CreatNFA(re);
	//	nfa->remove_epsilon();
	//	nfa->reduce();
	//	//nfa->output();
	//	DFA* dfa = nfa->nfa2dfa();
	//	delete nfa;

	//	if (dfa != NULL)
	//	{
	//		dfa->minimize();
	//	}
	//	//dfa->output();
	//	//std::cout << std::endl;
	//	//dfa->dump();
	//	CDfa newdfa;
	//	dfa->Dfa2CDfaNew(newdfa);
	//	//display(newdfa);
	//	dfaset.push_back(newdfa);
	//	delete dfa;
	//}

	//for (std::vector<CDfa>::iterator iDfa = dfaset.begin() + 1; iDfa != dfaset.end(); ++iDfa)
	//{
	//	std::vector<byte> visited1((iDfa - 1)->Size());
	//	std::fill(visited1.begin(), visited1.end(), 0);
	//	std::vector<byte> visited2(iDfa->Size());
	//	std::fill(visited2.begin(), visited2.end(), 0);
	//	if ((iDfa - 1)->Size() == iDfa->Size())
	//	{
	//		if(EqualDFA(*(iDfa - 1), visited1, (iDfa - 1)->GetStartId(), *iDfa, visited2, iDfa->GetStartId()))
	//		{
	//			std::cout << "the two DFAs are equal!" << std::endl;
	//		}
	//		else
	//		{
	//			std::cout << "the two DFAs are not equal!" << std::endl;
	//		}
	//	}
	//	else
	//	{
	//		std::cout << "the two DFAs are not equal!" << std::endl;
	//	}
	//}

	//REGEXNUM = parser->get_regex_num(ruleset);

	//int i;
	//int *group = new int[REGEXNUM];

	//for(i=0;i<REGEXNUM;i++){
	//	group[i] = i+1;}

	//int setgroup[2];
	//memset(setgroup, 0, 2*sizeof(int));	// clear the group

	//setgroup[0] = 1;
	//for (i = 1; i <= REGEXNUM; i++)
	//{
	//	setgroup[1]=group[i-1];
	//	ulong DFAsize =  parser->parse_regex_group(ruleset, setgroup);
	//	//printf("%d\n",DFAsize);
	//}	

	////parser->parse_to_dfa(ruleset);

	///* END USER CODE */
	//delete[] group;
	//if (parser!=NULL) delete parser;
	//fclose(ruleset);
	/* Automata de-allocation */
	//if (nfa!=NULL) delete nfa;
	//if (dfa!=NULL) delete dfa;
	
	system("pause");
	return 0;
}

