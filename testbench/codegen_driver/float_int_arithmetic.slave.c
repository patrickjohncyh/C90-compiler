float a;
int b;
float c;
float g[]={145,128,12304};

float add(){
	return a+b;
}
float sub(){
	return a-b;
}
float divi(){
	return a/b;
}
float mult(){
	return a*b;
}
float comma(){
	return a,c,b;
}
float logicalNot(){
	return !a;
}
float logicalNotT(){
	return !c;
}
float directAssign(){
	float temp = a;
	temp = a + b;
	return temp;
}
float addAssign(){
	float temp =a;
	return temp += b;
}
float subAssign(){
	float temp=a;
	return temp-=b;
}
float divAssign(){
	float temp=a;
	return temp/=b;
}
float mulAssign(){
	float temp=a;
	return temp*=b;
}
float lt(){
	return a <b;
}
float gt(){
	return a>b;
}
float lte(){
	return a<=b;
}
float ltee(){
	return a<=a;
}
float gte(){
	return a>=b;
}
float gtee(){
	return a>=a;
}
float equality(){
	return a==a;
}
float equalityF(){
	return a==b;
}
float notEqualityF(){
	return a!=a;
}
float notEquality(){
	return a!=b;
}
float logicalOr(){
	return (a || (b+=5) );
}
float logicalOr2(){
	return (c || (b+=5) );
}
float logicalOr3(){
	return (c || c );
}
float logicalAnd(){
	return (c && (b+=5) );
}
float logicalAnd2(){
	return (a && (b+=5) );
}
float logicalAnd3(){
	return (c && c );
}
float preinc(){
	return ++b;
}
float predec(){
	return --b;
}
float postinc(){
	return b++;
}
float postdec(){
	return b--;
}
float array(){

	g[0] = a;
	g[2] = b;
	return g[0] + g[2] + g[1] ;
}

float ternary(){

}








