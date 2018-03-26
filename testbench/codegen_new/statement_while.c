int main(){
	int a=0;
	int b=0;
	float res = 10;
	while(a--){
		res+=5;
	}
	while(b--){
		res+=3;
	}

	while(a-5){
		a++;
		res++;
	}
	while(b-5){
		b+=1;
		res+=2.5;
	}
	return res;

}