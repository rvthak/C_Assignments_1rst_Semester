#include <stdlib.h>
#include <stdio.h>
#include "utility.h"
#include "input.h"
#include "update.h"

void defaults(char *n, char *d, char *b, char *s){
	*n=11;
	*d=1;
	*b=WHITE;
	*s=0;
	return;
}

char start(char argc, char **argv, char *running){
	//Main variables
	char n=0;
	char d=0;
	char b=0;
	char s=0;
	
	// Pass default values
	defaults(&n, &d, &b, &s);
	
	if(argc!=0){
		// Argument management
		if(argmgr(argc, argv, &n, &d, &b, &s)==1){
			return 1;
		}
	}
	
	onestore(&n, &d, &b, &s);	// store values
	printf(">");
	update(running, 1);				//newgame
	
	return 0;
}

void onestore(char *n, char *d, char *b, char *s){ //stores and then locks to stored state
	static char dim=0;
	static char dif=0;
	static char plr=0;
	static char sw=0;
	static char stored=0;
	
	if(stored==0){
		dim=*n;
		dif=*d;
		plr=*b;
		sw=*s;
		stored=1;
	}
	else{
		*n=dim;
		*d=dif;
		*b=plr;
		*s=sw;
		stored=1;
	}
	return;
}

void showstate(char n, char **table){
	char i=0, j=0, num=1;
	char letter='A';
	char offset=0; // spaces from left
	
	char sup=0;
	char black[5]={'B', 'L', 'A', 'C', 'K'};
	char *side=malloc(2*n*sizeof(char));
	if(side==NULL){
		fprintf(stderr, " Output error: cant allocate the space required for output\n");
		return;
	}
	for(i=0; i<2*n-4; i++){
		if(i==n-3){
			for(j=0; j<5; j++){
				*(side+i+j)=black[j];
			}
			j--;
		}
		else{
			*(side+i+j)=' ';
		}
	}
	
	
	spacer(n*2+1); // set the header position
	printf("W H I T E\n");
	
	spacer(7); // set first Letter position
	for(i=0; i<n; i++){
		printf("%c   ", letter);
		letter++;
	}
	printf("\n");
	
	spacer(7); // set first-top layer
	for(i=0; i<n; i++){
		printf("_   ");
	}
	printf("\n");
	
	
	spacer(6); // set top layer of bricks
	for(i=0; i<n-1; i++){
		printf("/ \\_");
	}
	printf("/ \\");
	printf("\n");
	
	
	for(i=0; i<n; i++){
		spacer(offset);
		offset++;
		printf("%c ", side[sup++]);
		printf("%2d ", num);
		for(j=0; j<n; j++){
			printf("| %c ", (table[j][i]=='n') ? ' ' : table[j][i]);
		}
		printf("| %d\n", num++);
		
		spacer(offset);
		offset++;
		printf("%c ", side[sup++]);
		if(i==n-1){
			spacer(3);
			for(j=0; j<n; j++){
				printf("\\_/ ");
			}
			printf("\n");
		}
		else{
			bricks(n, offset);
		}
	}
	return;
}

void bricks(char n, char offset){
	char i=0;
	spacer(3);
	for(i=0; i<n; i++){
		printf("\\_/ ");
	}
	printf("\\");
	printf("\n");
}

void spacer(char n){ // 0 for space 1 for tab
	while(n>0){
		printf(" ");
		n--;
	}
	return;
}

char flip(char b){ // flips the value of b (turn playing)
	if(b == WHITE){
		return BLACK;
	}
		return WHITE;
}

void save(char n, char turn, char **table){
	FILE *file;
	char *statefile;
	char i, j;
	
	statefile=malloc(15*sizeof(char));
	scanf("%s", statefile);
	
	if((file=fopen(statefile, "wb"))==NULL){
		fprintf(stderr, ">Error creating savefile please try again\n");
		return ;
	}
	if(putc(n, file)==EOF){
		fprintf(stderr,">Error saving to file");
	}
	if(putc(turn, file)==EOF){
		fprintf(stderr,">Error saving to file");
	}
	
	for(i=0; i<n; i++){
		for(j=0; j<n; j++){
			if(putc( table[i][j] , file)==EOF){
				fprintf(stderr,">Error saving to file\n");
				return;
			}
		}
	}
	fclose(file);
	free(statefile);
	return;
}

