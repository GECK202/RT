/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postEffects.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabraham <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 16:59:46 by vabraham          #+#    #+#             */
/*   Updated: 2020/01/24 16:59:47 by vabraham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

SDL_Color	post_effects3(t_lst *lst, int i, SDL_Color res)
{
	if (lst->postEffects == 9)
	{
		res.g = (res.r + res.g + res.b) / 3;
		res.r = 0;
		res.b = 0;
	}
	else if (lst->postEffects == 10)
	{
		res.b = (res.r + res.g + res.b) / 3;
		res.g = 0;
		res.r = 0;
	}
	else if (lst->postEffects == 11)
		res = blur(lst->data_dop, i, 4, 4);
	else if (lst->postEffects == 12)
		res = pixel_picture(lst->data_dop, i, 7, 7);
	return (res);
}

SDL_Color	post_effects2(t_lst *lst, int i, SDL_Color res)
{
	if (lst->postEffects == 7)
	{
		if (res.r >= 128)
			res.r = 255;
		else
			res.r = 0;
		if (res.g >= 128)
			res.g = 255;
		else
			res.g = 0;
		if (res.b >= 128)
			res.b = 255;
		else
			res.b = 0;
	}
	else if (lst->postEffects == 8)
	{
		res.r = (res.r + res.g + res.b) / 3;
		res.g = 0;
		res.b = 0;
	}
	else
		res = post_effects3(lst, i, res);
	return (res);
}

SDL_Color	post_effects1(t_lst *lst, int i, SDL_Color res)
{
	if (lst->postEffects == 4)
	{
		res.r = (res.r + res.g + res.b) / 3;
		res.b = res.r;
		if (res.g < res.r)
		{
			res.b = res.g;
			res.g = res.r;
			res.r = res.b;
		}
	}
	else if (lst->postEffects == 5)
	{
		res.g = (res.r + res.g + res.b) / 3;
		res.b = res.g;
		if (res.r < res.g)
		{
			res.g = res.r;
			res.r = res.b;
			res.b = res.g;
		}
	}
	else
		res = post_effects2(lst, i, res);
	return (res);
}

SDL_Color	post_effects0(t_lst *lst, int i, SDL_Color res)
{
	if (lst->postEffects == 2 || lst->postEffects == 6)
	{
		res.r = (res.r + res.g + res.b) / 3;
		if (lst->postEffects == 6 && res.r >= 128)
			res.r = 255;
		else if (lst->postEffects == 6)
			res.r = 0;
		res.g = res.r;
		res.b = res.r;
	}
	else if (lst->postEffects == 3)
	{
		res.r = (res.r + res.g + res.b) / 3;
		res.g = res.r;
		if (res.b < res.r)
		{
			res.g = res.b;
			res.b = res.r;
			res.r = res.g;
		}
	}
	else
		res = post_effects1(lst, i, res);
	return (res);
}

void		post_effects(t_lst *lst)
{
	SDL_Color	res;
	int			i;

	i = -1;
	while (++i < S_W * S_H)
	{
		res.r = lst->data_dop[i] / 256 / 256;
		res.g = lst->data_dop[i] / 256 % 256;
		res.b = lst->data_dop[i] % (256 * 256);
		if (lst->postEffects == 1)
		{
			res.r = 255 - res.r;
			res.g = 255 - res.g;
			res.b = 255 - res.b;
		}
		else
			res = post_effects0(lst, i, res);
		lst->data[i] = (res.r << 16) + (res.g << 8) + res.b;
	}
}
