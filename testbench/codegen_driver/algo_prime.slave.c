int rand();

int power(int x, unsigned int y, int p)
{
    int res = 1;     
    x = x % p; 
    while (y > 0){
        if (y & 1)
            res = (res*x) % p;
        y = y>>1;
        x = (x*x) % p;
    }
    return res;
}

int miillerTest(int d, int n){
    int a = 2 + rand() % (n - 4);
    int x = power(a, d, n);
 
    if (x == 1  || x == n-1)
       return 1;
 
    while (d != n-1)
    {
        x = (x * x) % n;
        d *= 2;
 
        if (x == 1)      return 0;
        if (x == n-1)    return 1;
    }
 
    return 0;
}
 
int isPrime(int n, int k){
    int i;
    int d = n - 1;

    if (n <= 1 || n == 4)  return 0;
    if (n <= 3) return 1;

    while (d % 2 == 0)
        d /= 2;
    for ( i= 0; i < k; i++)
         if (miillerTest(d, n) == 0)
              return 0;
    return 1;
}
 
