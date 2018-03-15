int main(){
	int a=0;
	int b=1;
	do{
		b = b + b;
		a = a + 1;
		if(a > 3)
			break;
	}while(a < 5);

	return b;

}