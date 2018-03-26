int x = 12;
float y = 5.1234;

int main(){
	float z = x + y;
	{
		int x = 9;
		z+=x;
		{
			float x = - 2.9;
			z+=x;
			{
				int y = -2.596;
				z+=y;
				{
					int z=0;
				}
				{
					float y = -5.9;	
					z+=y;
				}
				z*=-y;
			}
			z+=x;
		}
		z+=x;
	}
	z+=(x+y);

	return z;
}