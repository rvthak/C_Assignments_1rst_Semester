// sdi1800164 - John Rovithakis --- Compile with -O3 for optimal execution time results

#include <stdio.h>
#include <time.h>

#define FROM 3990000000U
#define UNTIL 4010000000U

// Functs ---------------------------------------------------------------------------------------------

int isPrime(unsigned int a); // Input: a number - Output: 0 if it's prime, 1 if it isn't --- Deterministic - Slower method
int Miller_Rabin(unsigned int n); // Input: a number - Output: 0 if it's prime, 1 if it isn't --- Miller-Rabin(Deterministic) - Faster method
unsigned int powMod(unsigned int x, unsigned int n, unsigned int m); // Input: a Base number, an Exponent number and a Modulus number - Output: the result of their modular exponentiation --- Using the given math types from iphw1819_1.pdf


int main() {
	
	printf("Checking range [%u,%u] for primes\n", FROM, UNTIL);
	
	// Vars -------------------------------------------------------------------------------------------
	unsigned int i;		// Step
	unsigned int count;	// Counter
	
	clock_t time;	// Timer Var - return type of clock()

	// Deterministic part -----------------------------------------------------------------------------

	i=FROM|1; //peritopoiisi tou from
	count=0;

	time = clock();

	while (i < UNTIL) {
		if(i % 3 != 0 ){ // we don't check numbers divided by 3 to save time
			if ( isPrime(i) == 0 ) { // 0 for primes, one for non-primes
				count++;
			}
		}
		i+=2; // step 2 in order to skip even numbers
	}
	
	time = clock()-time;
	printf ("Deterministic algorithm: Found %u prime numbers in %.2f secs\n", count , (double)(time)/CLOCKS_PER_SEC);

	// Miller Rabin part ------------------------------------------------------------------------------
	
	i=FROM|1;
	count=0;
	
	time = clock();

	while(i < UNTIL){
		if(i % 3 != 0){
			if(Miller_Rabin(i) == 0) {
				count++;
			}
		}
		i+=2;
	}

	time = clock()-time;
	printf ("Miller-Rabin algorithm:  Found %u prime numbers in  %.2f secs\n", count , (double)(time)/CLOCKS_PER_SEC);
	
	// The End -------------------------------------------------------------------------
	return 0;
}


int Miller_Rabin(unsigned int n){

// Vars --------------------------------------------------------------------------------	

	unsigned short r=0; //short is enough - checked with trial and error for bigger numbers it has to change probably
	unsigned int d=n-1; 
	unsigned int x=0;
	
	unsigned short i=0; // r is short and i<r-1(see below) so consequently i will also be short
	
	char esc=0; // escape variable, it decides if the funct will return 1 --- i could use int here but it only takes 0 or 1, so i decided to save some bytes 


// Breakup "Write n-1 (d) in a specific form"-------------------------------------------

	while((d&1)==0){
		d=d/2;
		r++;
	}
	// when done d is odd

//Basic Miller-Rabin implementation (3 times - one for each a)	
// a = 2 -----------------------------------------------------------------------------	
	x=powMod(2, d, n);
	
	if( !(x==1 || x == n-1) ) {
		
		for(i=0; i<r-1; i++){
			x=(unsigned long long)x*x % n; // was actually faster to calc x like this when it is just squared
			if(x==n-1){
				esc = 1;
				break;
			}
		}
		
		if(esc==0){
			return 1;
		}

	}
	
// a = 7 -----------------------------------------------------------------------------
	x=powMod(7, d, n);
	esc=0;
	
	if( !(x==1 || x == n-1) ) {
		
		for(i=0; i<r-1; i++){
			x=(unsigned long long)x*x % n;
			if(x==n-1){
				esc = 1;
				break;
			}
		}
		
		if(esc==0){
			return 1;
		}
	
	}
	
// a = 61 -----------------------------------------------------------------------------
	x=powMod(61, d, n);
	esc=0;
	
	if( !(x==1 || x == n-1) ) {
		
		for(i=0; i<r-1; i++){
			x=(unsigned long long)x*x % n;
			if(x==n-1){
				esc=1;
				break;
			}
		}
		
		if(esc==0){
			return 1;
		}
	
	}
	
// End of sbjs ----------------------------------------------------------------------------
	return 0;
}


unsigned int powMod(unsigned int x, unsigned int n, unsigned int m){ 
	// i used exacly the formula given in the pdf
	
	unsigned int base=x;
	unsigned int pow=n;
	unsigned int res=1;
		
	while(pow!= 1){
		if((pow&1) == 0){
			base = (unsigned long long)base*base % m;
			pow/=2;
		}
		else{
			res = (unsigned long long)res*base%m;
			pow--;
		}	
	}
	res = (unsigned long long)res*base%m;
	return res;
}


int isPrime(unsigned int a) {
	// the deterministic function we analysed in class
	
	unsigned int x=5;
	unsigned int y=7;

	while ( x*x<= a || y*y <= a) { 
		if (a % x == 0 || a % y == 0 ) {
			return 1;
		}
		x = x + 6;
		y = y + 6;
	}
	
	return 0;
}