void load(char *n, char *b, char **table, char **tag, char **undo){
	FILE *file;
	char *statefile;
	char i, j;
	char ch;
	char tempd=1, temps=0;
	
	statefile=malloc(15*sizeof(char));
	scanf("%s", statefile);
	
	if((file=fopen(statefile, "rb"))==NULL){
		fprintf(stderr, ">Error reading savefile please try again\n");
		return ;
	}
	
	tbfree(table, *n);
	tbfree(tag, *n);
	//tbfree(undo, 2);
	
	*n=getc(file);
	*b=getc(file);
	
	onestore(n, &tempd, b, &temps);
	
	
	table=tballoc(*n, *n, 'n');
	tag=tballoc(*n, *n, 'n');
	undo=tballoc(2, (*n)*(*n), 30);
	
	if(table==NULL||tag==NULL||undo==NULL){
		fprintf(stderr,"Error alocating memory for table\n");
		return;
	}
	
	for(i=0; i<*n; i++){
		for(j=0; j<*n; j++){
			table[i][j]=getc(file);
		}
	}

	fclose(file);
	free(statefile);
	return;
}

void tbfree(char **table, char n){
	char x;
	for(x=0; x<n; x++){
		free(table[x]);
	}  
	free(table);
}

char **tballoc(char n, char n1, char init){
	char x,y;
	char **table;
			
	table=malloc(n*sizeof(char *));

	if(table==NULL){
		fprintf(stderr, " FATAL ERROR: Failed to allocate space for main table\n");
		return NULL;
	}

	for(x=0; x<n; x++){
		table[x]=malloc(n1*sizeof(char));
		if(table[x]==NULL){
			fprintf(stderr, " FATAL ERROR: Failed to allocate space for main table\n");
			return NULL;
		}
		for(y=0; y<n1; y++){
			table[x][y]=init;
		}
	}
	return table;
}

void zero(char **tag, char n){
	char i, j;
	
	for(i=0; i<n; i++){
		for(j=0; j<n; j++){
			tag[i][j]='n';
		}
	}
	return;
}

char check(char **table, char **tag, char n, char i, char j, char player){
	if(table[i][j]!=player){
		if(player=='b'){
			if(j==n-1){
				return 0;
			}
			return check(table, tag, n, i, j+1, player);
		}
		else{
			if(i==n-1){
				return 0;
			}
			return check(table, tag, n, i+1, j, player);
		}
	}
	else{
		tag[i][j]='t';
		if(reccheck(table, tag, n, i, j, player)==1){
			zero(tag, n);
			return 1;
		}
		return check(table, tag, n, i, j+1, player);
	}
}

char reccheck(char **table, char **tag, char n, char i, char j, char player){
	int s=0;
	
	if(i==n-1&&player=='b'){
		return 1;
	}
	else if(j==n-1&&player=='w'){
		return 1;
	}
	//printf("Checking at: %d %d\n", j, i);
	
	if(i!=n-1){
		//printf("Check right\n");
		if(table[i+1][j]==player&&tag[i+1][j]!='t'){
			//printf(" ->Gone right\n");
			tag[i][j]='t';
			s+=reccheck(table, tag, n, i+1, j, player);
			if(s==0){
				tag[i][j]='n';
			}
		}
	}
		
	if(i!=0&&s==0){
		//printf("Check left\n");
		if(table[i-1][j]==player&&tag[i-1][j]!='t'){
			//printf(" ->Gone left\n");
			tag[i][j]='t';
			s+=reccheck(table, tag, n, i-1, j, player);
			if(s==0){
				tag[i][j]='n';
			}
		}
	}
	
	if(j!=0&&s==0){
		//printf("Check up left\n");
		if(table[i][j-1]==player&&tag[i][j-1]!='t'){
			//printf(" ->Gone up left\n");
			tag[i][j]='t';
			s+=reccheck(table, tag, n, i, j-1, player);
			if(s==0){
				tag[i][j]='n';
			}
		}
	}
	
	if(i!=n-1&&j!=0&&s==0){
		//printf("Check up right\n");
		if(table[i+1][j-1]==player&&tag[i+1][j-1]!='t'){
			//printf(" ->Gone up right\n");
			tag[i][j]='t';
			s+=reccheck(table, tag, n, i+1, j-1, player);
			if(s==0){
				tag[i][j]='n';
			}
		}
	}
				
	if(i!=0&&j!=n-1&&s==0){
		//printf("Check down left\n");
		if(table[i-1][j+1]==player&&tag[i-1][j+1]!='t'){
			//printf(" ->Gone down left\n");
			tag[i][j]='t';
			s+=reccheck(table, tag, n, i-1, j+1, player);
			if(s==0){
				tag[i][j]='n';
			}
		}
	}
		
	if(j!=n-1&&s==0){
		//printf("Check down right\n");
		if(table[i][j+1]==player&&tag[i][j+1]!='t'){
			//printf(" ->Gone down right\n");
			tag[i][j]='t';
			s+=reccheck(table, tag, n, i, j+1, player);
			if(s==0){
				tag[i][j]='n';
			}
		}
	}
	return s;
}

