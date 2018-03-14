int f(int a, int b){
	return a + b;
}

int main(){
	return f(20,f(1,2));
}

