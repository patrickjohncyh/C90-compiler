int main(){
    int a=1;   
    int c=0; 
    int res=0;
    int x=5;
    if(c < a){
        res = res -10;
    }
    if(a || c){
        res = res - 67;
    }
    if( a && c+1 ){
        res = res - 1;
    }
    if( a && c || a ){
        res = res - 3;
    }
    if( a )
    if(a)
        if(c)
            res=res+5;
        else
            res=res+3;
    if(c)
        if(a)
            res=res+5;
        else
            res=res+(-3);

    a&&c;
    a||c;
    a<c;
    a==c;
    a*c;
    a-c;
    a+c;

    a=c+10;
    while(0<a){
        x=5;
        while(x<15){
            if(8 == x)
                c = c + 4;
            else if(9<x)
                c = c + 5;
            else{
                c=c+2;    
            }
                x=x+1;
        }
        a=a-1;
        c=c+1;
    }
    res=(res+c)*2;
    {
        int y=30;
        x = 40;
        x = x - y;
    }

    {
        int k = 4;
        x = x * k;
        return res-(k*x);
    }
   
    return 55;
}