float f(float a);
float g(float a,float b);
float h(float a,int b, float c);
float i(int a, float b, float c, int d, float e);


int main(){

	float a = 1.23;
	float b = 4.5;
	int c = 4;

	if(f(a) != a + 5){
		return 1;
	}
	if(g(a,b) != b/a){
		return 1;
	}
	if(h(a,c,b) != a*2+c*3+b*4){
		return 1;
	}

	if(i(c,a,a,c,b) !=  c*2+a/3+a-4+c*1.5+b/2 ){
		return 1;
	}

	return 0;

}