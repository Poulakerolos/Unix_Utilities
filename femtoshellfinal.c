#include<string.h>
#include<unistd.h>
#include<stdio.h>
#define arraysize 1000000
#define COLOR_YELLOW "\033[33m"
#define COLOR_RESET  "\033[0m"
//note:fflush(stdout) is used to empty the buffer without using \n
//(we do not need a new line each time the prompt is printed)

int echofile(char * ptr)
{  char *ptr2=ptr+1;
    printf("%s",ptr2);
    return 0;
}

int exitfile()
{
    printf("Good Bye\n");
    return -3;
}



int femtoshell_main(int argc ,char*argv[]){

    int number=0;
while(number!=-3){  
printf(COLOR_YELLOW "femto_shell_pversion>" COLOR_RESET);
fflush(stdout);
    char buf[arraysize];
   if(fgets(buf,arraysize,stdin)==NULL)
   return number;

   int i=0;
   char command [10];
   char *cpcommand=command;
   char * ptr=NULL;
   char * limit=buf;
   for(i;buf[i];i++){
   if(buf[i]==' '|buf[i]=='\n'){
                
            ptr=buf+i;
            while((limit!=ptr)&&(cpcommand-command)<9){
            *cpcommand=*limit;
            limit++;
            cpcommand++;
            }
            break;
   }
   };
   *cpcommand='\0';
    if (!strcmp(command, "echo")){
        number = echofile(ptr);
        fflush(stdout);}
    else if (!strcmp(command, "exit")||!strcmp(command,"exit\n")){
        number = exitfile();
        fflush(stdout);
    
        return 0;
    }
    else if(buf[0]=='\n');
           
    else {
        printf("Invalid command\n");
        fflush(stdout);
        number= -1;
        
    }

}
}


int main(int argc,char** argv){
femtoshell_main(argc,argv);
return 0;
}
