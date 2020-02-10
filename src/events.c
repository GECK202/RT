/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 22:08:27 by vabraham          #+#    #+#             */
/*   Updated: 2020/01/09 20:34:17 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include <stdio.h>

int		close_window(void *lst)
{
	(void)lst;
	exit(0);
}

void	mouse_move(SDL_MouseMotionEvent *e, t_lst *lst)
{
	lst->cursor.x = e->x;
	lst->cursor.y = e->y;
	if (!lst->change)
		return ;
	if (lst->mouse_light)
		move_light(lst, lst->cursor);
	else
		move_fig(lst, lst->cursor);
}

void	mouse_weel(Sint32 y, t_lst *lst)
{
	if (lst->mouse_light)
	{
		lst->scn->cur_lght->begin_pos.z -= 3;
		if (y < 0)
			lst->scn->cur_lght->begin_pos.z += 6;
		move_light(lst, lst->cursor);
	}
	else
	{
		lst->scn->cur_fig->begin_pos.z -= 3;
		if (y < 0)
			lst->scn->cur_fig->begin_pos.z += 6;
		move_fig(lst, lst->cursor);
	}
}

void	mouse_press(SDL_MouseButtonEvent *e, t_lst *lst)
{
	if (e->button == SDL_BUTTON_LEFT && lst->change && e->x >= 0
		&& e->y >= 0 && e->x < S_W && e->y < S_H)
		lst->change = 0;
	else if (e->button == SDL_BUTTON_LEFT && e->x >= 0
		&& e->y >= 0 && e->x < S_W && e->y < S_H)
		lst->change = 1;
}
