int a;
int b;
int c;

int add(){
	return a+b;
}
int sub(){
	return a-b;
}
int divi(){
	return a/b;
}
int mult(){
	return a*b;
}
int comma(){
	return a,c,b;
}
int logicalNot(){
	return !a;
}
int logicalNotT(){
	return !c;
}
int directAssign(){
	int temp = a;
	temp = a + b;
	return temp;
}
int addAssign(){
	int temp =a;
	return temp += b;
}
int subAssign(){
	int temp=a;
	return temp-=b;
}
int divAssign(){
	int temp=a;
	return temp/=b;
}
int mulAssign(){
	int temp=a;
	return temp*=b;
}
int lt(){
	return a <b;
}
int gt(){
	return a>b;
}
int lte(){
	return a<=b;
}
int ltee(){
	return a<=a;
}
int gte(){
	return a>=b;
}
int gtee(){
	return a>=a;
}
int equality(){
	return a==a;
}
int equalityF(){
	return a==b;
}
int notEqualityF(){
	return a!=a;
}
int notEquality(){
	return a!=b;
}
int logicalOr(){
	return (a || (b+=5) );
}
int logicalOr2(){
	return (c || (b+=5) );
}
int logicalOr3(){
	return (c || c );
}
int logicalAnd(){
	return (c && (b+=5) );
}
int logicalAnd2(){
	return (a && (b+=5) );
}
int logicalAnd3(){
	return (c && c );
}
int preinc(){
	return ++b;
}
int predec(){
	return --b;
}
int postinc(){
	return b++;
}
int postdec(){
	return b--;
}
int array(){
	int g[]={1123,128,928};
	g[0] = a;
	g[2] = b;
	return g[0] + g[1] + g[2];
}

int ternary(){

}








