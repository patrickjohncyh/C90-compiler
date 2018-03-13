int main(){
	int a = -1;
	( (a = a + 1) || (a = a + 5) );
	return a;
}