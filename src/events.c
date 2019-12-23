/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 22:08:27 by vabraham          #+#    #+#             */
/*   Updated: 2019/12/23 19:37:33 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int		close_window(void *l)
{
	t_lst	*lst;

	lst = (t_lst*)l;
	clean_opencl(&lst->ocl);
	exit(0);
}

int		mouse_move(int x, int y, void *l)
{
	t_lst *lst;

	lst = (t_lst*)l;
	if (!lst->change)
		return (0);
	if (lst->mouse_light)
		move_light(lst, x, y);
	else
		move_fig(lst, x, y);
	rain(lst);
	mlx_put_image_to_window(lst->mlx, lst->win, lst->img, 0, 0);
	return (0);
}

int		mouse_press0(int b, int x, int y, t_lst *lst)
{
	if (b == 5)
	{
		if (lst->mouse_light)
		{
			lst->scn->cur_lght->begin_pos.z -= 3;
			move_light(lst, x, y);
		}
		else
		{
			lst->scn->cur_fig->begin_pos.z -= 3;
			move_fig(lst, x, y);
		}
	}
	else if (b == 1 && lst->change && x >= 0 && y >= 0 && x < S_W && y < S_H)
		lst->change = 0;
	else if (b == 1 && x >= 0 && y >= 0 && x < S_W && y < S_H)
		lst->change = 1;
	rain(lst);
	mlx_put_image_to_window(lst->mlx, lst->win, lst->img, 0, 0);
	return (0);
}

int		mouse_press(int b, int x, int y, void *l)
{
	t_lst *lst;

	lst = (t_lst*)l;
	if (b == 4)
	{
		if (lst->mouse_light)
		{
			lst->scn->cur_lght->begin_pos.z += 3;
			move_light(lst, x, y);
		}
		else
		{
			lst->scn->cur_fig->begin_pos.z += 3;
			move_fig(lst, x, y);
		}
	}
	return (mouse_press0(b, x, y, lst));
}
