int main(){
int a = 0;
while(a < 10){
int b=0;
while(b < 10){
b = b + 1;
if(b > 5)
break;
}
a = b + 7;
}
return a;
}