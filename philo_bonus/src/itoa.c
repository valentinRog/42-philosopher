/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrogiste <vrogiste@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 13:06:03 by vrogiste          #+#    #+#             */
/*   Updated: 2022/03/25 16:37:28 by vrogiste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	number_of_digits(long long int n)
{
	if (n < 0)
		n *= -1;
	if (n < 10)
		return (1);
	return (1 + number_of_digits(n / 10));
}

static void	append_char(char c, char *buff)
{
	while (*buff)
		buff++;
	*buff = c;
}

static void	putnbr_buff(long long int n, char *buff)
{
	if (n < 0)
	{
		append_char('-', buff);
		n *= -1;
	}
	if (n >= 10)
		putnbr_buff(n / 10, buff);
	append_char(n % 10 + '0', buff);
}

char	*ft_itoa(int n)
{
	char	*dst;
	int		s_byte;

	s_byte = 0;
	if (n < 0)
		s_byte = 1;
	dst = calloc(number_of_digits(n) + 1 + s_byte, 1);
	if (!dst)
		return (NULL);
	putnbr_buff(n, dst);
	return (dst);
}
