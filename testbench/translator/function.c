int x = 15;

int f0(){
	return 14;
}
void f1(int a){
	x = x + a;

}
int f3(int a, int b, int c){
	return 1 + 2 * 3;
}

int main(){
	f1(20);
	return f0() +  f3(1,2,3);
}