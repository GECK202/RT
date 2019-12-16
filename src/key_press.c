/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_press.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 22:07:39 by vabraham          #+#    #+#             */
/*   Updated: 2019/11/23 16:06:52 by vabraham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	cycle(t_lst *lst)
{
	while (lst->scn->cur_lght)
	{
		lst->scn->cur_lght = lst->scn->cur_lght->next;
		if (lst->scn->cur_lght && lst->scn->cur_lght->type == point)
			break ;
	}
	if (!lst->scn->cur_lght)
	{
		lst->scn->cur_lght = lst->scn->lghts;
		while (lst->scn->cur_lght)
		{
			if (lst->scn->cur_lght->type == point)
				break ;
			lst->scn->cur_lght = lst->scn->cur_lght->next;
		}
	}
}

int		mouse_light(t_lst *lst)
{
	if (!lst->mouse_light)
	{
		lst->scn->cur_lght = lst->scn->lghts;
		while (lst->scn->cur_lght)
		{
			if (lst->scn->cur_lght->type == point)
				break ;
			lst->scn->cur_lght = lst->scn->cur_lght->next;
		}
		if (!lst->scn->cur_lght)
			return (0);
		lst->mouse_light = 1;
	}
	else
		cycle(lst);
	return (1);
}

int		key_press1(int key, t_lst *lst)
{
	if (key == 3)
	{
		if (lst->mouse_light)
			lst->mouse_light = 0;
		else if (lst->scn->cur_fig && lst->scn->cur_fig->next)
			lst->scn->cur_fig = lst->scn->cur_fig->next;
		else
			lst->scn->cur_fig = lst->scn->figs;
	}
	else if (key == 49)
		lst->scn->shadow = (lst->scn->shadow) ? 0 : 1;
	else if (key == 37)
		multy_y(lst, 3);
	else if (key == 43)
		multy_z(lst, -3);
	else if (key == 47)
		multy_z(lst, 3);
	else if (key == 5 && !mouse_light(lst))
		return (0);
	rain(lst);
	mlx_put_image_to_window(lst->mlx, lst->win, lst->img, 0, 0);
	return (0);
}

int		key_press0(int key, t_lst *lst)
{
	if (key == 13 || key == 1)
	{
		lst->alpha_cam.x += 3;
		if (key == 13)
			lst->alpha_cam.x -= 6;
		ret(lst);
		set_m4_rx(&lst->camera_x, lst->alpha_cam.x);
	}
	else if (key == 0 || key == 2)
	{
		lst->alpha_cam.y -= 3;
		if (key == 2)
			lst->alpha_cam.y += 6;
		ret(lst);
		set_m4_ry(&lst->camera_y, lst->alpha_cam.y);
	}
	else if (key == 12 || key == 14)
	{
		lst->alpha_cam.z -= 3;
		if (key == 12)
			lst->alpha_cam.z += 6;
		ret(lst);
		set_m4_rz(&lst->camera_z, lst->alpha_cam.z);
	}
	return (key_press1(key, lst));
}

int		key_press(int key, void *l)
{
	t_lst	*lst;

	lst = (t_lst*)l;
	if (key == 53)
		exit(1);
	else if (key == 123)
		lst->scn->cam_pos0.x -= 1;
	else if (key == 124)
		lst->scn->cam_pos0.x += 1;
	else if (key == 126)
		lst->scn->cam_pos0.z += 1;
	else if (key == 125)
		lst->scn->cam_pos0.z -= 1;
	else if (key == 43)
		lst->scn->cam_pos0.y -= 1;
	else if (key == 47)
		lst->scn->cam_pos0.y += 1;
	else if (key == 34)
		multy_x(lst, -3);
	else if (key == 31)
		multy_x(lst, -3);
	else if (key == 40)
		multy_y(lst, -3);
	return (key_press0(key, lst));
}
