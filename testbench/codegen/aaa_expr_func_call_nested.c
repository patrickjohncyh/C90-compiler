int f(int a,int b){
	return a + b;
}


int main(){
	return f(f(1,2), f(3,4));
}