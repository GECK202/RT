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

/*
** calculate intersection and return figure and distatnce of intersection
*/

// void	cls_isec2(t_isec **cisec, t_lst *lst, t_trc trc)
// {
// 	t_fig	*cur_fig;
// 	t_vec3	t;

// 	(*cisec)->t = INFINITY;
// 	(*cisec)->uv.x = INFINITY;
// 	(*cisec)->uv.y = INFINITY;
// 	(*cisec)->fig = NULL;
// 	cur_fig = lst->scn->figs;
// 	int i = 0;
// 	while (cur_fig)
// 	{
		
// 		// sel_fig_check(&t, trc.o, trc.d, cur_fig);
		
// 		if (t.x >= trc.min && t.x <= trc.max && t.x < (*cisec)->t)
// 		{
// 			(*cisec)->t = t.x;
// 			(*cisec)->fig = cur_fig;
// 			if (t.z != INFINITY)//cisec->fig->type == sphere && 
// 			{
// 				(*cisec)->uv.x = t.w;
// 				(*cisec)->uv.y = t.z;
// 			}
// 			else
// 			{
// 				(*cisec)->uv.x = INFINITY;
// 				(*cisec)->uv.y = INFINITY;
// 			}
// 		}
// 		if (t.y != t.x && t.y >= trc.min && t.y <= trc.max && t.y < (*cisec)->t)
// 		{
// 			(*cisec)->t = t.y;
// 			(*cisec)->fig = cur_fig;
// 			if (t.z != INFINITY)//cisec->fig->type == sphere && 
// 			{
// 				(*cisec)->uv.x = t.w;
// 				(*cisec)->uv.y = t.z;
// 			}
// 			else
// 			{
// 				(*cisec)->uv.x = INFINITY;
// 				(*cisec)->uv.y = INFINITY;
// 			}
// 		}
		
		
		
// 		cur_fig = cur_fig->next;
// 		i++;
// 	}
// }

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

void	cls_isec(t_isec **cisec, t_lst *lst, t_trc trc)///////
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
		
		if ((hit.count == 1 || hit.count == 2) && hit.isec1->t >= trc.min && hit.isec1->t <= trc.max)
		{
			hit.isec1->prev = NULL;
			hit.isec1->next = NULL;
			add_intersection(cisec, hit.isec1);
			// if (hit.isec1 && hit.isec1->t >= trc.min && hit.isec1->t <= trc.max && hit.isec1->t < (*cisec)->t)
			// {
			// 	if ((*cisec) != NULL)
			// 		free((*cisec));
			// 	*cisec = hit.isec1;
			// }
		}

		if (hit.count == 2 && hit.isec2->t >= trc.min && hit.isec2->t <= trc.max)
		{
			hit.isec2->prev = NULL;
			hit.isec2->next = NULL;
			add_intersection(cisec, hit.isec2);
			// if (hit.isec2 && hit.isec2->t >= trc.min && hit.isec2->t <= trc.max && hit.isec2->t < (*cisec)->t)
			// {
			// 	if ((*cisec) != NULL)
			// 		free((*cisec));
			// 	*cisec = hit.isec2;
			// }
		}
		cur_fig = cur_fig->next;
	}
}

void	cls_isec3(t_isec **cisec, t_lst *lst, t_trc trc)///////
{
	t_fig	*cur_fig;
	t_hit	hit;

	(*cisec)->t = INFINITY;
	(*cisec)->uv.x = INFINITY;
	(*cisec)->uv.y = INFINITY;
	(*cisec)->fig = NULL;
	cur_fig = lst->scn->figs;
	while (cur_fig)
	{
		
		sel_fig_check(&hit, trc.o, trc.d, cur_fig);
		
		if (hit.count == 1 || hit.count == 2)
		{
			if (hit.isec1->t >= trc.min && hit.isec1->t <= trc.max && hit.isec1->t < (*cisec)->t)
			{
				free((*cisec));
				*cisec = hit.isec1;
			}
		}

		if (hit.count == 2)
		{
			if (hit.isec2->t >= trc.min && hit.isec2->t <= trc.max && hit.isec2->t < (*cisec)->t)
			{
				free((*cisec));
				*cisec = hit.isec2;
			}
		}
		cur_fig = cur_fig->next;
	}
}

/*
** calculate normal for intersection dot of figure
*/

// t_vec3	get_normal(t_isec *cisec, t_trc trc)
// {
// 	t_vec3	n;
// 	t_fig	fig;
// 	float	m;
// 	t_vec3	vt;
// 	t_vec3	c;

// 	fig = *(cisec->fig);
// 	n.x = 0;
// 	if (fig.type == plane)
// 		n = cisec->n;
// 		// n = set_vec3(fig.dir);
// 	else if (fig.type == sphere)
// 		n = cisec->n;
// 		// n = minus_vec3(trc.p, fig.pos);
// 	else if ((fig.type == cylinder) || (fig.type == conus))
// 		n = cisec->n;
// 	// {
// 	// 	vt = mult_vec3f(fig.dir, cisec->t);
// 	// 	c = set_vec3(fig.pos);
// 	// 	m = dot(trc.d, vt) + dot(minus_vec3(trc.o, c), fig.dir);
// 	// 	n = minus_vec3(minus_vec3(trc.p, c), mult_vec3f(fig.dir, m));
// 	// }
// 	return (div_vec3f(n, len_vec3(n)));
// }

