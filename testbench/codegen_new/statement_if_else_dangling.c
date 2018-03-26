int main(){
    int a=1;   
    int c=0; 
    float b=0.001;
    float d=0;
    int res=0;
    if(a)
        if(c)
            res+=15;
        else
            res+=30;
    if(c)
        if(c)
            res+=15;
        else
            res+=30;
    if(b)
        if(d)
            res+=16;
        else
            res+=31;
    if(d)
        if(d)
            res+=17;
        else
            res+=33;
    return res;
}

/* 61 */