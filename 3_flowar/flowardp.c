#include <stdio.h>
#include <stdlib.h>
#include "header.h"

#define INF -1	// used form the functions as positive infinite
#define macmin(A, B)	( (A)<(B) ? (A) : (B) ) //macro min function used from min

int min(int a, int b); 	// calculates min for non negative numbers taking into account positive infinity - INF (its bigger than everything else)

#ifdef PATH 								//i have 2 different versions of pg (print graph) one for the case when we need the exacts paths followed and on for the raw distances
void pg(int n, int **graph, int **next);  	//with paths
void printpath(int **next, int u, int v); 	//"decodes" and prints the paths followed in the right format
#else
void pg(int n, int **graph); 				// only distances
#endif

void solve(int n, int **graph){
	int i=0, j=0, k=0; 	// counters
	
	#ifdef PATH
	int temp=0;			// helping variable
	int **next=malloc(n*sizeof(int *)); 	//built the dynamic array needed for storing the values for path tracing (in the same way as the main graph array)
	if(next==NULL){		// malloc error checking
		printf(" FATAL MEMORY ERROR DURING NEXT BUILD (Step 1)\n");
		return;			//failure
	}
	
	for(i=0; i<n; i++){
		*(next+i)=malloc(n*sizeof(int));
		if(*(next+i)==NULL){	// malloc error checking
			printf(" FATAL MEMORY ERROR DURING DIST AND NEXT BUILD (Step 2)\n");
			return; 	//failure
		}
		for(j=0; j<n; j++){	
			// Initializing the next array 
			if(graph[i][j]==INF){ 	//if there is no initial connection between nodes i and j there is no path
				next[i][j]=INF;
			}
			else{
				next[i][j]=j;		// else you can go directly from i to j
			}
		}
	}
	#endif
	// Main floyd warshall 
	for(k=0; k<=n-1; k++){			// for each amount of possible intermediate nodes (0=direct path...n-1=the path passes from all the possible available nodes
		for(i=0; i<n; i++){			// for each combinaton of i and j nodes
			for(j=0; j<=i; j++){	// i only reach until i (j<=i) since the array is symetric 
				#ifdef PATH
				temp=graph[i][j]; 	// temporarily store the current value of graph[i][j] in order to be able to check if the path changed or stayed the same
				#endif
				if(graph[i][k]==INF||graph[k][j]==INF){ // if there is no connection between i and k or k and j do nothing
					;
				}
				else{ // if there is a connection available check if its shorter and replace the current shorter one if needed
					graph[i][j]= min(graph[i][j], graph[i][k]+graph[k][j]);
				}
				#ifdef PATH
				if(graph[i][j]!=temp){ // if temp has changed after the calculations the path has gone through K so update the next array
					next[i][j]=next[i][k];
				}
				#endif
			}
		}
	}
	
	#ifdef PATH // print the respective output considering if PATH is defined
	pg(n, graph, next);
	#else
	pg(n, graph);
	#endif
	
	return; // success
}

int min(int a, int b){
	if(a==INF){ 		//if a or b =INF return the corresponding other one else return the min of these two 
		return b;		//if they are both INF it will either go into a or b but the other one is also INF so it will then return INF
	}
	else if(b==INF){
		return a;
	}
	else{
		return macmin(a, b); //calls the macro function that return the min (a,b NOT negative)
	}
}

#ifdef PATH
void pg(int n, int **graph, int **next){ // just prints the shortest distances in the required format - WITH path
	int i=0, j=0;
	printf("\n");
	for(i=1; i<n; i++){
		for(j=0; j<i; j++){
			printf(" From node %-2d to node %-2d:", i, j);
			if(graph[i][j]==INF){
				printf(" There is no path\n");
			}
			else{
				printf(" Length of shortest path is %-2d\n", graph[i][j]);
				printf(" Shortest path is: ");
				printpath(next,  i, j); // will calculate and print the actual path
			}
		}
		printf("\n");
	}
}

void printpath(int **next, int u, int v){
	// i dont have a case of no path because i only call this function when im sure that there is an available path
	printf(" %d", u);
	while(u!=v){	//trace back the path by going to the next "cell" pointed by the current one until i reach the line-node i need to reach
		u=next[u][v];
		printf(" -> %d", u);
	}
	printf("\n");
	return;
}
#else
void pg(int n, int **graph){ // - WITHOUT path
	int i=0, j=0;
	printf("\n");
	for(i=1; i<n; i++){
		for(j=0; j<i; j++){
			printf(" From node %2d to node %2d:", i, j);
			if(graph[i][j]==INF){
				printf(" There is no path\n");
			}
			else{
				printf(" Length of shortest path is %-2d\n", graph[i][j]);
			}
		}
		printf("\n");
	}
}
#endif
