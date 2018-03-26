float f1(){
	float a=123.1384;
	return a;
}

float f2(){
	float a=-123.1384;
	return a;
}

int f3(){
	int a = 1959948;
	return a;
}

int f4(){
	int a = -1959948;
	return a;
}

float f5(){
	int a=123.1384;
	return a;
}

float f6(){
	int a=-123.1384;
	return a;
}

char f7(){
	float a = 1959948.111;
	return a;
}

short f8(){
	float a = -1959948.1231;
	return a;
}

char f9(){
	float a = 416906;
	return a;
}

short f10(){
	float a = -425098;
	return a;
}

unsigned char f11(){
	float a = 416906;
	return a;
}

unsigned short f12(){
	float a = -425098;
	return a;
}



int main(){
	float x=123.1384;
	int y = 1959948;
	float z = 123;
	return (f1() == x && f2() == -x && f3() == y && f4() == -y && f5() == z &&  f6() == -z && f7() == 12 && f8() == 6132 && f9() == -118 && f10() == -(-(-31882)) && f11()==138 && f12()==33654  );

}












