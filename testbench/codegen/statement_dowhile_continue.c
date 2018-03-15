int main(){
	int x = 0;
	int a = 5;
	do{
		x = x+1;
		if(x > 2)
			continue;
		a = a + 3;
	}while(x < 5);

	return a;
}