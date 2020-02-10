/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabraham <vabraham@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 21:30:06 by vkaron            #+#    #+#             */
/*   Updated: 2020/02/10 19:24:25 by vabraham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "stdio.h"

void		add_link(t_isec *prev, t_isec *ins)
{
	prev->next = ins;
	ins->prev = prev;
}

void		add_intersection(t_isec **cisec, t_isec *isec)
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

void		cls_isec(t_isec **cisec, t_lst *lst, t_trc trc)
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
		sel_fig_check(lst, &hit, trc, cur_fig);
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

	res.r = clamp(((c & 0xff0000) >> 16) * koef, 0, 255);
	res.g = clamp(((c & 0xff00) >> 8) * koef, 0, 255);
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
	SDL_Color	refl_col;
	t_isec		*cisec;

	trc.min = MIN_OFFSET;
	trc.max = INFINITY;
	trc.d = refl_r(trc.d, n);
	refl_col = trace(lst, trc, depth, cisec);
	return (refl_col);
}

SDL_Color	get_refr_col(t_lst *lst, t_trc trc, t_vec3 n, float ito)
{
	// int			r_col;
	SDL_Color	refr_col;
	// float		angle;
	
	// angle = 10;
	trc.d = div_vec3f(trc.d, len_vec3(trc.d));
	// if (dot(trc.d, n) < 0)
	// 	// angle = -angle;
	// angle = angle * M_PI / 180;
	
	// refr_col.r = 255;
	// refr_col.g = 0;
	// refr_col.b = 0;

	// n = invert_vec3(n);
	// n = div_vec3f(n, len_vec3(n));

    float cosi = -clampf(dot(trc.d, n), -1.0, 1.0);

    float etai = 1.0;
    float etat = ito;
    // cosi = -dot(trc.d, n);
    if (cosi < 0)
    {
    	cosi = -cosi;
    
    	float tmp = etai;
    	etai = etat;
    	etat = tmp;
    	n = invert_vec3(n);
    } 
    float eta = etai / etat; 
    float k = 1 - eta * eta * (1 - cosi * cosi); 
    if (k < 0)
    	// k = 0.5;
    	// trc.d = cross(n, trc.d);
    	// angle=0;
    	// return(refr_col);
    	// trc.d = invert_vec3(trc.d);
    	trc.d = cre_vec3(1,0,0);
    else
    	trc.d = plus_vec3(mult_vec3f(trc.d, eta), mult_vec3f(n, (eta * cosi - sqrtf(k)))); 

    trc.d = div_vec3f(trc.d, len_vec3(trc.d));
    trc.d = invert_vec3(trc.d);


	// float cosi = MIN(1.0, dot(trc.p, n));

	trc.min = MIN_OFFSET;
	trc.max = INFINITY;
	t_isec *cisec;
	refr_col = trace(lst, trc, 0, cisec);
	return (refr_col);
}

/*
** set parametr for call light function
*/

t_l_prm		set_l_prm(t_trc trc, t_vec3 n)
{
	t_l_prm b;

	b.n = set_vec3(n);
	b.p = set_vec3(trc.p);
	b.v = set_vec3(trc.d);
	return (b);
}

void		get_normal_from_file(t_isec *cisec, t_lst *lst)
{
	t_vec3	norm;
	t_vec3	koeff_w_h;
	t_vec3	index;
	uint	n;

	koeff_w_h.w = cisec->fig->mat->norm_map.map->w;
	koeff_w_h.y = cisec->fig->mat->norm_map.map->h;
	index.x = cisec->uv.x * koeff_w_h.w;
	index.y = cisec->uv.y * koeff_w_h.y;
	index.z = clamp((int)(index.x + index.y * koeff_w_h.w),
		0, (int)(koeff_w_h.w * koeff_w_h.y));
	n = cisec->fig->mat->norm_map.data[(int)index.z];
	norm.x = 0.5 - ((n & 0xff0000) >> 16) / 255.0f;
	norm.y = 0.5 - ((n & 0xff00) >> 8) / 255.0f;
	norm.z = 1.0 - (n & 0xff) / 255.0f;
	cisec->n = minus_vec3(cisec->n, norm);
	cisec->n = div_vec3f(cisec->n, len_vec3(cisec->n));
}

SDL_Color	mix_color(SDL_Color c1, SDL_Color c2, float koef)
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

