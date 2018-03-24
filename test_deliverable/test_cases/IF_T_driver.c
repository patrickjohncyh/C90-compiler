int f(int x);

int main(){

	if(f(1) != 555 && f(-1) !=555 && f(93) !=555 && f(-191) != 555){
		return 1;
	}
	return 0;
}