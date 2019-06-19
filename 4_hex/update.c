#include "update.h"
#include <stdio.h>
#include <stdlib.h>
#include "utility.h"
#include "input.h"
#include "ai.h"
#include "string.h"

void update(char *running, char input){
	static char **table;
	static char **tag;
	static char **undo; //2* n^2
	static char *alpha;
	static char player=0;
	static char n=0;
	static char d=0;
	static char b=0;
	static char s=0;
	static char roundcount=0;
	
	char x=0, y=0;
	int val=0;
	int count=1, ch=0;

	
	switch (input){
		case 0:
			//printf("Quit\n");
			*running=0; // if built fails it crashes in ths command idk why
			tbfree(table, n);
			tbfree(tag, n);
			tbfree(undo, 2);
			break;
			
		case 1:
			
			if(table!=NULL){
				tbfree(table, n);
			}
			
			//get input from new game parameters
			
			onestore(&n, &d, &b, &s);
			
			/*for(x=0; x<8; x++){// skip "newgame"
				ch=getchar(); 
			}

			if( ch!='\n'){
				
				while(ch!='\n'){
					
					printf(" %c\n", ch);
					if(ch=='w'){
						printf("Hey\n");
						b='w';
					}
					else if(ch=='b'){
						printf("Hey2\n");
						b='b';
					}
					ch=getchar();
					if(ch==' '){
						break;
					}
				}
				
				printf("player: %c\n", b);
				
				while(ch!='\n'&&ch!=' '){
					printf(" %c\n", ch);
					if(ch=='f'){
						printf("Hey\n");
						s=0;
					}
					else if(ch=='n'){
						printf("Hey2\n");
						s=1;
					}
					ch=getchar();
				}
				
				printf("swap: %d\n", s);
				
				val=0;
				while(ch!='\n'&&ch!=' '){
					val=val*10+ch-'0';
					ch=getchar();
				}
				
				if(val>=4&&val<=26){
					n=val;
				}
				else if(val==0){
					//do nothing keep default value
				}
				else{
					fprintf(stderr, "Error big n given: Using deflalts\n");
					onestore(&n, &d, &b, &s);
				}
				printf("Value: %d\n", n);
				
			}*/

				
			player=b;
			if(b=='b'){
				b=flip(b);
			}
			
			if( (alpha=malloc(26*sizeof(char)))==NULL){
				printf(" FATAL error in alpha alloc\n");
				*running=0;
				return;
			}
			
			for(x=0; x<26; x++){
				alpha[x]='A'+x;
			}
			
			table=tballoc(n, n, 'n');
			tag=tballoc(n, n, 'n');
			undo=tballoc(2, n*n, 30);
			if(table==NULL||tag==NULL||undo==NULL){
				fprintf(stderr,"Error alocating memory for table\n");
				return;
			}
			
			if(b==player){
				printf("%s player (human) plays now\n", player=='w' ? "White" : "Black");
			}
			else{
				printf("%s player (computer) plays now\n", player=='b' ? "White" : "Black");
			}
			
			break;
			
		case 2:
			//printf("Play\n");
			if(b==player){
				if(getmove(&x, &y, n)!=0){ // input error
					return; // go back to read again
				}
	
				if(table[x][y]!='n'){
					printf(" Can't play this move, no space, try something else\n");
					return;
				}
				else{
					table[x][y]=player;
					
					/*if(roundcount>=0){
						undo[roundcount][1]=x;
						undo[roundcount][2]=y;
					}*/
					roundcount++;
					
					if(player==WHITE){
						if(check(table, tag, n, 0, 0, 'w')==1){
							printf("White player (human) wins with path ");
						}
					}
					else{
						if(check(table, tag, n, 0, 0, 'b')==1){
							printf("Black player (human) wins with path ");
						}
					}
				}
				
				b=flip(b);
			}
			else{
				printf("Its not your turn cant play\n");
			}
			
			if(b==player){
				printf("%s player (human) plays now\n", player=='w' ? "White" : "Black");
			}
			else{
				printf("%s player (computer) plays now\n", player=='b' ? "White" : "Black");
			}
			
			break;
			
		case 3:
			//printf("cont\n");
			if( b==flip(player) ){
				roundcount++;
				pcmove(d+1, b, table, undo, tag, n, roundcount, &s, 0, alpha);
				
				if(player==BLACK){
					if(check(table, tag, n, 0, 0, 'w')==1){
						printf("White player (computer) wins with path ");
					}
				}
				else{
					if(check(table, tag, n, 0, 0, 'b')==1){
						printf("Black player (computer) wins with path ");
					}
				}
				
				b=flip(b);
				
			}
			else{
				printf("Its your turn now!\n");
			}
			
			/*if(b==player){
				printf("%s player (human) plays now\n", player=='w' ? "White" : "Black");
			}
			else{
				printf("%s player (computer) plays now\n", player=='b' ? "White" : "Black");
			}*/
			
			break;
			
		case 4:
			//printf("showstate\n");
			showstate(n, table);
			break;
			
		case 5:
			//printf("undo\n");
			/*roundcount--;
			table[ undo[roundcount][1] ][ undo[roundcount][2] ]='n';
			
			undo[roundcount][1]=30;//max==26
			undo[roundcount][2]=30;
			
			b=flip(b);*/
			
			if(b==player){
				printf("%s player (human) plays now\n", player=='w' ? "White" : "Black");
			}
			else{
				printf("%s player (computer) plays now\n", player=='b' ? "White" : "Black");
			}
			
			break;
			
		case 6:
			//printf("suggest\n");
			if(b==player){
				pcmove(d+1, b, table, undo, tag, n, roundcount, &s, 1, alpha);
			}
			else{
				printf("You can only ask for advice when its your turn\n");
			}
			break;
			
		case 7:
			//printf("level\n");			
			scanf("%d", &val);
			if(val>0){
				d=val;
				//printf("New diff= %d\n", d);
			}
			break;
			
		case 8:
			//printf("swap\n");
			if(s==1&&roundcount==1&&b==player){
				swap(table, n, player);
				s=0;
				b=flip(b);	
				roundcount--;
				
				if(b==player){
					printf("%s player (human) plays now\n", player=='w' ? "White" : "Black");
				}
				else{
					printf("%s player (computer) plays now\n", player=='b' ? "White" : "Black");
				}
				
			}
			else{
				printf("Swap not enabled or possible\n");
			}
			
			break;
		case 9:
			//printf("save\n");
			save(n, b, table);
			break;
			
		case 10:
			//printf("load\n");
			load(&n, &b, table, tag, undo);
			onestore(&n, &d, &b, &s);
			player=b;
			break;
			
		case 11:
			printf("Input error: wrong spelled command or too long for no reason\n");
			break;
			
		default:
			printf("FATAL ERROR IN INPUT\n");
			break;
	}
	
	return;	
}
