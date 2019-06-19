#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "string.h"

char argmgr(char argc, char *argv[], unsigned char *n, unsigned char *d, char *b, char *s){
	
	if(argc>7){
		fprintf(stderr, " User input given too long, please view README.txt for details\n");
		return 1;
	}
	
	while(--argc){
		argv++;
		if(equals(*argv, "-n")==0){
			if(checknum(*++argv)==0){
				*n=atoi(*argv);
				argc--;
				if(*n<4||*n>26){
					fprintf(stderr, " Wrong parameters given! Must be in form: -n <size> (size=number in [4,26] )\n");
					return 1;
				}
			}
			else{
				fprintf(stderr, " Wrong parameters given! Must be in form: -n <size> (size=number in [4,26] )\n");
				return 1;
			}
		}

		if(equals(*argv, "-d")==0){
			if(checknum(*++argv)==0){
				*d=atoi(*argv);
				argc--;
				if(*d<1){
					fprintf(stderr, " Wrong parameters given! Must be in form: -d <difficulty> (difficulty=number >= 1)\n");
					return 1;
				}
			}
			else{
				fprintf(stderr, " Wrong parameters given! Must be in form: -d <difficulty> (difficulty=number >= 1)\n");
				return 1;
			}
		}

		if(equals(*argv, "-b")==0){
			*b=BLACK;
		}

		if(equals(*argv, "-s")==0){
			*s=1;
		}
	
	}
	
	return 0;
}

char inputmgr(void){
	char *cmd=malloc(12*sizeof(char));
	printf(">");
	scanf("%11s", cmd);

	if(equals(cmd, "quit")==0){
		return 0;
	}
	else if(equals(cmd, "newgame")==0){
		return 1;
	}
	else if(equals(cmd, "play")==0){
		return 2;
	}
	else if(equals(cmd, "cont")==0){
		return 3;
	}
	else if(equals(cmd, "showstate")==0){
		return 4;
	}
	else if(equals(cmd, "undo")==0){
		return 5;
	}
	else if(equals(cmd, "suggest")==0){
		return 6;
	}
	else if(equals(cmd, "level")==0){
		return 7;
	}
	else if(equals(cmd, "swap")==0){
		return 8;
	}
	else if(equals(cmd, "save")==0){
		return 9;
	}
	else if(equals(cmd, "load")==0){
		return 10;
	}
	else{
		return 11;
	}	
}

char getmove(char *x, char *y, int n){
	char temp=getchar();// take the space or the \n out of the way
	*x=getchar(); //letter
	if(*x<'A'||*x>'Z'){
		fprintf(stderr, " Wrong input given in play. Format: [Cap-letter][Number]\n");
		return 1;
	}
	if(*x-'A'>n){
		fprintf(stderr, " Wrong input given in play. Small number given, out of table bounts\n");
		return 1;
	}
	*x=(*x)-'A';	// so it starts from 0 like the array
	
	*y=getchar();
	if(*y<'0'||*y>'9'){
		fprintf(stderr, " Wrong input given in play. Format: [Cap-letter][Number]\n");
		return 1;
	}
	*y=(*y)-'0';
	
	temp=getchar();
	if(temp==' '||temp=='\n'){
		if(*y==0){
			fprintf(stderr, " Wrong input given in play. Small number given, out of table bounts\n");
			return 1;
		}
		*y=(*y)-1; 	// so it starts from 0 like the array
		return 0;
	}
	else if(temp<'0'||temp>'9'){
		fprintf(stderr, " Wrong input given in play. Format: [Cap-letter][Number]\n");
		return 1;
	}
	else if(*y==0&&temp=='0'){
		fprintf(stderr, " Wrong input given in play. Small number given, out of table bounts\n");
		return 1;
	}
	else if(*y<=2&&temp-'0'<=n%10){
		temp=temp-'0';
		*y=10*(*y)+temp-1;	// so it starts from 0 like the array
		return 0;
	}
	else{
		fprintf(stderr, " Wrong input given in play. Big number given, out of table bounts\n");
		return 1;
	}
}
