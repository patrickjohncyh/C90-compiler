int x();

int main(){
	int a = 25;
	goto A;
	a = a + 5;
	A:
	a = a + 6;

	a = a + x();
	if(a < 100)
		goto A;

	return a;
}

int x(){
	int a = 40;
	goto A;
	a = a + 15;
	A:
	a = a + 23;
	return a;
}