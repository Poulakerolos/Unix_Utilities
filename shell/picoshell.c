#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#define yellow_color "\033[33m"
#define reset_color  "\033[0m"
//block1:take user input {each word is separated form the other by one space}
char *readshell(){

    int arraysize = 8;
    int length = 0;
    char *buf = malloc(arraysize);
    int c;
    while ((c = getchar()) != '\n') {
	    if(c==EOF){
	    free(buf);
	    return 0;
	    }
	if (arraysize < length + 1) {
	    arraysize *= 2;
	    char *temp = realloc(buf, arraysize);
	    if (temp == NULL) {
		printf("error\n");
		return NULL;
	    }
	    buf = temp;
	}
	if (c == ' ') {
	    if (length > 0 && buf[length - 1] != ' ')
		buf[length++] = ' ';
	} 
     	else
		buf[length++] = (char) c;
	
    }
    buf[length] = 0;
    return buf;
}
//block2:returns pointer to character {will be used int block 3}
char *findword(char *buf)
{
    int commandsize = 4;
    int length = 0;
    char *bufc = malloc(commandsize);
    while (buf[length] != ' ' && buf[length] != '\0') {
	if (length + 1 >= commandsize) {
	    commandsize *= 2;
	    char *temp = realloc(bufc, commandsize);
	    if (temp == NULL) {
		printf("error\n");
		return NULL;
	    }
	    bufc=temp;
	}
	bufc[length] = buf[length];
	length++;
    }
    bufc[length++]='\0';
    return  bufc;
}
//block3:uses block2 to divide the user input into tokens,returns char** 
//char**-----------------------------
//      |         |        |        |
//    token1    token2   token3   NULL
char **tokenize(char*input){
	int maxTokens=100;
	char**tokens=malloc(maxTokens*sizeof(char*));
	if(tokens==NULL) {
		printf("error in tokenize\n");
		fflush(stdout);
		return NULL;}
	int argc=0,pos=0;
	while(input[pos]!='\0'){
		if(argc>maxTokens-1){
		maxTokens*=2;
		char**temp=realloc(tokens,sizeof(char*)*maxTokens);
			if(temp==NULL){
			printf("error:reallocating tokens\n");
			return NULL;
			}
		tokens=temp;
		}
		while(input[pos]==' ')pos++;
	char*word=findword(&input[pos]);
	tokens[argc++]=word;
	pos+=strlen(word);
	}
	tokens[argc]=NULL;
	return tokens;
}
//deallocation
void freetokens(char**tokens){
for(int i=0;tokens[i];i++)
	free(tokens[i]);
free(tokens);
}

int picoshell_main()	
{
	printf(yellow_color "PicoShell_Pversion>>" reset_color);
	fflush(stdout);
    int status;
    char *input = readshell();
    if(input ==NULL){
    free(input);
    return 0;
    }
    char**argv=tokenize(input);
    if(argv==NULL||argv[0]==NULL){
    freetokens(argv);
    free(input);
    return 1;
    }
    if(strcmp(argv[0],"exit")==0){
    goto terminate;
    }
    if(strcmp(argv[0],"cd")==0){
   int value= chdir(argv[1]);
   if(value==0){
   return 1;
   }
   else
	   return 0;
    }
    pid_t pid = fork();
    if (pid > 0) {
	free(input);
	freetokens(argv);
	wait(&status);
	return 1;
    } else if (pid == 0) {
    execvp(argv[0],argv);
    printf("error: undefined input \n");
    free(input);
    freetokens(argv);
    return 2;
    }
    else{
    printf("error\n");
    free(input);
    freetokens(argv);
    return 0;}
    
terminate:{
		  freetokens(argv);
		  free(input);
		  printf("Good bye\n");
		  return 0;}
		  }
		  
int main(){int x=1;
	while(x){
 x=picoshell_main();
	}
	printf("\n");
return 0;
}
