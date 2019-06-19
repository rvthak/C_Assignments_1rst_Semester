// calculate.c iphw1819_2 sdi1800164
#include <stdio.h>

int expression(int ch, int m);		// The main recursive function when done returns result - parameters: 1st: next character, 2nd: call mode => 0 for normal use of function, anything else for reset of the static variables 
int inputmgr(void);					// Input Manager - Takes user inputed characters, passes them throught the error manager and then passes them to expression to move on with calculations
int errormgr(int ch, int i);		// Error Manager - Takes input throught input manager and if needed interupts the expression's recursion and takes control of the output in order to let the user know that an error has occured - Parameters: 1st: for next character, 2nd: for call mode -  1 for value return and reset, 2 for check mode, anything else(i use 0) for default use 
int sign(int op);					// Takes + of - for input and accordingly returns 1 or -1 for the signed numbers
void skip(int ch);					// If expression's recursion ended violently because of error it skips the rest of the line until '\n' in order to move normally with the next calculation for the next line

int main(){
	int ch=0;
	int result=0;
	int error=0;
	int i=1; //result counter

	ch=inputmgr();
	// normal version
	while(ch!=EOF){		
		result=expression(ch, 0);
		error=errormgr(ch, 1); // checks if there is any error and resets errormgr
		// i could have used a switch here
		if(error==0){
			printf("Result %d: %d \n", i, result);
		}
		else if(error==1){
			printf("Result %d: Unexpected character (Missing operator) \n", i);
		}
		else if(error==2){
			printf("Result %d: Unexpected character (Missing number)\n", i);
		}
		else if(error==3){
			printf("Result %d: Unexpected end of input \n", i);
		}
		else if(error==4){
			printf("Result %d: Missing closing parenthesis \n", i);
		}
		else if(error==5){
			printf("Result %d: Forbidden character inputed \n", i);
		}
		else{
			printf("Result %d: Unknown error please contact the Dev and provide the input \n", i);
		}
		i++;
		expression(0, 1);
		ch=inputmgr();
	}
	return 0;
}

int expression(int ch, int m){
	// I made some variables static in order to maintain their values througt the recursion
	static int val=0;		// numeric value storage while the value is being calculated
	static int sig=1;		// sign storage (either 1 or -1)
	
	static int prevChar='\n';	// holds the value of the previous char entered ( NOT including space(' ') and tab)
	static int store=0;			// used for storing the value of val while the new value of val is being calculated for multiplication (i consider the 2 values as one, in order to achieve the right operation priority)
	
	static int mult=0;		// flag variable used to let the system know that a multiplication is on hold
	
	int tempval=0;		// these 3 variables are temps used to reset the values of the statics while expression hasn't finished the recursion
	int tempsig=0;
	int tempstore=0;
	
	if(m==0){ // normal-calculator mode 
		
		if(errormgr(ch, 2)!=0){ // checks if any error has appeared, if any interrupts recursion violently and returns 0
			skip(ch); 
			return 0;
		}

		if(ch>='0'&&ch<='9'){ // i built the value of val while i get the chars one by one
			val=val*10+ch-'0';
			prevChar=ch;
			ch=inputmgr();
			return expression(ch, 0);
		}
		else if(ch==' '||ch=='	'){ // ignore the space( ' ' ) or the tab
			ch=inputmgr();
			return expression(ch, 0);
		}
		else if(ch=='('){ // if a parenthesis opens
			prevChar=ch;
			ch=inputmgr(); 
			if(store!=0){ // i check if a multiplication is on hold and act accordingly
				tempstore=store;
				store=0;
				mult=0;
				
				tempval=tempstore*expression(ch, 0);
				val=tempval;	
			}
			else{ // else we have to deal with + or - so we move ahead normally 
				tempval=expression(ch, 0);
				val=tempval;
			}
			// again i consider the parenthesis one value with the multiplication vals or other parentheses in order to manage right operations priority 

			if(errormgr(ch, 2)!=0){ //in order to exit imediately in case of no ')'
				return 0;
			}
			
			if(prevChar=='\n'){ // check if ch = '\n' to end recursion if '\n' located after closing parenthesis
				tempval=val;
				val=0;
				return tempval;
			}
			prevChar=ch;
			ch=inputmgr();
			return expression(ch, 0);
		}
		else if(mult==1&&prevChar=='*'&&(ch=='+'||ch=='-')){ // if multiplication is on hold and we have signed values, update the val's sign
			sig*=sign(ch);
			prevChar=ch;
			ch=inputmgr();
			return expression(ch, 0);
		}
		else if(mult==1){ // if mult is on hold an a ch is not a number, parenthesis, sign or space move on with mult
			mult=0;
			tempstore=store;
			tempsig=sig;
			store=0;
			sig=1;
			val=val*tempsig*tempstore; // i consider the two vals(store(the value held from previous operations) and val) as one
			return expression(ch, 0);
		}
		else if(ch==')'){ // if parenthesis closed return the value and end the recursion inside the '(' case that calculates val
			prevChar=ch;
			tempval=val;
			tempsig=sig;
			val=0;
			sig=1;
			return tempsig*tempval;
		}
		else if(ch=='+'||ch=='-'){	// if '+' or '-' found
			if(prevChar=='\n'||prevChar=='+'||prevChar=='-'){ // if on start of a new line its a sign (i initialized the prevchar at the beginning to be '\n') or if it had an operation previously it's a sign
				sig*=sign(ch);	//update the sign
				prevChar=ch;
				ch=inputmgr();
				return expression(ch, 0);
			}
			else{ // else its an operation
				tempsig=sig;
				tempval=val;
				val=0;
				sig=sign(ch);
				prevChar=ch;
				ch=inputmgr();
				return tempsig*tempval+expression(ch, 0);	//i multiply with sign and add so subtraction works also
			}	
		}
		else if(ch=='*'){ // i set multiplication on hold and reset needed values 
			mult=1;
			tempsig=sig;
			store=tempsig*val;
			sig=1;
			val=0;
			prevChar=ch;
			ch=inputmgr();
			return expression(ch, 0);
		}
		else if(ch=='\n'){ // end the expression's recursion
			prevChar=ch;
			tempval=val;
			tempsig=sig;
			val=0;
			sig=1;
			return tempsig*tempval;
		}
	}
	else{ // reset mode
		val=0;
		sig=1;
		store=0;
		mult=0;
		prevChar='\n';
		return 0;
	}		
}

