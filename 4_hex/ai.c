#include "ai.h"
#include <stdio.h>
#include "utility.h"
#include <stdlib.h>

int minmaxAB(char dif, char minmax, char player, char **table, char **tag, char n, char x, char y){
	int value=0;
	char i, j;
	char temp;
	
	if(dif==0){//reached the end of the tree
		return valueof(table, tag, n, x, y, player);
	}
	
	
	if(minmax==1){//maximizing player
		value=-40;

		for(i=0; i<n; i++){
			for(j=0; j<n; j++){
				
				if(table[i][j]=='n'){

					table[i][j]=player;
					if(value < (temp=minmaxAB(dif-1, 0, flip(player), table, tag, n, x, y))){
						value=temp;
					}
					
					table[i][j]='n';

				}
			}
		}
		
	}
	else{//minimizing player
		value=40;

		for(i=0; i<n; i++){
			for(j=0; j<n; j++){
				
				if(table[i][j]=='n'){
					
					table[i][j]=player;
					if(value > (temp=minmaxAB(dif-1, 1, flip(player), table, tag, n, x, y))){
						value=temp;
					}
					
					table[i][j]='n';
					
				}
			}
		}
		
	}

	return value;
}

int valueof(char **table, char **tag, char n, char x, char y, char player){
	int lengthmax=0, lenghtmin=0;
	char storiesmax=0, storiesmin=0;
	char i, j;
	
	table[x][y]=player;
	
	lengthmax=maxlen(table, tag, n, player);
	lenghtmin=maxlen(table, tag, n, flip(player));
	storiesmax=levels(table, n, player);
	storiesmin=levels(table, n, flip(player));
	
	table[x][y]='n';
	
	return lengthmax-lenghtmin+3*(storiesmax-storiesmin);
}

void pcmove(char d, char player, char **table, char **undo, char **tag, char n, char round, char *s, char sug, char *alpha){

	if(round==0){
		if(sug==1){
			printf(" You may play at %c%d\n", alpha[(char)(n/2+0.5)], (char)(n/2+0.5)+1);
		}
		else{
			printf("Move played: %c%d\n", alpha[(char)(n/2+0.5)], (char)(n/2+0.5)+1);
			table[(char)(n/2+0.5)][(char)(n/2+0.5)]=player;
			/*if(round>=-1){
				undo[round-1][1]=(char)(n/2+0.5);
				undo[round-1][2]=(char)(n/2+0.5);
			}*/
		}
		return;
	}
	else if(round==1){
		srand(120);
		if((rand()%10==3||rand()%10==7)&&*s==1){
			if(sug==1){
				printf(" You may swap\n");
			}
			else{
				printf("Move played: swap\n");
				swap(table, n, player);
				*s=0;
			}
		}
		else{
			if(table[(char)(n/2+0.5)][(char)(n/2+0.5)]==flip(player)){
				if(sug==1){
					printf(" You may play at %c%d\n", alpha[(char)(n/2+0.5)-1], (char)(n/2+0.5)+2);
				}
				else{
					printf("Move played: %c%d\n", alpha[(char)(n/2+0.5)-1], (char)(n/2+0.5)+2);
					table[(char)(n/2+0.5)-1][(char)(n/2+0.5)+1]=player;
					/*if(round>=-1){
						undo[round-1][1]=(char)(n/2+0.5)-1;
						undo[round-1][2]=(char)(n/2+0.5)+1;
					}*/
				}
			}
			else{
				if(sug==1){
					printf(" You may play at %c%d\n", alpha[(char)(n/2+0.5)], (char)(n/2+0.5)+1);
				}
				else{
					printf("Move played: %c%d\n", alpha[(char)(n/2+0.5)], (char)(n/2+0.5)+1);
					table[(char)(n/2+0.5)][(char)(n/2+0.5)]=player;	
					/*if(round>=-1){
							undo[round-1][1]=(char)(n/2+0.5);
							undo[round-1][2]=(char)(n/2+0.5);
						}*/
				}	
			}
		}
		return;
	}
	
	char i, j;
	int maxval=-1000;
	char movex=0, movey=0, temp;
	
	for(i=0; i<n; i++){
		for(j=0; j<n; j++){
			
			if(table[i][j]=='n'){
				table[i][j]==player;
				if(maxval< (temp=minmaxAB(d, 0, flip(player), table, tag, n, i, j))){
					maxval=temp;
					movex=i;
					movey=j;
				}
				table[i][j]=='n';
			}
		}
	}
	
	if(sug==1){
		printf(" You may play at %c%d\n", alpha[movex], movey+1);
	}
	else{
		printf("Move played: %c%d\n", alpha[movex], movey+1);
		table[movex][movey]=player;
		/*if(round>=-1){
			undo[round-1][1]=movex;
			undo[round-1][2]=movey;
		}*/
	}
	return;
}

