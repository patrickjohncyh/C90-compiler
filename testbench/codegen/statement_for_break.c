int main(){
	int i=0;
	int j=0;
	int a=0; 
	for(i=0;i<5;i=i+1){
		for(j=0;j<5;j=j+1){
			if(j>2){
				break;
			}
			a=a+2;
		}
	}
	return a;
}