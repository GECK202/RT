/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 21:30:06 by vkaron            #+#    #+#             */
/*   Updated: 2020/01/14 17:37:21 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "stdio.h"

void add_link(t_isec *prev, t_isec *ins)
{
	prev->next = ins;
	ins->prev = prev;
	
}

void	add_intersection(t_isec **cisec, t_isec *isec)
{
	t_isec *cur;
	t_isec *prev;

	if ((*cisec)->fig == NULL)
	{
		free((*cisec));
		*cisec = isec;
		return ;
	}

	cur = *cisec;
	prev = NULL;
	if (!cur || isec->t < cur->t)
	{
		*cisec = isec;
		if (cur!=NULL)
		{
			add_link(isec, cur);
		}
		return ;
	}
	while (cur)
	{
		if (isec->t < cur->t)
		{
			add_link(prev, isec);
			add_link(isec, cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
	add_link(prev, isec);
}

void	cls_isec(t_isec **cisec, t_lst *lst, t_trc trc)
{
	t_fig	*cur_fig;
	t_hit	hit;

	(*cisec)->t = INFINITY;
	(*cisec)->uv.x = INFINITY;
	(*cisec)->uv.y = INFINITY;
	(*cisec)->fig = NULL;
	(*cisec)->n = cre_vec3(0,0,1);
	cur_fig = lst->scn->figs;
	while (cur_fig)
	{
		sel_fig_check(&hit, trc.o, trc.d, cur_fig);
		if (hit.count == 1 || hit.count == 2)
		{
			if (hit.isec1->t >= trc.min && hit.isec1->t <= trc.max)
			{
				hit.isec1->prev = NULL;
				hit.isec1->next = NULL;
				add_intersection(cisec, hit.isec1);
			}
			else
				free(hit.isec1);
		}
		if (hit.count == 2)
		{
			if (hit.isec2->t >= trc.min && hit.isec2->t <= trc.max)
			{
				hit.isec2->prev = NULL;
				hit.isec2->next = NULL;
				add_intersection(cisec, hit.isec2);
			}
			else
				free(hit.isec2);
		}
		cur_fig = cur_fig->next;
	}
}

SDL_Color	mult_sdl_color(SDL_Color col, float koef)
{
	SDL_Color res;

	res.r = clamp(col.r * koef, 0, 255);
	res.g = clamp(col.g * koef, 0, 255);
	res.b = clamp(col.b * koef, 0, 255);
	return (res);
}

SDL_Color	mult_int_color(int c, float koef)
{
	SDL_Color res;

	res.r = clamp(((c & 0xff0000)>>16) * koef, 0, 255);
	res.g = clamp(((c & 0xff00)>>8) * koef, 0, 255);
	res.b = clamp((c & 0xff) * koef, 0, 255);
	return (res);
}

SDL_Color	plus_sdl_color(SDL_Color col1, SDL_Color col2)
{
	SDL_Color res;

	res.r = clamp(col1.r + col2.r, 0, 255);
	res.g = clamp(col1.g + col2.g, 0, 255);
	res.b = clamp(col1.b + col2.b, 0, 255);
	return (res);
}

/*
** calculate reflection color for dot
*/

SDL_Color	get_refl_col(t_lst *lst, t_trc trc, t_vec3 n, int depth)
{
	// int			r_col;
	SDL_Color	refl_col;

	trc.min = MIN_OFFSET;
	trc.max = INFINITY;
	trc.d = refl_r(trc.d, n);//set_vec3()
	refl_col = trace(lst, trc, depth);
	// refl_col.r = (r_col & 0xFF0000) >> 16;
	// refl_col.g = (r_col & 0xFF00) >> 8;
	// refl_col.b = r_col & 0xFF;
	return (refl_col);
}

/*
** set parametr for call light function
*/

t_l_prm	set_l_prm(t_trc trc, t_vec3 n)
{
	t_l_prm b;

	b.n = set_vec3(n);
	b.p = set_vec3(trc.p);
	b.v = set_vec3(trc.d);
	return (b);
}


