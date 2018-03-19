int main(){
	int x = 0;
	int y = 0;
	int z = 0;
	while(x < 10){
		x = x + 1;
		y =0;
		if(x > 8)
			continue;
		while( y++ < 10){
			if(y > 7)
				continue;
			z++;
		}
	}
	return z;
}