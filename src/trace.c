/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabraham <vabraham@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 21:30:06 by vkaron            #+#    #+#             */
/*   Updated: 2020/02/10 22:12:55 by vabraham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

SDL_Color	get_color_from_file(t_map map, t_vec3 uv, t_vec3 l)
{
	int				index_x;
	int				index_y;
	unsigned int	index;
	int				c;
	SDL_Color		res;

	index_x = (uv.x) * map.map->w;
	index_y = (uv.y) * map.map->h;
	index = clamp(index_x + index_y * map.map->w, 1,
		map.map->w * map.map->h - 1);
	c = map.data[index];
	res.r = clamp(((c & 0xff0000) >> 16) * l.x, 0, 255);
	res.g = clamp(((c & 0xff00) >> 8) * l.y, 0, 255);
	res.b = clamp((c & 0xff) * l.z, 0, 255);
	return (res);
}

float		get_transp_from_file(t_map map, t_vec3 uv)
{
	int		index_x;
	int		index_y;
	int		index;
	int		c;
	float	res;

	index_x = (uv.x) * map.map->w;
	index_y = (uv.y) * map.map->h;
	index = clamp(index_x + index_y * map.map->w, 0, map.map->w * map.map->h);
	c = map.data[index];
	res = 1.0 - clamp(((c & 0xff)), 0, 255) / 255.0;
	if (res > 0.5)
		res = 1;
	return (res);
}

void		free_isec_list(t_isec *cisec)
{
	t_isec	*tmp;

	while (cisec)
	{
		tmp = cisec;
		cisec = cisec->next;
		free(tmp);
	}
}

/*
** ray trace function
*/

void		cycle_trace(t_lst *lst, t_trc *trc,
	t_isec *ci, SDL_Color *tres)
{
	t_vec3 l;

	trc->p = plus_vec3(mult_vec3f(trc->d, ci->t), (trc->o));
	if (ci->fig->mat->norm_map.map && ci->uv.x && ci->uv.x != INFINITY)
		get_normal_from_file(ci, lst);
	trc->d = invert_vec3(trc->d);
	l = light(lst, set_l_prm(*trc, ci->n), ci->fig, lst->scn->lghts);
	if (ci->fig->mat->diff_map.map && ci->uv.x && ci->uv.x != INFINITY)
		*tres = get_color_from_file(ci->fig->mat->diff_map, ci->uv, l);
	else
	{
		tres->r = clamp(ci->fig->mat->col.r * l.x, 0, 255);
		tres->g = clamp(ci->fig->mat->col.g * l.y, 0, 255);
		tres->b = clamp(ci->fig->mat->col.b * l.z, 0, 255);
	}
	if (lst->scn->fog.enable)
		*tres = mix_color(lst->scn->fog.col, *tres, ci->t / lst->scn->fog.near);
	trc->o = set_vec3(trc->p);
	if (lst->depth > 0 && ci->fig->mat->refl > 0)
		*tres = plus_sdl_color(mult_sdl_color(*tres, 1.0 - ci->fig->mat->refl),
			mult_sdl_color(get_refl_col(lst, *trc,
			ci->n, lst->depth - 1), ci->fig->mat->refl));
	check_refr(lst, trc, ci, tres);
}

SDL_Color	trace(t_lst *lst, t_trc trc, int depth, t_isec *cisec)
{
	SDL_Color	res;
	t_vec3		fkt;
	SDL_Color	tres;
	t_isec		*cur_isec;
	SDL_Color	col;

	if (init_trace(lst, &cisec, trc, &col))
		return (col);
	cur_isec = cisec;
	init_trace0(lst, depth, &fkt, &res);
	while (cur_isec)
	{
		if (cur_isec->fig->mat->mask_map.map &&
			cur_isec->uv.x && cur_isec->uv.x != INFINITY)
			fkt.z = get_transp_from_file(cur_isec->fig->mat->mask_map,
				cur_isec->uv);
		else
			fkt.z = cur_isec->fig->mat->transpare;
		if (fkt.z < 1.0)
			cycle_trace(lst, &trc, cur_isec, &tres);
		if (idono(&fkt, &res, tres, &cur_isec))
			break ;
	}
	lst->res_help = res;
	return (return_trace(lst, fkt, col, &cisec));
}
