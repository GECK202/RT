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


SDL_Color	get_color_from_file2(t_map map, t_vec3 uv)
{
	int index_x = (uv.x) * map.map->w;
	int index_y = (uv.y) * map.map->h;
	int index = clamp(index_x + index_y * map.map->w, 0, map.map->w * map.map->h);
	int c = map.data[index];

	SDL_Color res;

	res.r = clamp((c & 0xff0000)>>16, 0, 255);
	res.g = clamp((c & 0xff00)>>8, 0, 255);
	res.b = clamp(c & 0xff, 0, 255);
	return (res);
}

/*
** check shadow for current point or directional light
*/

t_vec3	transpare_shadow(t_isec *shdw, t_vec3 kof)
{
	if (!shdw)
		return (kof);
	else if (shdw->fig->mat->transpare == 0)
		return (cre_vec3(0,0,0));
	SDL_Color col;
	if (shdw->fig->mat->diff_map.map && shdw->uv.x && shdw->uv.x != INFINITY)
		col = get_color_from_file2(shdw->fig->mat->diff_map, shdw->uv);
	else
	{
		col.r = shdw->fig->mat->col.r;
		col.g = shdw->fig->mat->col.g;
		col.b = shdw->fig->mat->col.b;
	}

	// col.r = col.r * (1 - shdw->fig->mat->transpare); //255 - col.r;
	// col.g = col.g * (1 - shdw->fig->mat->transpare); //255 - col.g;
	// col.b = col.b * (1 - shdw->fig->mat->transpare); //255 - col.b;
	float tr = ((shdw->fig->mat->transpare) * 2 - 1.0) * 255;

	col.r = clamp(tr + col.r, 0, 255) * (shdw->fig->mat->transpare);
	col.g = clamp(tr + col.g, 0, 255) * (shdw->fig->mat->transpare);
	col.b = clamp(tr + col.b, 0, 255) * (shdw->fig->mat->transpare);

	kof.x = clampf(kof.x - 0.90 + (float)col.r / 255.0, 0, kof.x);
	kof.y = clampf(kof.y - 0.90 + (float)col.g / 255.0, 0, kof.y);
	kof.z = clampf(kof.z - 0.90 + (float)col.b / 255.0, 0, kof.z);

	// kof.x = clampf(kof.x - (((1.0 - shdw->fig->mat->transpare) * col.r) / 255.0),0,1);
	// kof.y = clampf(kof.y - (((1.0 - shdw->fig->mat->transpare) * col.g) / 255.0),0,1);
	// kof.z = clampf(kof.z - (((1.0 - shdw->fig->mat->transpare) * col.b) / 255.0),0,1);

	return (transpare_shadow(shdw->next, kof));
}


t_vec3	get_shadow(t_lst *lst, t_trc *trc, t_l_prm b, t_lght *c_lght)
{
	t_isec	*shdw;
	t_vec3	s;
	t_vec3	b_col;

	b_col = cre_vec3(\
		c_lght->col.r * c_lght->ints / 255.0,\
		c_lght->col.g * c_lght->ints/ 255.0,\
		c_lght->col.b * c_lght->ints/ 255.0);
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
	if (lst->scn->shadow && shdw->fig != NULL)
	{
		s = transpare_shadow(shdw, b_col);
		free_isec_list(shdw);
		return (s);
	}
	free(shdw);
	return (b_col);
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

	ints = cre_vec3(0,0,0);
	trc.min = MIN_OFFSET;
	c_lght = lst->scn->lghts;
	trc.o = set_vec3(b.p);
	
	// int fd = open("tmp.txt", O_RDWR);
	while (c_lght)
	{
		if (c_lght->type == ambient)
		{
			ints.x += (c_lght->ints * c_lght->col.r / 255.0);
			ints.y += (c_lght->ints * c_lght->col.g / 255.0);
			ints.z += (c_lght->ints * c_lght->col.b / 255.0);
		}
		else
		{
			// if (c_lght->type == point)
			// {
			// 	t_vec3 dl = minus_vec3(c_lght->pos, b.p);
			// 	dl = div_vec3f(dl, len_vec3(dl));
			// 	float al = acos(dot(dl, c_lght->dir))*180 /M_PI;
			// 	// printf("%f\n", al);
			// 	if (al > 90)
			// 	{
			// 		c_lght = c_lght->next;
			// 		continue;
			// 	}
			// }
			t_vec3 kof;
			kof = get_shadow(lst, &trc, b, c_lght);
			float diff = get_diffuse(trc, b, c_lght);
			float spec = get_specular(trc, b, fig->mat->spec, c_lght->ints);
			// printf("%f\n", kof.x);
			// kof = cre_vec3(1,1,1);
			// printf("[");
			if (kof.x > 0)
			{
				if (kof.x > 1.0)
					kof.x = 1.0;
				// printf("%f ", kof.x);
				ints.x += kof.x * diff;
				ints.x += kof.x * spec;
			}
			if (kof.y > 0)
			{
				if (kof.y > 1.0)
					kof.y = 1.0;
				// printf("%f ", kof.y);
				ints.y += kof.y * diff;
				ints.y += kof.y * spec;
			}
			if (kof.z > 0)
			{
				if (kof.z > 1.0)
					kof.z = 1.0;
				// printf("%f", kof.z);
				ints.z += kof.z * diff;
				ints.z += kof.z * spec;
			}
			// printf("]\n");
			// if (kof.x>0 && kof.y>0 && kof.z>0 && kof.x<1 && kof.y<1 && kof.z<1)
				// printf("[%f %f %f]\n", kof.x, kof.y, kof.z);
		}
		ints.x = clampf(ints.x,0,1);
		ints.y = clampf(ints.y,0,1);
		ints.z = clampf(ints.z,0,1);
		c_lght = c_lght->next;
	}
	// close(fd);
	return (ints);
}
