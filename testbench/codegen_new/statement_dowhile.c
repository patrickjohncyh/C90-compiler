int main(){
	int a=0;
	float b=0;
	int res = 0;
	do{
		res+=5;
	}while(a++);
	do{
		res+=20;
	}while(b++);

	do{
		res+=5;
		a--;
	}while(a+4);
	
	do{
		res+=2;
		b-=0.5;
	}while(b+5.5);


	return res;

}