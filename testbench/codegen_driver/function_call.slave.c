int	  atoi(  char *str );
long  atol(  char *str );
int   abs( int n );
int	  isdigit( int ch );
char *strcpy( char *dest,  char *src );


int call_atoi(){
	return atoi("123");
}

int call_atoi_param(char *str){
	return atoi(str);
}

long call_atol(){
	return atol("555");
}

int call_abs_param(int n){
	return abs(n);
}


int call_isdigit(){
	char a = '9';
	return isdigit(a);
}

int call_isdigit_false(){
	char a = '\n';
	return isdigit(a);
}

void call_strcpy(char *dest){
	char *str = "Hello World!";
	strcpy(dest,str);
}