/*
** calculate reflection color for dot
*/

SDL_Color	get_refl_col(t_lst *lst, t_trc trc, t_vec3 n, int depth)
{
	int			r_col;
	SDL_Color	refl_col;

	trc.min = MIN_OFFSET;
	trc.max = INFINITY;
	trc.d = set_vec3(refl_r(trc.d, n));
	r_col = trace(lst, trc, depth);
	refl_col.r = (r_col & 0xFF0000) >> 16;
	refl_col.g = (r_col & 0xFF00) >> 8;
	refl_col.b = r_col & 0xFF;
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
	int index = clamp(index_x + index_y * w, 0, w * h - 1);
	uint n = cisec->fig->mat->norm_map.data[index];
	
	norm.x = 0.5 - ((n & 0xff0000)>>16) /255.0f;
	norm.y = 0.5 - ((n & 0xff00)>>8) /255.0f;
	norm.z = 1.0 - (n & 0xff) /255.0f;//(n & 0xff) / 255.0f + norml.z * (koeff);

	cisec->n = minus_vec3(cisec->n, norm);
	cisec->n = div_vec3f(cisec->n, len_vec3(cisec->n));
	// return (norm);
}


int 	return_background(t_lst *lst, t_trc trc)
{
	SDL_Color	res;

	if (!lst->scn->diff_map.map)
		return (lst->scn->bgc);

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
	int index = clamp(index_x + index_y * w, 0, w * h - 1);
	int n = lst->scn->diff_map.data[index];
	res.r = clamp(((n & 0xff0000)>>16), 0, 255);
	res.g = clamp(((n & 0xff00)>>8), 0, 255);
	res.b = clamp((n & 0xff), 0, 255);
	return ((res.r << 16) + (res.g << 8) + res.b);
}


SDL_Color	get_color_from_file(t_map map, t_vec3 uv, float l)
{
	SDL_Color res;

	int w = map.map->w;
	int h = map.map->h;
	int index_x = (uv.x) * w;
	int index_y = (uv.y) * h;
	int index = clamp(index_x + index_y * w, 0, w * h - 1);
	int c = map.data[index];
	res.r = clamp(((c & 0xff0000)>>16) * l, 0, 255);
	res.g = clamp(((c & 0xff00)>>8) * l, 0, 255);
	res.b = clamp((c & 0xff) * l, 0, 255);
	return (res);
}

/*
** ray trace function
*/

int		trace(t_lst *lst, t_trc trc, int depth)
{
	SDL_Color	res;
	t_vec3		n;
	float		l;
	t_isec		*cisec;
	SDL_Color	refl_col;

	cisec = malloc(sizeof(t_isec));
	cls_isec(&cisec, lst, trc);

	int col = return_background(lst, trc);
	if (cisec->fig == NULL)
		return (col);
	
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
		if (cur_isec->fig->mat->transpare != 1.0)
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
				tres.r = clamp(cur_isec->fig->mat->col.r * l, 0, 255);
				tres.g = clamp(cur_isec->fig->mat->col.g * l, 0, 255);
				tres.b = clamp(cur_isec->fig->mat->col.b * l, 0, 255);
			}
			if (depth > 0 && cur_isec->fig->mat->refl > 0)
			{
				trc.o = set_vec3(trc.p);
				refl_col = get_refl_col(lst, trc, cur_isec->n, depth - 1);
				tres.r = res.r * (1 - cur_isec->fig->mat->refl) + refl_col.r * cur_isec->fig->mat->refl;
				tres.g = res.g * (1 - cur_isec->fig->mat->refl) + refl_col.g * cur_isec->fig->mat->refl;
				tres.b = res.b * (1 - cur_isec->fig->mat->refl) + refl_col.b * cur_isec->fig->mat->refl;
			}

			koef = (1.0 - cur_isec->fig->mat->transpare) * full;
			{
				res.r = clamp(res.r + koef * tres.r, 0, 255);
				res.g= clamp(res.g + koef * tres.g, 0, 255);
				res.b = clamp(res.b + koef * tres.b, 0, 255);
			}
			full -= koef;
		}
		if ((full < 0.05) || cur_isec->fig->mat->transpare == 0.0)
			break;
		cur_isec = cur_isec->next;
	}

	if (full > 0)
	{
		res.r = clamp(res.r + full * ((col & 0xff0000)>>16), 0, 255);
		res.g = clamp(res.g + full * ((col & 0xff00)>>8), 0, 255);
		res.b = clamp(res.b + full * (col & 0xff), 0, 255);
	}
	

	

	
	
	

	
	
	
	
	int color;
	// double kof = 1 - cisec->fig->mat->transpare, kof0 = cisec->fig->mat->transpare;
	color = (res.r << 16) + (res.g << 8) + res.b;
	return (color);
}
