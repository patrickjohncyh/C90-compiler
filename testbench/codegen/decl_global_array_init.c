int a[5] = {90,12,334,30,3};

int f(){
	a[2] = 67;
}

int main(){
	f();
	return a[2];
}

