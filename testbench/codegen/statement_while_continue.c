int main(){
	int x = 0;
	int a = 1;
	while(x < 5){
		x = x+1;
		if(x > 2)
			continue;
		a = a + 5;
	}
	return a;
}