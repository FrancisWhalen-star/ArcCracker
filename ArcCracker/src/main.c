#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

/*
	Name: ArcCracker
	Email: whalen.jiang@gmail.com
	Author: Francis Whalen
	Date: 21/12/19 16:02
	Description: A program that crack the rar,zip,7z,etc file based on 7z and WinRAR.
*/

/*
TIPs:
struct option
    {
      const char *name;
      int has_arg;
      int *flag;
      int val;
    };

has_arg:
    # define no_argument        0
    # define required_argument  1
    # define optional_argument  2

*/

extern int sevenzCrack();
extern int winrarCrack();
extern void init();

enum {rar = 0, zip, sevenz} type;
enum {sevenzip = 0, winrar} engine;
char arcPath[256];
char sevenzEnginePath[256];
char winrarEnginePath[256];
int minLen = 1;
int maxLen = 5;
char dictOpt[5] = "nl";
char customDict[64];

int getEnginePath();
void setEnginePath();

int main(int argc, char *argv[]) {
	if(getEnginePath()){
		printf("conf.ini not found , please set:\n");
		setEnginePath();
	}

	const char *optString = "hvt:u:cd:l:";
	struct option opts[] = {
	    {"help",	no_argument,		NULL,	'h'},
        {"version",	no_argument,		NULL,	'v'},
        {"type",	required_argument,	NULL,	't'},
		{"using",	required_argument,	NULL,	'u'},
		{"conf",	no_argument,		NULL,	'c'},
		{"dict",	required_argument,	NULL,	'd'},
		{"len",		required_argument,	NULL,	'l'},
		{0, 0, 0, 0}
	};
	if(argc == 1){
		printf("Parameter Missing.\n");
	    exit(0);
	}
	int isPathEmpty = 1;
	for(int i=1; i<argc; i++){
		if(strlen(argv[i]) > 16){
			strcpy(arcPath, argv[i]);
			isPathEmpty = 0;
		}
	}

	char opt;
	while((opt=getopt_long_only(argc, argv, optString, opts, NULL)) != -1){
		switch(opt){
            case 'h': // -h »ò --help		|	show help
                printf("Usage:arccracker encrypted_archive.ext [--type rar|zip|7z] [--using 7z|winrar]\n\n");
                printf("Options: --help    | -h : Show this screen.\n");
                printf("         --version | -v : Show the version of this program.\n");
                printf("         --type    | -t : Specify the archieve type, this needed when the\n");
                printf("                          program couldn't detect the proper file type.\n");
                printf("                          Arguement : rar|zip|7z\n");
                printf("         --using   | -u : Specify the engine used.\n");
                printf("                          Arguement : 7z(default)|winrar\n");
				printf("         --conf    | -c : Set configuration file.\n");
				printf("                          e.g. 7-Zip  : %%7-Zip%%/7z.exe\n");
				printf("                               WinRAR : %%WinRAR%%/unrar.exe\n\n");
				printf("         --dict    | -d : Set the character dictionary used.(not sentence!)\n");
				printf("                          Arguement : n(number)\n");
				printf("                                      l(lowwer)\n");
				printf("                                      u(upper)\n");
				printf("                                      s(symbol)\n");
				printf("                                      c(custom)\n");
				printf("                          e.g. nl(default) : lowweralpha and number in dict.\n");
				printf("                               c  : used the dictionary customed by you.\n");
				printf("         --len     | -l : Set the password min and max length\n");
				printf("                          Arguement : minlen:maxlen (default : 1:5)\n");
                printf("Info:    This program supports any encrypted archives that engine supported.\n");
                printf("         Setting configuration file is needed when first launch.\n");
                printf("         Noted that with great power, come with great responsibility.\n");
                break;
            case 'v': // -v or --version	|	show version
                printf("ArcCracker 0.0.1 based on 7-Zip and WinRAR engine.\n");
                printf("By Francis Whalen (whalen.jiang@gmail.com)\n");
                break;
			case 't': // -t or --type     	|   set encryped archive type (actually not used at all)
				type = !strcmp(optarg, "rar")? rar : !strcmp(optarg, "7z")? sevenz : zip;
				break;
			case 'u': // -u or --using		|   select the engine used
				engine = !strcmp(optarg, "winrar")? winrar : sevenzip;
				break;
			case 'c': // -c or --conf       |	set the configuration of engine
				setEnginePath();
				break;
            case 'd': // -d or --dict		|   set the character dictionary used or custom a dictionary
            	strcpy(dictOpt, optarg);
				for(int i=0; dictOpt[i]; i++){
					if(dictOpt[i] == 'c'){
						printf("Custom dictionary :\n");
						gets(customDict);
					}
				}
            	break;
            case 'l':
            	minLen = maxLen = 0;
            	int optlen = strlen(optarg);
				int sign = 0;
				for(int i=0; i<optlen; i++){
					if(optarg[i] == ':'){
						sign = 1;
					}
					if(!sign && optarg[i] >= '0' && optarg[i] <= '9'){
						minLen = minLen * 10 + optarg[i] - '0';
					}
					if(sign && optarg[i] >= '0' && optarg[i] <='9'){
						maxLen = maxLen * 10 + optarg[i] - '0';
					}
				}
				break;
            default:
            	return 1;
                break;
            }
	}

	if(!isPathEmpty){
		switch(engine){
		    case sevenzip:
		    	init(arcPath, sevenzEnginePath, minLen, maxLen, dictOpt, customDict);
			    sevenzCrack();
		    	break;
		    case winrar:
		    	init(arcPath, winrarEnginePath, minLen, maxLen, dictOpt, customDict);
		    	winrarCrack();
				break;
		    default:
		        break;
	    }
	}

	return 0;
}

int getEnginePath(){
	FILE *conf;
	if(conf = fopen("conf.ini", "r")){
		fgets(sevenzEnginePath, 256, conf);
		fgets(sevenzEnginePath, 256, conf);
		fgets(winrarEnginePath, 256, conf);
		fgets(winrarEnginePath, 256, conf);
		sevenzEnginePath[strlen(sevenzEnginePath)-1] = '\0';
	    winrarEnginePath[strlen(winrarEnginePath)-1] = '\0';
		fclose(conf);
		return 0;
	}
	fclose(conf);
	return -1;
}

void setEnginePath(){
	FILE *conf;
	printf("[7-ZIP ENGINE PATH]\n");
	gets(sevenzEnginePath);
	printf("[WinRAR ENGINE PATH]\n");
	gets(winrarEnginePath);
	if(conf = fopen("conf.ini", "w")){
		fprintf(conf, "[7-ZIP ENGINE PATH]\n");
		fprintf(conf, "%s\n", sevenzEnginePath);
		fprintf(conf, "[WinRAR ENGINE PATH]\n");
		fprintf(conf, "%s\n", winrarEnginePath);
	}
	fclose(conf);
}
