int f(int x){
	int a[5];
	a[x] = x*6;
	return a[x];
}

int f_init(int x){
	int a[5] = {10,20,30,40,50};
	a[x] = a[x] + 4;
	return a[x];
}

int f_init2(int x){
	int a[] = {10,20,30,40,50};
	a[x] = a[x] + 4;
	return a[x];
}

