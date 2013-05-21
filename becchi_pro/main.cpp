#include "../becchi_pro/stdinc.h"
#include "../becchi_pro/parser.h"
#include "../becchi_pro/nfa.h"
#include "../becchi_pro/dfa.h"
//#include <sys/time.h>

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


///* *** */
//void cal_interaction(int remain[], int num){
//
//	//int num = parser->get_regex_num(ruleset);
//	
//	//memset(DFAdata, 0, (num+1) * (num+1) *sizeof(unsigned long));	// clear
//
//	int i,j;
//	unsigned long temp = 0;
//
//	for(i=0 ; i < REGEXNUM+1 ; i++){
//		//DFAdata[i]=(unsigned long *) malloc(sizeof(unsigned long )*(num+1));
//		memset(DFAdata[i], 0, (REGEXNUM+1) *sizeof(unsigned long));	// clear
//	}
//
//	int group[3];
//	memset(group, 0, 3*sizeof(int));	// clear the group
//
//	group[0] = 1;
//	for(i=1;i<=num;i++){
//		group[1]=remain[i-1];
//		DFAdata[i][i] =  parser->parse_regex_group(ruleset, group);
//		DFAdata[0][0] += DFAdata[i][i];
//	}
//
//	group[0] = 2;
//	for(i=2;i<=num;i++){
//		group[1]=remain[i-1];
//		for(j=1;j<i;j++){
//			group[2]=remain[j-1];
//			temp =  parser->parse_regex_group(ruleset, group) ;
//			if(temp < DFAdata[i][i] + DFAdata[j][j])
//				DFAdata[i][j] = DFAdata[j][i] =0;
//			else
//				DFAdata[i][j] = DFAdata[j][i] = 1;
//		}
//	}
//
//	for(i=1;i<=num;i++){
//		for(j=1;j<=num;j++){
//			DFAdata[i][0] += DFAdata[i][j];
//		}
//		DFAdata[i][0] -= DFAdata[i][i];
//		DFAdata[0][i] = DFAdata[i][0];
//	}
//
//	// for(i=0;i<=num;i++){
//	// 	for(j=0;j<=num;j++){
//	// 		printf("%lu ",DFAdata[i][j]);
//	// 	}
//	// 	printf("\n");
//	// }
//}


