int main(){
	int a[3] = {4,8,12};
	int *b = &a;
	(*b) = 20;
	return a[0];
}