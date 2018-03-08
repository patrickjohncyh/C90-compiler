int factorial(int num)
{
    if (num == 0)
        return 1;
    else if(num==1)
    	return 1;
    else
    	return(num * factorial(num - 1));
}

int main(){
	return factorial(5);
}

