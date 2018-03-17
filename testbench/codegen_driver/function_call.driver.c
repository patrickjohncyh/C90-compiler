#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>


int main(){


	if(call_atoi()!=123){
		return 1;
	}
	if(call_atoi_param("1294912") != 1294912){
		return 1;
	}
	if(call_atol()!=555){
		return 1;
	}
	if(call_abs_param(-234594)!=234594){
		return 1;
	}
	if(!call_isdigit()){
		return 1;
	} 
	if(call_isdigit_false()){
		return 1;
	} 

	char newStr[13]; /*Hello World!\000*/
	call_strcpy(newStr);
	if(strcmp(newStr,"Hello World!")){
		return 1;
	}




	return 0;
}