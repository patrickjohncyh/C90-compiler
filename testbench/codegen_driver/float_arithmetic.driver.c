#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float add();
float sub();
float divi();
float mult();
float comma();
float logicalOr();
float logicalAnd();
float logicalNot();
float directAssign();
float addAssign();
float subAssign();
float divAssign();
float mulAssign();
float ternary();
float equality();
float notEquality();
float lt();
float gt();
float lte();
float gte();
float preinc();
float predec();
float postinc();
float postdec();
float array();


extern float a;
extern float b;
extern float c;




int main(){

	int r = 10000;
	int i = 0;
	for(i = 0; i < 100;i++){
		a = ((float)rand()/(float)(RAND_MAX)) * r;
		b = ((float)rand()/(float)(RAND_MAX)) * r;
		c = 0;

		fprintf(stderr,"ite : %d\n",i);
		fprintf(stderr,"a   : %f\n",a);
		fprintf(stderr,"b   : %f\n",b);

		if(add() != a+b){
			return 1;
		}
		if(sub() != a-b){
			return 1;
		}
		if(divi() != a/b){
			return 1;
		}
		if(mult() != a*b){
			return 1;
		}



	}
	return 0;


}
























