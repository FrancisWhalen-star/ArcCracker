#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <process.h>
#include <math.h>

const char *numSet = "0123456789";
const char *lowerAlphaSet = "abcdefghijklmnopqrstuvwxyz";
const char *upperAlphaSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char *symbolSet = " !\"#$%\'()*+,-./:;=?@[\\]^_`{}~";

char passwd[64];
char cmd[256];
int currLen = 0;

int minPasswdLen, maxPasswdLen;
char enginePath[256];
char archivePath[256];
double startClock, stopClock;
double totalPossibility;
long long sum;
int count;
int debug;

typedef struct s_dictSet *DictSet;
struct s_dictSet{
	int dictLen;
	int* mark;
	char* dict;
};
DictSet dictSet;


int nextPasswd();
int sevenzCrack();
int winrarCrack();
void init(char* arcPath, char* engPath, int minLen, int maxLen, char* dictArgs, char* customDict);
DictSet createDictSet(char* dictArgs, char* customDict);


int sevenzCrack(){
	char temp[66];
	printf("minlen : %d ; maxlen : %d\n", minPasswdLen, maxPasswdLen);
	printf("7-zip engine working...\n");
    do{
//        sprintf(cmd, "%s %s %s %s%s", enginePath, "-y -bso0 -bse0 -bsp0 x", archivePath, "-p", passwd);
		sprintf(temp, "%s%s", "-p", passwd);
//        if((debug=system(cmd)) == 0){ // system() will return 0 if detect password, error return 2
        if((debug=_spawnlp(_P_WAIT, enginePath, enginePath, "-y", "-bso0", "-bse0", "-bsp0", "x", archivePath, temp, NULL)) == 0){
//		if(0){
		    printf("\nPassword found : %s", passwd);
	        return 0;
        }
        stopClock = clock();
        if((double)(stopClock-startClock)/CLK_TCK > 3){
            startClock = clock();
            printf("\rCracking : %s --%3d key/sec  remaining : %.4f day ", passwd, count/3, (totalPossibility-sum)/count/28800);
            count = 0;
        }
    }while(nextPasswd());
	printf("\nPassword not found.");
	return -1;
}

int winrarCrack(){
    printf("minlen : %d ; maxlen : %d\n", minPasswdLen, maxPasswdLen);
	printf("WinRAR engine working...\n");
 	do{
	    sprintf(cmd, "%s %s %s %s%s", enginePath, "-y -inul x", archivePath, "-p", passwd);

	    if((debug=system(cmd)) == 0){ // system will return 0 if detect password, error return 11, path error return 10
			printf("\nPassword found : %s", passwd);
	    	return 0;
        }else if(debug == 10){
        	sprintf(cmd, "%s %s %s %s%s", enginePath, "-y x", archivePath, "-p", passwd);
        	system(cmd);
			return -2;
		}

        stopClock = clock();
        if((double)(stopClock-startClock)/CLK_TCK > 3){
            startClock = clock();
            printf("\rCracking : %s --%3d key/sec  remaining : %.4f day", passwd, count/3, (totalPossibility-sum)/count/28800);
            count = 0;
        }
    }while(nextPasswd());
    printf("\nPassword not found.");
	return -1;
}

int old_nextPasswd(){
	if(currLen <= maxPasswdLen){
		int i;
		for(i=currLen-1; i>=0; i--){
			if(passwd[i] == numSet[9]){
				continue;
			}
			for(int j=0; j<10; j++){
				if(passwd[i] == numSet[j]){
					passwd[i] = numSet[j+1];
					count++;

					int m = i + 1;
					while(m < currLen){
						passwd[m++] = numSet[0];
					}
					return 1;
				}
			}
		}
		if(i < 0 && passwd[0] == numSet[9]){
			currLen++;
			for(int i = 0; i < currLen; i++){
			    passwd[i] = numSet[0];
	        }
		}
	}else{
		return 0;
	}
}

int nextPasswd(){
	/* inverted string */
	/* plus 1 */
	dictSet->mark[0]++;
	count++;
	sum++;
	/* carry 1 */
	if(dictSet->mark[currLen-1] == dictSet->dictLen - 1){
		if(++currLen > maxPasswdLen)
			return 0;
	}
	for(int i=0; i<currLen; i++){
		if(dictSet->mark[i] >= dictSet->dictLen){
			dictSet->mark[i] = 0;
			dictSet->mark[i+1]++;
		}else{
			break;
		}
	}

	/* mapping to passwd */
	for(int i=currLen-1; i>=0; i--){
	    passwd[currLen-1-i] = dictSet->dict[dictSet->mark[i]];
	}
	return 1;
}

void init(char* arcPath, char* engPath, int minLen, int maxLen, char* dictArgs, char* customDict){
	FILE *p;
	int i;
	for(i=0; arcPath[i]; i++){
		if(arcPath[i] == ' ') break;
	}
	if(!(p=fopen(arcPath, "rb"))){
		fclose(p);
		printf("ERROR : Archive file does not found.");
		exit(0);
	}
	fclose(p);

	startClock = clock();
	count = 0;
    for(int i=0; i<minLen; i++){
		passwd[i] = '0';
	}
	currLen = minLen;
	minPasswdLen = minLen;
	maxPasswdLen = maxLen;
	strcpy(enginePath, engPath);
	if(i != strlen(arcPath)){
		sprintf(archivePath, "\"%s\"", arcPath);
	}else{
	    strcpy(archivePath, arcPath);
    }

	createDictSet(dictArgs, customDict);

	totalPossibility = pow(dictSet->dictLen, maxPasswdLen);

	printf("Archive file path : %s\n", arcPath);
}

DictSet createDictSet(char* dictArgs, char* customDict){
	dictSet = (DictSet)malloc(sizeof(struct s_dictSet));
	dictSet->dictLen = 0;
	for(int i=0; dictArgs[i]; i++){
		if(dictArgs[i] == 'n'){
			dictSet->dictLen += 10;
		}
		if(dictArgs[i] == 'l'){
			dictSet->dictLen += 26;
		}
		if(dictArgs[i] == 'u'){
			dictSet->dictLen += 26;
		}
		if(dictArgs[i] == 's'){
			dictSet->dictLen += 33;
		}
		if(dictArgs[i] == 'c'){
			dictSet->dictLen = strlen(customDict);
		}
	}

	dictSet->dict = (char*)malloc(sizeof(char)*dictSet->dictLen + 1);
	dictSet->dict[0] = '\0';
	for(int i=0; dictArgs[i]; i++){
		if(dictArgs[i] == 'n'){
			strcat(dictSet->dict, numSet);
		}
		if(dictArgs[i] == 'l'){
			strcat(dictSet->dict, lowerAlphaSet);
		}
		if(dictArgs[i] == 'u'){
			strcat(dictSet->dict, upperAlphaSet);
		}
		if(dictArgs[i] == 's'){
			strcat(dictSet->dict, symbolSet);
		}
		if(dictArgs[i] == 'c'){
			strcpy(dictSet->dict, customDict);
		}
	}
    dictSet->mark = (int*)malloc(sizeof(int)*(maxPasswdLen + 1));

	for(int i=0; i<maxPasswdLen; i++){
		if(i < minPasswdLen){
		    dictSet->mark[i] = 0;
		}else{
			dictSet->mark[i] = -1;
		}
	}

	return dictSet;
}

void debugf(){
	printf("numSet:%s", numSet);
	printf("lowerAlphaSet:%s", lowerAlphaSet);
	printf("upperAlphaSet:%s", upperAlphaSet);
	printf("charSet:%s", symbolSet);
}
