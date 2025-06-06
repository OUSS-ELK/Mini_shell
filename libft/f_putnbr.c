#include<unistd.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	f_putnbr(int	nbr)
{
	long	nb;

	nb = (long)nbr;
	if (nb < 0)
	{
		ft_putchar('-');
		nb *= -1;
	}
	else if (nb > 9)
	{
		while (nb > 9){
			ft_putchar(nb / 10);
		}
	}
	else
		ft_putchar((nb % 10) + 48);
}

int main()
{
	f_putnbr(31);
//	write(1 "\n", 1);
//	f_putnbr(100);
//	write(1 "\n", 1);
//	f_putnbr(-2147483648);
//	write(1 "\n", 1);
//	f_putnbr(2147483647);
	return 0;
}