void		return_background0(t_vec3 *xuv, t_trc trc)
{
	t_vec3		dt;

	dt = div_vec3f(trc.d, len_vec3(trc.d));
	xuv->x = dot((t_vec3){0, 0.9999, 0, 0}, dt);
	xuv->y = acos(xuv->x);
	xuv->x = sin(acos(xuv->x));
	xuv->z = (acos(dot(dt, (t_vec3){0, 0, 1, 0}) / xuv->x)) / (2 * M_PI);
	if (dot(cross((t_vec3){0, 0.9999, 0, 0}, (t_vec3){0, 0, 1, 0}), dt) > 0)
		xuv->z = 1.0 - xuv->z;
	xuv->y = xuv->y / M_PI;
}

SDL_Color	return_background(t_lst *lst, t_trc trc)
{
	SDL_Color	res;
	t_vec3		xuv;
	int			index;

	if (lst->scn->fog.enable && lst->scn->fog.max_tr == 0)
		return (lst->scn->fog.col);
	if (!lst->scn->diff_map.map)
	{
		if (lst->scn->fog.enable)
			return (mix_color(lst->scn->bgc,
		lst->scn->fog.col, lst->scn->fog.max_tr));
		else
			return (lst->scn->bgc);
	}
	return_background0(&xuv, trc);
	index = clamp((int)(xuv.z * lst->scn->diff_map.map->w) + (int)(xuv.y *
		lst->scn->diff_map.map->h) * lst->scn->diff_map.map->w, 0,
		lst->scn->diff_map.map->w * lst->scn->diff_map.map->h);
	index = lst->scn->diff_map.data[index];
	res.r = clamp(((index & 0xff0000) >> 16), 0, 255);
	res.g = clamp(((index & 0xff00) >> 8), 0, 255);
	res.b = clamp((index & 0xff), 0, 255);
	if (lst->scn->fog.enable)
		return (mix_color(res, lst->scn->fog.col, lst->scn->fog.max_tr));
	return (res);
}

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
	t_isec *ci, SDL_Color *tres, float t)
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
	///////////
	if (ci->fig->mat->refr > 1)// || cur_isec->fig->type == plane)
	{
		trc->o = set_vec3(trc->p);
		SDL_Color refr_col = get_refr_col(lst, *trc, ci->n2, ci->fig->mat->refr);
		*tres = mix_color(refr_col, *tres, 1-t);
	}
}

int			init_trace(t_lst *lst, t_isec **cisec, t_trc trc, SDL_Color *col)
{
	*cisec = malloc(sizeof(t_isec));
	cls_isec(cisec, lst, trc);
	*col = return_background(lst, trc);
	if ((*cisec)->fig == NULL)
	{
		free(*cisec);
		return (1);
	}
	return (0);
}

int			idono(t_vec3 *fkt, SDL_Color *res,
	SDL_Color tres, t_isec **cur_isec)
{
	if (fkt->z < 1.0)
	{
		fkt->y = (1.0 - fkt->z) * fkt->x;
		*res = plus_sdl_color(*res, mult_sdl_color(tres, fkt->y));
		fkt->x -= fkt->y;
	}
	if ((fkt->x < 0.05) || fkt->z == 0.0)
		return (1);
	*cur_isec = (*cur_isec)->next;
	return (0);
}

void		init_trace0(t_lst *lst, int depth, t_vec3 *fkt, SDL_Color *res)
{
	fkt->x = 1.0f;
	*res = (SDL_Color){0, 0, 0};
	lst->depth = depth;
}

SDL_Color	return_trace(t_lst *lst, t_vec3 fkt, SDL_Color col, t_isec **cisec)
{
	if (fkt.x > 0)
		lst->res_help = plus_sdl_color(lst->res_help,
			mult_sdl_color(col, fkt.x));
	if (lst->scn->fog.enable)
		lst->res_help = mix_color(lst->scn->fog.col,
			lst->res_help, (*cisec)->t / lst->scn->fog.near);
	free_isec_list(*cisec);
	return (lst->res_help);
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
			cycle_trace(lst, &trc, cur_isec, &tres, fkt.z);
		if (idono(&fkt, &res, tres, &cur_isec))
			break ;
	}
	lst->res_help = res;
	return (return_trace(lst, fkt, col, &cisec));
}
