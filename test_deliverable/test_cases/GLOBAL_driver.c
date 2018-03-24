void f(int x);
extern int a;

int main(){
	f(938);
	if(a != 938){
		return 1;
	} 

	return 0;
}