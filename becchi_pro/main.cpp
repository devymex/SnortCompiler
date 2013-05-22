#include "../becchi_pro/stdinc.h"
#include "../becchi_pro/parser.h"
#include "../becchi_pro/nfa.h"
#include "../becchi_pro/dfa.h"
#include <fstream>
#include <iostream>
#include <string>

int VERBOSE = 1;
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
			printf(">> DFA limit is %d\n",THRESHOLD);
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

void display(CDfanew &newdfa)
{
	for(size_t i = 0; i != newdfa.Size(); ++i)
	{
		std::cout << i << ":  ";
		for(size_t j = 0; j != newdfa.GetGroupCount(); ++j)
		{
			std::cout << (size_t)newdfa[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

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

/*
 *  MAIN - entry point
 */
int main(int argc, char **argv){
	
	init_conf();

	while(!parse_arguments(argc,argv)) usage();
	printf(">> file: %s\n",config.regex_file);

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

	std::vector<std::string> regset;
	ReadRegexs(argv[3], regset);
	
	//std::vector<CDfanew> dfaset;
	for (std::vector<std::string>::iterator iReg = regset.begin(); iReg != regset.end(); ++iReg)
	{
		const char* re = iReg->c_str();
		NFA* nfa = CreatNFA(re);
		nfa->remove_epsilon();
		nfa->reduce();
		nfa->output();
		DFA* dfa = nfa->nfa2dfa();
		delete nfa;

		if (dfa != NULL)
		{
			dfa->minimize();
		}
		//dfa->output();
		//std::cout << std::endl;
		//dfa->dump();
		CDfanew newdfa;
		dfa->Dfa2CDfanew(newdfa);
		display(newdfa);
		//dfaset.push_back(newdfa);
		delete dfa;
	}

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
	//	size_t DFAsize =  parser->parse_regex_group(ruleset, setgroup);
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