void	get_normal_from_file(t_isec *cisec, t_lst *lst)
{
	t_vec3	norm;
	float koeff = 0.1;

	int w = cisec->fig->mat->norm_map.map->w;
	int h = cisec->fig->mat->norm_map.map->h;
	int index_x = cisec->uv.x * w;
	int index_y = cisec->uv.y * h;
	int index = clamp(index_x + index_y * w, 0, w * h);
	uint n = cisec->fig->mat->norm_map.data[index];
	
	norm.x = 0.5 - ((n & 0xff0000)>>16) /255.0f;
	norm.y = 0.5 - ((n & 0xff00)>>8) /255.0f;
	norm.z = 1.0 - (n & 0xff) /255.0f;

	cisec->n = minus_vec3(cisec->n, norm);
	cisec->n = div_vec3f(cisec->n, len_vec3(cisec->n));
}

SDL_Color 	mix_color(SDL_Color c1, SDL_Color c2, float koef)
{
	SDL_Color	res;
	float		akoef;	

	koef = clampf(koef, 0, 1);
	akoef = 1.0 - koef;
	res.r = clamp(c1.r * koef + c2.r * akoef, 0, 255);
	res.g = clamp(c1.g * koef + c2.g * akoef, 0, 255);
	res.b = clamp(c1.b * koef + c2.b * akoef, 0, 255);
	return (res);
}

SDL_Color 	return_background(t_lst *lst, t_trc trc)
{
	SDL_Color	res;

	if (lst->scn->fog.enable && lst->scn->fog.max_tr == 0)
		return (lst->scn->fog.col);
	if (!lst->scn->diff_map.map)
	{
		if (lst->scn->fog.enable)
			return (mix_color(lst->scn->bgc, lst->scn->fog.col, lst->scn->fog.max_tr));
		else
			return (lst->scn->bgc);
	}
	t_vec3 up_cam = cre_vec3(0.0,0.9999,0.0);
	t_vec3 look_cam = cre_vec3(0.0,0.0,1.0);
	t_vec3 dt = trc.d;
	dt = div_vec3f(dt, len_vec3(dt));
		
	float x = dot(up_cam, dt);
	float u = acos(x);
	x = sin(u);
				
	float v = (acos(dot(dt, look_cam)/x)) / (2 * M_PI);
	if (dot(cross(up_cam, look_cam), dt) > 0)
		v = 1.0 - v;
	u = u / M_PI ;
	int w = lst->scn->diff_map.map->w;
	int h = lst->scn->diff_map.map->h;
	int index_x = v * w;
	int index_y = u * h;
	int index = clamp(index_x + index_y * w, 0, w * h);
	int n = lst->scn->diff_map.data[index];
	res.r = clamp(((n & 0xff0000)>>16), 0, 255);
	res.g = clamp(((n & 0xff00)>>8), 0, 255);
	res.b = clamp((n & 0xff), 0, 255);
	if (lst->scn->fog.enable)
		return (mix_color(res, lst->scn->fog.col, lst->scn->fog.max_tr));
	return (res);
	// return ((res.r << 16) + (res.g << 8) + res.b);
}

SDL_Color	get_color_from_file(t_map map, t_vec3 uv, t_vec3 l)
{
	int index_x = (uv.x) * map.map->w;
	int index_y = (uv.y) * map.map->h;
	int index = clamp(index_x + index_y * map.map->w, 0, map.map->w * map.map->h);
	int c = map.data[index];

	SDL_Color res;

	res.r = clamp(((c & 0xff0000)>>16) * l.x, 0, 255);
	res.g = clamp(((c & 0xff00)>>8) * l.y, 0, 255);
	res.b = clamp((c & 0xff) * l.z, 0, 255);
	return (res);
}

float	get_transp_from_file(t_map map, t_vec3 uv)
{
	int index_x = (uv.x) * map.map->w;
	int index_y = (uv.y) * map.map->h;
	int index = clamp(index_x + index_y * map.map->w, 0, map.map->w * map.map->h);
	int c = map.data[index];

	float res;

	// res.r = clamp(((c & 0xff0000)>>16) * l.x, 0, 255);
	res = 1.0 - clamp(((c & 0xff)), 0, 255) / 255.0;
	if (res > 0.5)
		res = 1;
	// res.b = clamp((c & 0xff) * l.z, 0, 255);
	return (res);
}

