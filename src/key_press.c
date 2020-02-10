/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_press.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 22:07:39 by vabraham          #+#    #+#             */
/*   Updated: 2020/01/09 19:38:16 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

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

int		key_press1(SDL_Keycode key, t_lst *lst)
{
	if (key == SDLK_f)
	{
		if (lst->mouse_light)
			lst->mouse_light = 0;
		else if (lst->scn->cur_fig && lst->scn->cur_fig->next)
			lst->scn->cur_fig = lst->scn->cur_fig->next;
		else
			lst->scn->cur_fig = lst->scn->figs;
		return (0);
	}
	else if (key == SDLK_SPACE)
		lst->scn->shadow = (lst->scn->shadow) ? 0 : 1;
	else if (key == SDLK_l)
		multy_y(lst, 3);
	else if (key == SDLK_y)
		multy_z(lst, -3);
	else if (key == SDLK_t)
		multy_z(lst, 3);
	else if (key == SDLK_g && !mouse_light(lst))
		return (0);
	return (1);
}

int		key_press0(SDL_Keycode key, t_lst *lst)
{
	if (key == SDLK_w || key == SDLK_s)
	{
		lst->alpha_cam.x += 3;
		if (key == SDLK_w)
			lst->alpha_cam.x -= 6;
		ret(lst);
		set_m4_rx(&lst->camera_x, lst->alpha_cam.x);
	}
	else if (key == SDLK_a || key == SDLK_d)
	{
		lst->alpha_cam.y -= 3;
		if (key == SDLK_d)
			lst->alpha_cam.y += 6;
		ret(lst);
		set_m4_ry(&lst->camera_y, lst->alpha_cam.y);
	}
	else if (key == SDLK_q || key == SDLK_e)
	{
		lst->alpha_cam.z -= 3;
		if (key == SDLK_q)
			lst->alpha_cam.z += 6;
		ret(lst);
		set_m4_rz(&lst->camera_z, lst->alpha_cam.z);
	}
	return (key_press1(key, lst));
}

int		key_press(SDL_Keycode key, t_lst *lst)
{
	if (key == 8)
		return (save_scene(lst));
	else if (key == 13)
		return (scrin(lst));
	else if (key == 91)
	{
		lst->postEffects -= 1;
		if (lst->postEffects < 0)
			lst->postEffects = END_FOR_POST_EFFECTS;
		return (0);
	}
	else if (key == 93)
	{
		lst->postEffects += 1;
		if (lst->postEffects > END_FOR_POST_EFFECTS)
			lst->postEffects = 0;
		return (0);
	}
	return (key_press_dop(key, lst));
}
