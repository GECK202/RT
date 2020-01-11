/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_fig2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 13:43:40 by vkaron            #+#    #+#             */
/*   Updated: 2020/01/12 00:55:24 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		set_rad_fig(t_lst *lst, char *word)
{
	if (!word)
		return (0);
	lst->scn->cur_fig->rad = ft_atof(word);
	return (1);
}

int		set_ang_fig(t_lst *lst, char *word)
{
	t_fig	*fig;

	if (!word)
		return (0);
	fig = lst->scn->cur_fig;
	if (fig->rad)
		return (1);
	fig->ang = ft_atof(word);
	if (fig->ang > -90 && fig->ang < 90)
	{
		fig->rad = tan(fig->ang * M_PI / 180);
		fig->rad = fig->rad * fig->rad + 1.0f;
	}
	else
		fig->rad = 33;
	return (1);
}
