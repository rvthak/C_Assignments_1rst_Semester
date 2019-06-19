#include "string.h"

char equals(const char *s1, const char *s2){
	for ( ; *s1 == *s2 ; s1++, s2++){
		if (! *s1){
			return 0;
		}	
	}
	return 1;
}

char checknum(const char *s1){
	for( ; *s1!=0; s1++){
		if(*s1<'0'||*s1>'9'){
			return 1;
		}
	}
	return 0;
}

