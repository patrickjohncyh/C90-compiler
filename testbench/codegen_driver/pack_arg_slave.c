float f(float a);
float g(float a,float b);
float h(float a,int b, float c);
float i(int a, float b, float c, int d, float e);
float j(float a);
float k(float a);



int main(){

	float a = 1.23;
	float b = 4.5;
	int c = 4;

	if(f(a) != a + 5){
		return 1;
	}
	if(g(a,b) != a + b){
		return 1;
	}
	if(h(a,c,b) != a+c+b){
		return 1;
	}

	if(i(c,a,a,c,b) != c+a+a+c+b){
		return 1;
	}




	return 0;

}