char maxlen(char **table, char **tag, char n, char player){
	char i, j, max=0, temp;
	for(i=0; i<n; i++){
		for(j=0; j<n; j++){
			if(table[i][j]==player){
				if( max<( temp= reclen(table, tag, n, i, j, player) )  ){
					max=temp;
				}
			}
		}
	}
	return max;
}

char reclen(char **table, char **tag, char n, char i, char j, char player){
	char max=1,temp;
	tag[i][j]='t';
	
	if(i!=n-1){
		//printf("Check right\n");
		if(table[i+1][j]==player&&tag[i+1][j]!='t'){
			//printf(" ->Gone right\n");
			temp=reclen(table, tag, n, i+1, j, player );
			if(temp+1>max){
				max=temp+1;
			}
		}
	}
		
	if(i!=0){
		//printf("Check left\n");
		if(table[i-1][j]==player&&tag[i-1][j]!='t'){
			//printf(" ->Gone left\n");
			temp=reclen(table, tag, n, i-1, j, player );
			if(temp+1>max){
				max=temp+1;
			}
		}
	}
	
	if(j!=0){
		//printf("Check up left\n");
		if(table[i][j-1]==player&&tag[i][j-1]!='t'){
			//printf(" ->Gone up left\n");
			temp=reclen(table, tag, n, i, j-1, player);
			if(temp+1>max){
				max=temp+1;
			}
		}
	}
	
	
	if(i!=n-1&&j!=0){
		//printf("Check up right\n");
		if(table[i+1][j-1]==player&&tag[i+1][j-1]!='t'){
			//printf(" ->Gone up right\n");
			temp=reclen(table, tag, n, i+1, j-1, player );
			if(temp+1>max){
				max=temp+1;
			}
		}
	}
				
	if(i!=0&&j!=n-1){
		//printf("Check down left\n");
		if(table[i-1][j+1]==player&&tag[i-1][j+1]!='t'){
			//printf(" ->Gone down left\n");
			temp=reclen(table, tag, n, i-1, j+1, player );
			if(temp+1>max){
				max=temp+1;
			}
		}
	}
		
	if(j!=n-1){
		//printf("Check down right\n");
		if(table[i][j+1]==player&&tag[i][j+1]!='t'){
			//printf(" ->Gone down right\n");
			temp=reclen(table, tag, n, i, j+1, player );
			if(temp+1>max){
				max=temp+1;
			}
		}
	}
	tag[i][j]='n';
	return max;
}

char levels(char **table, char n, char player){
	char i, j;
	char count=0;
	
	if(player=='b'){
		for(i=0; i<n; i++){
			for(j=0; j<n; j++){
				if(table[i][j]==player){
					count++;
					break;
				}
			}
		}
	}
	else{
		for(i=0; i<n; i++){
			for(j=0; j<n; j++){
				if(table[j][i]==player){
					count++;
					break;
				}
			}
		}
	}
	
	
	return count;
}

void swap(char **table, char n, char player){
	char i,j;
	
	for(i=0; i<n; i++){
		for(j=0; j<n; j++){
			if(table[i][j]==flip(player)){
				table[i][j]=='n';
				table[j][i]==player;
				return;
			}
		}
	}
	printf("Error: Did not swap\n");
	return;
}
