int main(){
	int c = 0;	//ensure that addressing is correct
	int d = 0;	//ensure that addressing is correct
	int e = 0;	//ensure that addressing is correct
	char a[4];
	int b = 121318; //ensure that array is storing in the right direction on the stack
	a[3] = 198;

	return a[3] + c + d + e;
}