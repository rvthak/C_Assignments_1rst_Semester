#include <stdio.h>
#include <stdlib.h>
#include "header.h"

int main(){
	int i=0, j=0;	// counters
	int n=0; 		// amount of nodes
	int **p;		// main graph pointer
	
	scanf("%d", &n); 					// Read the number of nodes from the user
	p=malloc( (n)*sizeof(int *) ); 		// allocate the needed memory for the "array spine"
	if(p==NULL){ 						// check for malloc errors
		printf("FATAL ERROR: Primary memory allocation failed\n");
		return 1; 		// failure
	}
	
	for(i=0; i<n; i++){ 				// for each "cell" of the previous "array"
		*(p+i)=malloc( n*sizeof(int) );	// allocate the needed memory for the "array lines"
		if(*(p+i)==NULL){ 				// check for malloc failure every time
			printf("FATAL ERROR: Secondary memory allocation failed\n");
			return 1;	// failure
		}
		for(j=0; j<=i; j++){			// i fill the "array" with values
			if(i==j){					// the main diagonal has 0 since the distance of a node from himself is 0
				p[i][j]=0;
			}	
			else{						// for every other "cell"
				scanf("%d", &p[i][j]);	// i read the value from the user
				p[j][i]=p[i][j];		// i copy the value to the symetric "cell" since the whole "array" is symetric to the main diagonal
			}
		}
	}
	solve(n, p); 		// main function - processes the main graph and prints results
	
	for(i=0; i<n; i++){ 	// free the space of each "array line"
		free( *(p+i) );
	}	
	free(p);	// free the "spine array" memory
	
	return 0;	// success
}
