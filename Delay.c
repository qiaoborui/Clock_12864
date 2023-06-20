void Delay(unsigned int n)		//@12.000MHz 1ms
{
	unsigned char i, j;
	while(n--)
	{
		i = 2;
		j = 239;
		do
		{
			while (--j);
		} while (--i);		
	}
}
