int main(){
	int a =5;
	int *b = &a;
	*b = *b + 20;
	return a;

}