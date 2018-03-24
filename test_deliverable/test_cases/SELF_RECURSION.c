int sum(int x){
	if(x==0){
		return 0;
	}
	return x + sum(x-1);
}