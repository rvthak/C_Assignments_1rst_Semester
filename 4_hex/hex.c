#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "utility.h"
#include "update.h"

char main(char argc, char *argv[]){
	char running=1;
	
	// start set-up
	if(start(argc, argv, &running)!=0){
		return 1;
	}
	
	// Main game loop
	while(running){
		update(&running, inputmgr() );
	}
	
	return 0; // END
}


