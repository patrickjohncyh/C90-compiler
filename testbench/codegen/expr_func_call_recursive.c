int factorial(int num)
{
    if (num <2)
    	return 1;
    else
    	return(num * factorial(num - 1));
}

int main(){
	return factorial(5);
}

