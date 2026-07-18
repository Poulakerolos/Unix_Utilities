#include<string.h>
#include <unistd.h>
int echo_main(int argc, char *argv[]) {
    
    if(argc<1){
        write(1,"no input string\n",17);
        return -1;
    }
    else {
        int i=1;//first arg is echo-->[0]
        for(;i<argc;i++){
        write(1,argv[i],strlen(argv[i]));
        if(i<argc-1)
        write(1," ",1);
            
        }

    }
        write(1,"\n",1);
        return 0;
    }

