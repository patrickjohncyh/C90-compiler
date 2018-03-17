#include <stdio.h>

float f(float a);
float g(float a,float b);
float h(float a,int b, float c);
float i(int a, float b, float c, int d, float e);
float j(float a);
float k(float a);

float f_ref(float a){
	return a + 5;
}

float g_ref(float a,float b){
	return a + b;
}

float h_ref(float a,int b, float c){
	return a + b + c;
}

float i_ref(int a, float b, float c, int d, float e){
	return a + b + c + d + e;
}

int main(){

	float a = 1.23;
	float b = 4.5;
	int c = 4;

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




	return 0;

}