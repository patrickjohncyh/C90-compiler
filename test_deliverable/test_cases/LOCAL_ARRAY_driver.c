int f(int x);
int f_init(x);
int f_init2(x);

int main(){

	if(f(3) != 18 && f_init(3) != 44 && f_init2(3) != 44 ){
		return 1;
	}
	return 0;
}