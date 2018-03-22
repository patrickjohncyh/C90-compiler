#include <stdio.h>

float f(float a);
float g(float a,float b);
float h(float a,int b, float c);
float i(int a, float b, float c, int d, float e);
float j(float a, float b, float c, float d, float e);
int k(unsigned char a, int b, short c);

float f_ref(float a){
	return a + 5;
}

float g_ref(float a,float b){
	return a + b;
}

float h_ref(float a,int b, float c){
	return a + b - c;
}

float i_ref(int a, float b, float c, int d, float e){
	return a + b - c + d - e;
}


float j_ref(float a, float b, float c, float d, float e){
	return a+b+c+d+e;
}

int k_ref(unsigned char a, int b, short c){
	return a*b/c;
}

int main(){

	float a = 1.23;
	float b = 4.5;
	int c = 4;
	float x = 143.4;
	float y = 9.7485;
	float z = 0.134;
	unsigned char d = 241;
	short e = 10;

	if(f(a) != f_ref(a)){
		return 1;
	}
	if(g(a,b) != g_ref(a,b)){
		return 1;
	}
	if(h(a,c,b) != h_ref(a,c,b)){
		return 1;
	}
	if(i(c,a,a,c,b) != i_ref(c,a,a,c,b)){
		return 1;
	}
	if(j(a,b,x,y,z) != j_ref(a,b,x,y,z)){
		return 1;
	}
	if(k(d,c,e) != k_ref(d,c,e) ){
		return 1;
	}




	return 0;

}