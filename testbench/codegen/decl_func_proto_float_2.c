float f(float a, int b, float c);


int main(){
	float a = 1.25;
	float c = 9.5;
	int b = -5;
	return f(1.25,-5,9.5) == a*b*c ;
}

float f(float x, int y, float z){
	return x * y * z;
}