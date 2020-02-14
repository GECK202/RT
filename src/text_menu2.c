/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_menu2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabraham <vabraham@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/14 18:06:26 by vabraham          #+#    #+#             */
/*   Updated: 2020/02/14 18:14:42 by vabraham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	draw_type_lght(t_lst *l, int fsize, int x, int y)
{
	SDL_Rect d;

	d.x = x;
	d.y = y;
	if (l->scn->cur_lght->type == ambient)
		print_ttf(l->mimg, "AMBIENT", fsize, &d);
	else if (l->scn->cur_lght->type == direct)
		print_ttf(l->mimg, "DIRECT", fsize, &d);
	else if (l->scn->cur_lght->type == point)
		print_ttf(l->mimg, "POINT", fsize, &d);
	else if (l->scn->cur_lght->type == lconus)
		print_ttf(l->mimg, "CONUS", fsize, &d);
}

void	hide_menu_for_screen(t_lst *lst)
{
	if (lst->show_menu)
	{
		rain(lst);
		post_effects(lst);
	}
}