int errormgr(int ch, int i){
	// used statics for the same reason that i used them for expression
	static int error=0;		// error value storage
	static int prevInp=0;	// holds the value of the previous char inputed ( including space(' ') and tab )
	static int prevChar=0;	// holds the value of the previous char inputed ( NOT including space(' ') and tab )
	static int ancChar=0;	// holds the value of the previous previous char inputed ( NOT including space(' ') and tab)
	static int openPar=0;	// holds a value connected with the amount of opening/closing parenthesis used 
	int tmp=0;				// temporary variable used for reseting statics
	
	if(i==1){	//error output and reset mode - outputs the first error that happened during expression's recursion, then resets all the statics for next usage
		tmp=error;
		
		error=0;
		prevInp=0;
		prevChar=0;
		ancChar=0;
		openPar=0;
		
		return tmp;
	}
	else if(i==2){	//error check mode - only returns the error or 0 if none exists
		return error;
	}
	
	if(error!=0){	//checks if any error had previously appeared and if any does not check for more
		return error;	
	}
	
	// error testing
	if(ch>='0'&&ch<='9'&&(prevInp==' '||prevInp=='	')&&prevChar>='0'&&prevChar<='9'){ // case of no operator between numbers
		error=1;
	
	}
	else if(ch=='*'&&(prevChar=='*'||prevChar=='+'||prevChar=='-')){ // case of no number between two '*'
		error=2;
	
	}
	else if((ch=='+'||ch=='-')&&(prevChar=='+'||prevChar=='-')&&(ancChar=='+'||ancChar=='-')){	// case of no number between two '+' or '-' also considering number sign
		error=2;

	}
	else if(ch=='\n'&&(prevChar=='+'||prevChar=='-'||prevChar=='*'||prevChar=='(')){	// case of input ending while still in the middle of a calculation
		error=3;

	}
	else if(ch=='('){	// i add 1 when a parenthesis opens and i subtract 1 when it closes, if the input ends and openpar != 0 there is an incomplete parenthesis
		openPar++;
	}
	else if(ch==')'){
		openPar--;
	}
	else if(ch=='\n'&&openPar!=0){	// case of missing closing parenthesis (basically can also locate a missing opening parenthesis if count < 0)
		error=4;
	
	}
	else if((ch<'0'||ch>'9')&&ch!='+'&&ch!='-'&&ch!='*'&&ch!='('&&ch!=')'&&ch!=' '&&ch!='	'&&ch!='\n'){ // case of inputing char that is not supported
		error=5;
	}
	if(ch!=' '&&ch!='	'){ // update the value of the storage vals that do not include space( ' ' ) and tab
		ancChar=prevChar;
		prevChar=ch;
	}
	prevInp=ch;	// update the value of the storage val that includes space( ' ' ) and tab

	return error; // if no error has appeared returns 0
}

void skip(int ch){ //calls itshelf recursively until it finds '\n' => it skips a line when an error has been located in order to move on with the next line
	if(ch=='\n'){
		return;
	}
	ch=inputmgr();
	skip(ch);	
}

int sign(int op){ //gets + or - and return 1 or -1 accordingly
	if(op=='+'){
		return 1;
	}
	else if(op=='-'){
		return -1;
	}
}

int inputmgr(void){
	int ch=getchar(); //gets char
	if(ch==EOF){
		return EOF;
	}
	if(errormgr(ch, 0)!=0){ // passes through error mgr 
		skip(ch);
		return '\n';
	}
	return ch; // gives ch back
}
