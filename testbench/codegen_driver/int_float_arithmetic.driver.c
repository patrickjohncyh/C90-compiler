#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float add();
float sub();
float divi();
float mult();
float comma();
float logicalOr();
float logicalOr2();
float logicalOr3();
float logicalAnd();
float logicalAnd2();
float logicalAnd3();
float logicalNot();
float logicalNotT();
float directAssign();
float addAssign();
float subAssign();
float divAssign();
float mulAssign();
float ternary();
float equality();
float equalityF();
float notEquality();
float notEqualityF();
float lt();
float gt();
float lte();
float ltee();
float gte();
float gtee();
float preinc();
float predec();
float postinc();
float postdec();
float array();


extern int a;
extern float b;
extern float c;
extern int g[];




int main(){
	float b_pre;
	float temp = 1.5;
	float temp2 = 128;
	int r = 10000;
	int i = 0;
	for(i = 0; i < 1000;i++){
		b = (float)( ((float)rand()/(float)(RAND_MAX)) * r);
		a = rand();
		c = 0;

		if(i%6){
			a = -a;
		}
		else if(i%12){
			b = -b;
		}

		fprintf(stderr,"ite : %d\n",i);
		fprintf(stderr,"a   : %f\n",a);
		fprintf(stderr,"b   : %d\n",b);

		if(add() != ( a+b)){
			return 1;
		}
		if(sub() != ( a-b)){
			return 1;
		}
		if(divi() != ( a/b)){
			return 1;
		}
		if(mult() != ( a*b)){
			return 1;
		}
		if(comma() != ( b)){
			return 1;
		}
		if(logicalNot() != ( !a)){
			return 1;
		}
		if(logicalNotT() != ( !c)){
			return 1;
		}
		if(directAssign() != ( a+b)){
			return 1;
		}
		if(addAssign() != ( a+b)){
			return 1;
		}
		if(subAssign() != ( a-b)){
			return 1;
		}
		if(divAssign() != ( a/b)){
			return 1;
		}
		if(mulAssign() != ( a*b)){
			return 1;
		}
		if(lt() != ( a<b)){
			return 1;
		}
		if(gt() != ( a>b)){
			return 1;
		}
		if(lte()!= (a<=b)){
			return 1;
		}
		if(ltee() != ( a<=a)){
			return 1;
		}
		if(gte() != ( a>=b)){
			return 1;
		}
		if(gtee() != ( a>=a)){
			return 1;
		}
		if(equality() != ( a==a)){
			return 1;
		}
		if(equalityF()!= (a==b)){
			return 1;
		}
		if(notEquality() != ( (a!=b))){
			return 1;
		}
		if(notEqualityF() != ( (a!=a))){
			return 1;
		}

		b_pre = b;

		if( (logicalOr() != ( (a || b+5)) )|| (b != b_pre) ){
			return 1;
		}
		b = b_pre;
		if( (logicalOr2() != ( (c || b+5))) || (b != b_pre+5) ){
			return 1;
		}
		b = b_pre;
		if( logicalOr3() != ( (c || c))) {
			return 1;
		}
		b = b_pre;
		if( (logicalAnd() != ( (c && b+5)))|| (b != b_pre) ){
			return 1;
		}
		b = b_pre;
		if( (logicalAnd2() != ( (a && b+5))) || (b != b_pre+5) ){
			return 1;
		}
		b = b_pre;
		if(logicalAnd3() != ( (c && c))){
			return 1;
		}
		b = b_pre;
		if(preinc() != ( b_pre+1) || b_pre+1!=b){
			return 1;
		}
		b = b_pre;
		if(predec() != ( b_pre-1) || b_pre-1!=b){
			return 1;
		}
		b = b_pre;
		if(postinc() != ( b_pre) || b_pre+1!=b){
			return 1;
		}
		b = b_pre;
		if(postdec() != ( b_pre )|| b_pre-1!=b){
			return 1;
		}
		if(array() != ( (g[0] + g[2] + g[1]))){
			return 1;
		}

	}
	return 0;


}
