//void fang_yu(int remain[], int num, unsigned long DFAlimit){
//	//init_data();
//
//	int i,j,k;
//
//	cal_interaction(remain,num);
//
//	// printf("\nnow the group is \n");
//	// for(j=0;j<num;j++){
//	// 	printf("%d ", remain[j]);
//	// }	
//
//	//printf("\n >>>>>>>>>>>>> a new group \n");
//
//	int min,tempsum;
//	int remainnum = num;
//	int count = num;
//	unsigned long size=0, tempsize=0;
//	int *NG,*tempNG;
//	int *tempremain, *tempgroup;
//	bool istaken,isfirst;
//
//	if(num == 1){
//		groupcnt++;
//		printf("\ngroup%d is \n",groupcnt);
//		printf("%lu\n",remain[0]);
//		printf("size = %lu\n",DFAdata[0][0]);
//		return;
//	}
//
//	NG = (int *) malloc(sizeof(int )*(num+1));
//	tempNG = (int *) malloc(sizeof(int )*(num+1));
//	tempgroup = (int *) malloc(sizeof(int )*(num+1));
//	tempremain = (int *) malloc(sizeof(int )*num);
//
//	memset(NG, 0, (num+1)*sizeof(int));
//	memset(tempNG, 0, (num+1)*sizeof(int));
//	memset(tempgroup, 0, (num+1)*sizeof(int));
//	memset(tempremain, 0, num*sizeof(int));
//
//
//	min = DFAdata[1][0];
//
//	j=1;
//	for(i=2;i<num;i++){
//		if(DFAdata[i][0]<min){
//			min=DFAdata[i][0];
//			j=i;
//		}
//	}
//	NG[0]++;
//	NG[NG[0]]=j;
//
//	count--; 
//
//	memcpy(tempNG,NG,(num+1)*sizeof(int));
//
//	while(true){
//
//		min = 0;
//		
//		isfirst = true;
//		for(i=1;i<=num;i++){
//			istaken = false;
//			
//			tempsum = 0;
//			for(j=1;j<=tempNG[0];j++){
//				if(i == tempNG[j]) istaken =true;
//			}
//
//			if(!istaken){
//				for(j=1;j<=tempNG[0];j++){
//					tempsum += DFAdata[i][tempNG[j]];
//				}
//
//				if(isfirst){
//					min = tempsum;
//					k = i;
//					isfirst = false;
//				}
//				else{
//					if(tempsum < min){
//						min = tempsum;
//						k = i;
//					}
//				}			
//			}
//		}
//
//		//printf("k = %d\n", k);
//		tempNG[0]++;
//
//		tempNG[tempNG[0]]=k;
//
//		tempgroup[0] = tempNG[0];
//		for(j=1;j<=tempNG[0];j++){
//			tempgroup[j] = remain[tempNG[j]-1];
//			//printf(">>%d ", tempgroup[j]);
//		}
//		
//		tempsize = parser->parse_regex_group(ruleset, tempgroup);
//		//printf("size = %lu\n", size);
//	
//		if(tempsize <= DFAlimit){
//			size = tempsize;
//			memcpy(NG,tempNG,(num+1)*sizeof(int));
//			count--;
//			// printf("\nnow NG is \n");
//			// for(j=1;j<=NG[0];j++){
//			// 	printf("%d ", NG[j]);
//			// }			
//			// printf("size = %lu\n", size);
//			if(count<=0) {
//				groupcnt++;
//				printf("\ngroup%d is \n",groupcnt);
//				for(j=1;j<=NG[0];j++){
//					printf("%d ", remain[NG[j]-1]);
//				}
//				printf("\nsize = %lu\n",size);
//				free(NG);
//				free(tempNG);
//				free(tempgroup);
//				free(tempremain);
//				return;
//			}
//		}
//		else{
//			memcpy(tempremain,remain,num*sizeof(int));
//			for(j=1;j<=NG[0];j++){
//				for(i=0;i<num;i++){
//					if(tempremain[i] == remain[NG[j]-1]){
//						for(k=i;k<num;k++){
//							tempremain[k]=tempremain[k+1];
//						}
//						remainnum--;
//					}
//				}
//			}
//			groupcnt++;
//			printf("\ngroup%d is \n",groupcnt);
//			for(j=1;j<=NG[0];j++){
//				printf("%d ", remain[NG[j]-1]);
//			}
//			printf("\nsize = %lu\n",size);
//			// printf("remainnum is %d\n",remainnum);
//			// printf("\n >>>>>>>>>>>>> a new group \n");
//			fang_yu(tempremain, remainnum, DFAlimit);
//
//			free(NG);
//			free(tempNG);
//			free(tempgroup);
//			free(tempremain);
//			break;
//		}
//
//	}
//
//
//
//}
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

	NFA *nfa=NULL;
	DFA *dfa=NULL;
	
	ruleset=fopen(config.regex_file,"r");
	parser=new regex_parser(false,false);

	//struct timeval start,end;
	//gettimeofday(&start,NULL);
	/* BEGIN USER CODE */

	//cal_total_DFA();

	REGEXNUM = parser->get_regex_num(ruleset);

	int i;
	int *group = new int[REGEXNUM];

	//DFAdata = (unsigned long **) malloc(sizeof(unsigned long *)*(REGEXNUM+1));
	//for(i=0 ; i < REGEXNUM+1 ; i++){
	//	DFAdata[i]=(unsigned long *) malloc(sizeof(unsigned long )*(REGEXNUM+1));
	//	memset(DFAdata[i], 0, (REGEXNUM+1) *sizeof(unsigned long));	// clear
	//}

	//parser->parse_to_dfa(ruleset);

	for(i=0;i<REGEXNUM;i++){
		group[i] = i+1;}

	//fang_yu(group,REGEXNUM,THRESHOLD);

	int setgroup[2];
	memset(setgroup, 0, 2*sizeof(int));	// clear the group

	setgroup[0] = 1;
	for (i = 1; i <= REGEXNUM; i++)
	{
		setgroup[1]=group[i-1];
		size_t DFAsize =  parser->parse_regex_group(ruleset, setgroup);
		//printf("%d\n",DFAsize);
	}
	/* END USER CODE */
	//gettimeofday(&end,NULL);
	//printf(">> preprocessing time: %ldms\n",
	//		(end.tv_sec-start.tv_sec)*1000+(end.tv_usec-start.tv_usec)/1000);
	delete[] group;
	if (parser!=NULL) delete parser;
	fclose(ruleset);
	/* Automata de-allocation */
	if (nfa!=NULL) delete nfa;
	if (dfa!=NULL) delete dfa;
	
	system("pause");
	return 0;

}

