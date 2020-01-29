/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 14:02:07 by vkaron            #+#    #+#             */
/*   Updated: 2020/01/12 00:53:16 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include <stdio.h>

void	cls_isec3(t_isec **cisec, t_lst *lst, t_trc trc);
void	free_isec_list(t_isec *cisec);
/*
** calculate vector of reflection
*/

t_vec3	refl_r(t_vec3 l, t_vec3 n)
{
	float	k;
	t_vec3	r;

	k = 2 * dot(n, l);
	r.x = k * n.x - l.x;
	r.y = k * n.y - l.y;
	r.z = k * n.z - l.z;
	return (r);
}

/*
** check shadow for current point or directional light
*/

SDL_Color	get_color_from_file_shadow(t_map map, t_vec3 uv)
{
	int index_x = (uv.x) * map.map->w;
	int index_y = (uv.y) * map.map->h;
	int index = clamp(index_x + index_y * map.map->w, 0, map.map->w * map.map->h);
	int c = map.data[index];

	SDL_Color res;
	res.r = (c & 0xff0000)>>16;
	res.g = (c & 0xff00)>>8;
	res.b = c & 0xff;
	return (res);
}

t_vec3	colorFig(t_lst *lst, t_isec *shdw, t_trc trc)
{
	SDL_Color	color;

	trc.p = plus_vec3(mult_vec3f(trc.d, shdw->t), (trc.o));
			
	if (shdw->fig->mat->norm_map.map && shdw->uv.x && shdw->uv.x != INFINITY)
		get_normal_from_file(shdw, lst);
	trc.d = invert_vec3(trc.d);

	if (shdw->fig->mat->diff_map.map && shdw->uv.x && shdw->uv.x != INFINITY)
		color = get_color_from_file_shadow(shdw->fig->mat->diff_map, shdw->uv);
	else
	{
		color.r = shdw->fig->mat->col.r;
		color.g = shdw->fig->mat->col.g;
		color.b = shdw->fig->mat->col.b;
	}

	t_vec3 vec;
	vec.x = 1 - (float)color.r / 255;
	vec.y = 1 - (float)color.g / 255;
	vec.z = 1 - (float)color.b / 255;

	return (vec);
}

t_vec3	transpare_shadow(t_lst *lst, t_isec *shdw, float kof, t_vec3 color, t_trc *trc)
{
	if (!shdw)
		return (color);
	else if (shdw->fig->mat->transpare == 0)
	{
		color.x = 0;
		color.y = 0;
		color.z = 0;
		return (color);
	}
	kof *= 1 - shdw->fig->mat->transpare;
	t_vec3 new_color = colorFig(lst, shdw, *trc);
	new_color.x *= kof;
	new_color.y *= kof;
	new_color.z *= kof;
	color.x -= new_color.x;
	color.y -= new_color.y;
	color.z -= new_color.z;
	// color.x = kof;
	// color.y = kof;
	// color.z = kof;
	return (transpare_shadow(lst, shdw->next, kof, color, trc));
}



t_vec3	get_shadow(t_lst *lst, t_trc *trc, t_l_prm b, t_lght *c_lght)
{
	t_isec		*shdw;

	if (c_lght->type == point)
	{
		trc->d.x = c_lght->pos.x - b.p.x;
		trc->d.y = c_lght->pos.y - b.p.y;
		trc->d.z = c_lght->pos.z - b.p.z;
		trc->max = 1;
	}
	else if (c_lght->type == direct)
	{
		trc->d.x = c_lght->dir.x;
		trc->d.y = c_lght->dir.y;
		trc->d.z = c_lght->dir.z;
		trc->max = INFINITY;
	}
	shdw = malloc(sizeof(t_isec));
	cls_isec(&shdw, lst, *trc);
	
	t_vec3 color;
	color.x = 1;
	color.y = 1;
	color.z = 1;
	if (lst->scn->shadow && shdw->fig != NULL)
	{
		color = transpare_shadow(lst, shdw, 1, color, trc);
		free_isec_list(shdw);
		return (color);
	}
	free(shdw);
	color.x = 1;
	color.y = 1;
	color.z = 1;
	return (color);
}

/*
** calculate brightness for point or directional light
*/

float	get_diffuse(t_trc trc, t_l_prm b, t_lght *c_lght)
{
	float	n_dot;
	float	ints;

	ints = 0.0f;
	n_dot = dot(b.n, trc.d);
	if ((n_dot) > 0)
		ints += (c_lght->ints * n_dot) / (len_vec3(b.n) * len_vec3(trc.d));
	return (ints);
}

/*
** calculate specular for point or directional light
*/

float	get_specular(t_trc trc, t_l_prm b, float f_spec, float l_ints)
{
	t_vec3	r;
	float	r_dot;
	float	spec;
	float	ints;

	ints = 0.0f;
	if (f_spec > 0)
	{
		r = refl_r(trc.d, b.n);
		r_dot = dot(r, b.v);
		if (r_dot > 0)
		{
			spec = r_dot / (len_vec3(r) * len_vec3(b.v));
			spec = pow(spec, f_spec);
			ints += l_ints * spec;
		}
	}
	return (ints);
}

/*
** calculate intenses of lights for current dot
*/

t_vec3	light(t_lst *lst, t_l_prm b, t_fig *fig)
{
	t_vec3	ints;
	t_trc	trc;
	t_lght	*c_lght;

	ints.x = 0;
	ints.y = 0;
	ints.z = 0;
	trc.min = MIN_OFFSET;
	c_lght = lst->scn->lghts;
	trc.o = set_vec3(b.p);
	while (c_lght)
	{
		if (c_lght->type == ambient){
			ints.x += c_lght->ints;
			ints.y += c_lght->ints;
			ints.z += c_lght->ints;
		}
		else
		{
			t_vec3 shadow = get_shadow(lst, &trc, b, c_lght);
			// if ((kof = get_shadow(lst, &trc, b, c_lght)) > 0)
			if (shadow.x > 0 && shadow.y > 0 && shadow.z > 0)
			{
				float diff = get_diffuse(trc, b, c_lght);
				float spec = get_specular(trc, b, fig->mat->spec, c_lght->ints);
				// if (shadow.x > 1)
				// 	shadow.x = 1;
				// if (shadow.y > 1)
				// 	shadow.y = 1;
				// if (shadow.z > 1)
				// 	shadow.z = 1;
				if (shadow.x < 0 || shadow.y < 0 || shadow.z < 0)
					write(1, "1", 1);
				ints.x += diff * shadow.x + spec * shadow.x;
				ints.y += diff * shadow.y + spec * shadow.y;
				ints.z += diff * shadow.z + spec * shadow.z;
				// ints += kof * get_diffuse(trc, b, c_lght);
				// ints += kof * get_specular(trc, b, fig->mat->spec, c_lght->ints);
			}
		}
		c_lght = c_lght->next;
	}
	if (ints.x > 1)
		ints.x = 1;
	if (ints.y > 1)
		ints.y = 1;
	if (ints.z > 1)
		ints.z = 1;
	if (ints.x > 1 || ints.y > 1 || ints.z > 1 || ints.x < 0 || ints.y < 0 || ints.z < 0)
	write(1, "1", 1);
	// ints.r = clamp(ints.r, 0, 255);
	// ints.g = clamp(ints.g, 0, 255);
	// ints.b = clamp(ints.b, 0, 255);

	// t_vec3 vec;
	// vec.x = (float)ints.r / 255;
	// vec.y = (float)ints.g / 255;
	// vec.z = (float)ints.b / 255;
	return (ints);
}
