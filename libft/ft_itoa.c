/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 20:52:08 by vkaron            #+#    #+#             */
/*   Updated: 2019/09/14 19:57:36 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	check_negative(char *buf, int *iter, int *nbr)
{
	if (*nbr < 0)
	{
		buf[10] = '-';
		*iter = 9;
		if (*nbr == -2147483648)
		{
			buf[9] = '8';
			*nbr = -214748364;
			*iter = 8;
		}
		*nbr = *nbr * -1;
	}
}

static char	*return_str_from_buf(char *buf, int iter)
{
	char	*tmp;
	int		j;

	tmp = (char *)malloc(sizeof(char) * (11 - iter));
	if (!tmp)
		return (0);
	if (buf[10] == '-')
	{
		buf[iter] = '-';
		buf[10] = 0;
	}
	else
		iter++;
	j = 0;
	while (buf[j + iter])
	{
		tmp[j] = buf[j + iter];
		j++;
	}
	tmp[j] = 0;
	return (tmp);
}

char		*ft_itoa(int nbr)
{
	char			buf[12];
	int				i;

	i = 10;
	ft_pad_left(buf, 0, 12);
	if (nbr == 0)
		buf[i--] = '0';
	check_negative(buf, &i, &nbr);
	while (nbr > 0)
	{
		buf[i] = nbr % 10 + '0';
		nbr /= 10;
		i--;
	}
	return (return_str_from_buf(buf, i));
}
