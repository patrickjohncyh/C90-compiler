int main(){
	int c = 10;	//ensure that addressing is correct
	int d = 20;	//ensure that addressing is correct
	int e = 30;	//ensure that addressing is correct
	char a[4];
	int b = 121318; //ensure that array is storing in the right direction on the stack
	a[2] = 198;
	a[3] = 123;


	return a[2] + c + d + e;
}