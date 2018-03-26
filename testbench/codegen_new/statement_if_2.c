int main(){
	int a=0;	
	float b=0.000;
	int res=31;
	if(a){
		res+=23;
	}
	if(b){
		res+=17;
	}
	if(-b){
		res+=18;
	}
	return res;
}
/* ret 31 */