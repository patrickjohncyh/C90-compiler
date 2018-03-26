int main(){
	int a=1;	
	float b=0.001;
	int c=-1;
	float d = -0.0001;
	int res=0;
	if(a){
		res+=23;
	}
	if(b){
		res+=17;
	}
	if(c){
		res+=24;
	}
	if(d){
		res+=18;
	}
	return res;
}
/*ret 82 */