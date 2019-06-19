#include <stdio.h>
#include <stdlib.h>
#include "header.h"

#define INF -1	// used form the functions as positive infinite
#define macmin(A, B)	( (A)<(B) ? (A) : (B) )	//macro min function used from min

int fwmem(int **graph, int ***sol, int i, int j, int k); // recursive function using Floyd-Warshall algorithm calculates the shortest path from node i to node j
int min(int a, int b);	// calculates min for non negative numbers taking into account positive infinity - INF (its bigger than everything else)
void pg(int n, int **graph); // prints the output

void solve(int n, int **graph){
	int i=0, j=0, k=0;	// counters
	int ***sol; 		// main 3D array pointer
	
	sol=malloc(n*sizeof(int *));	// built a 3D array to store the needed values for memoization
	if(sol==NULL){
		printf(" FATAL MEMORY ERROR DURING SOL BUILD\n");
		return;	// failure
	}
	for(i=0; i<n; i++){
		sol[i]=malloc(n*sizeof(int *));
		if(sol[i]==NULL){
			printf(" FATAL MEMORY ERROR DURING SOL BUILD\n");
			return;	// failure
		}
		for(j=0; j<n; j++){
			sol[i][j]=malloc(n*sizeof(int));
			if(sol[i][j]==NULL){
				printf(" FATAL MEMORY ERROR DURING SOL BUILD\n");
				return;	// failure
			}
			for(k=0; k<n; k++){
				if(k==0){
					sol[i][j][k]=graph[i][j]; 	// i set the "base" layer of values as the initial values of graph to use them when k gets to 0
				}
				else{
					sol[i][j][k]=-2;	// i initialize the non still filled sol member to -2 since its out of value bounds
				}
			}
		}
	}
		
	for(k=0; k<=n-1; k++){			// for each amount of possible intermediate nodes (0=direct path...n-1=the path passes from all the possible available nodes
		for(i=0; i<n; i++){			// for each combinaton of i and j nodes
			for(j=0; j<=i; j++){	// i only reach until i (j<=i) since the array is symetric
				graph[i][j]=min(graph[i][j], fwmem(graph, sol, i, j, k));
			}
		}
	}

	pg(n, graph);	// print results
	return;		// success
}

int fwmem(int **graph, int ***sol, int i, int j, int k){
	int temp0=0, temp1=0, temp2=0, temp3=0; // temp variables to be able to call the recursive function only once every time and then use the value returned multiple times
	if(i==j){	// if we are on the main diagonal
		return 0;
	}
	if(k<0){	// we reached the destination-node
		return graph[i][j];
	}
	
	
	if(sol[i][j][k-1]==-2){						// i check if the cell is empty (-2)
		temp0=fwmem(graph, sol, i, j, k-1);		// if yes i calculate is value, store it, and move on
		sol[i][j][k-1]=temp0;
	}
	else{	// if its already stored
		if(k==0){	// if k==0 => k-1 is out of array bounds so i give it the value for k=0
			temp0=sol[i][j][k];
		}
		else{		// else just read the stored value
			temp0=sol[i][j][k-1];
		}
	}
	
	// i follow the exact same process for the other two calls of fwmem
	if(sol[i][k][k-1]==-2){
		temp1=fwmem(graph, sol, i, k, k-1);
		sol[i][k][k-1]=temp1;
	}
	else{
		if(k==0){
			temp1=sol[i][j][k];
		}
		else{
			temp1=sol[i][k][k-1];
		}
	}
	
	if(sol[k][j][k-1]==-2){
		temp2=fwmem(graph, sol, k, j, k-1);
		sol[k][j][k-1]=temp2;
	}
	else{
		if(k==0){
			temp1=sol[i][j][k];
		}
		else{
			temp2=sol[k][j][k-1];
		}
	}
	
	
	if(temp1==INF||temp2==INF){	// if any or both are INF => no possible path => temp3=INF
		temp3=INF;
	}
	else{	// else there is a path, add the values of the two paths
		temp3=temp1+temp2;	
	}
	
	return min( temp0 , temp3);	// considering not existing paths(temp=INF), return the shortest
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

void pg(int n, int **graph){
	int i=0, j=0;
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
