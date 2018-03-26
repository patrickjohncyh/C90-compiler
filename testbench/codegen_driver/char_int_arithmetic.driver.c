#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int add();
int sub();
int divi();
int mult();
int comma();
int logicalOr();
int logicalOr2();
int logicalOr3();
int logicalAnd();
int logicalAnd2();
int logicalAnd3();
int logicalNot();
int logicalNotT();
int directAssign();
int addAssign();
int subAssign();
int divAssign();
int mulAssign();
int ternary();
int equality();
int equalityF();
int notEquality();
int notEqualityF();
int lt();
int gt();
int lte();
int ltee();
int gte();
int gtee();
int preinc();
int predec();
int postinc();
int postdec();
int array();


extern int a;
extern char b;
extern char c;
extern char g[];




int main(){
	char b_pre;
	int temp = 1.5;
	int temp2 = 128;
	int r = 10000;
	int i = 0;
	for(i = 0; i < 1000;i++){
		a = rand();
		b =	rand()%128+1;
		c = 0;

		if(i%6){
			a = -a;
		}
		else if(i%12){
			b = -b;
		}

		fprintf(stderr,"ite : %d\n",i);
		fprintf(stderr,"a   : %d\n",a);
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
		if(equalityF() != (a==b)){
			return 1;
		}
		if(notEquality() != ( (a!=b))){
			return 1;
		}
		if(notEqualityF() != ( (a!=a))){
			return 1;
		}

		b_pre = b;
		if( (logicalOr() != ( (a || b_pre+5)) || (b != b_pre) )){
			return 1;
		}
		b = b_pre;
		if( (logicalOr2() != ( (c || (b_pre+=5) ) ) || (b != b_pre) )){
			return 1;
		}
		b_pre-=5;
		b = b_pre;
		if( logicalOr3() != ( (c || c))){
			return 1;
		}
		b = b_pre;
		if( (logicalAnd() != ( (c && (b_pre+=5))) || (b != b_pre) )){
			return 1;
		}
		b = b_pre;
		if( (logicalAnd2() != ( (a && (b_pre+=5))) || (b != b_pre) )){
			return 1;
		}
		b_pre-=5;
		b = b_pre;
		if(logicalAnd3() != ( (c && c))){
			return 1;
		}
		b = b_pre;
		if(preinc() != ( ++b_pre) || b_pre!=b){
			return 1;
		}
		b_pre--;
		b = b_pre;
		if(predec() != ( --b_pre) || b_pre!=b){
			return 1;
		}
		b_pre++;
		b = b_pre;
		if(postinc() != ( b_pre++) || b_pre!=b){
			return 1;
		}
		b_pre--;
		b = b_pre;
		if(postdec() != ( b_pre--) || b_pre!=b){
			return 1;
		}
		b_pre++;
		if(array() != ( (g[0] + g[2] + g[1]))){
			return 1;
		}

	}
	return 0;


}
























