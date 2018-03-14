void f(int a[2]){
	a[1] = 123;
}

int main(){
	int a[2] = {10,20};
	f(a);
	return a[1];
}

