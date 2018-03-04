int f();
extern int a;

int main(){
	if(f() != 1)
		return 1;
	if(a != 0)
		return 2;
	return 0;
}