void free_isec_list(t_isec *cisec)
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

SDL_Color		trace(t_lst *lst, t_trc trc, int depth)
{
	SDL_Color	res;
	t_vec3		n;
	t_vec3		l;
	t_isec		*cisec;
	SDL_Color	refl_col;

	SDL_Color fog_col;
	fog_col.r = 180;
	fog_col.g = 200;
	fog_col.b = 200;

	// int res_fog = (fog_col.r << 16) + (fog_col.g << 8) + fog_col.b;

	cisec = malloc(sizeof(t_isec));
	cls_isec(&cisec, lst, trc);

	SDL_Color col = return_background(lst, trc);
	if (cisec->fig == NULL)
	{
		free(cisec);
		return (col);
	}
	
	SDL_Color	tres;
	t_isec *cur_isec;
	cur_isec = cisec;

	float full = 1.0f;
	float koef;
	res.r = 0;
	res.g = 0;
	res.b = 0;
	int flag = 0;
	while (cur_isec)
	{
		float transp;
		if (cur_isec->fig->mat->mask_map.map && cur_isec->uv.x && cur_isec->uv.x != INFINITY)
			transp = get_transp_from_file(cur_isec->fig->mat->mask_map, cur_isec->uv);
		else
			transp = cur_isec->fig->mat->transpare;

		if (transp < 1.0)
		{
			trc.p = plus_vec3(mult_vec3f(trc.d, cur_isec->t), (trc.o));
			
			if (cur_isec->fig->mat->norm_map.map && cur_isec->uv.x && cur_isec->uv.x != INFINITY)
				get_normal_from_file(cur_isec, lst);
			trc.d = invert_vec3(trc.d);
			l = light(lst, set_l_prm(trc, cur_isec->n), cur_isec->fig);

			if (cur_isec->fig->mat->diff_map.map && cur_isec->uv.x && cur_isec->uv.x != INFINITY)
				tres = get_color_from_file(cur_isec->fig->mat->diff_map, cur_isec->uv, l);
			else
			{
				// tres = mult_sdl_color(cur_isec->fig->mat->col, l);
				tres.r = clamp(cur_isec->fig->mat->col.r * l.x, 0, 255);
				tres.g = clamp(cur_isec->fig->mat->col.g * l.y, 0, 255);
				tres.b = clamp(cur_isec->fig->mat->col.b * l.z, 0, 255);
			}


			if (lst->scn->fog.enable)
			{
				float fog_n = cur_isec->t / lst->scn->fog.near;
				tres = mix_color(lst->scn->fog.col, tres, fog_n);
			}

			if (depth > 0 && cur_isec->fig->mat->refl > 0)
			{
				trc.o = set_vec3(trc.p);
				refl_col = get_refl_col(lst, trc, cur_isec->n, depth - 1);
				tres = plus_sdl_color(mult_sdl_color(tres, 1.0 - cur_isec->fig->mat->refl), mult_sdl_color(refl_col, cur_isec->fig->mat->refl));
			}
			koef = (1.0 - transp) * full;
			res = plus_sdl_color(res, mult_sdl_color(tres, koef));
			full -= koef;
		}
		if ((full < 0.05) || transp == 0.0)
			break;
		cur_isec = cur_isec->next;
	}
	if (full > 0)
		res = plus_sdl_color(res, mult_sdl_color(col, full));

	if (lst->scn->fog.enable)
	{
		float fog_n = cisec->t / lst->scn->fog.near;
		res = mix_color(lst->scn->fog.col, res, fog_n);
		// // printf("%f\n", fog_n);
		// if (fog_n > 1)
		// 	fog_n = 1;
		// res.r = res.r * (1 - fog_n) + fog_col.r * fog_n;
		// res.g = res.g * (1 - fog_n) + fog_col.g * fog_n;
		// res.b = res.b * (1 - fog_n) + fog_col.b * fog_n;
	}
	// int color;
	// color = (res.r << 16) + (res.g << 8) + res.b;
	
	free_isec_list(cisec);
	return (res);
}
