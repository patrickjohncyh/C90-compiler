void f();
extern int a;

int main(){
	a=0;
	f();
	a = a + 5;
	if(a != 943){
		return 1;
	} 
	return 0;
}