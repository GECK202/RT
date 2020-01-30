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

t_vec3	transpare_shadow(t_isec *shdw, t_vec3 kof)
{
	if (!shdw)
		return (kof);
	else if (shdw->fig->mat->transpare == 0)
		return (cre_vec3(0,0,0));
	SDL_Color col;
	col.r = 255 - shdw->fig->mat->col.r;
	col.g = 255 - shdw->fig->mat->col.g;
	col.b = 255 - shdw->fig->mat->col.b;
	kof.x = kof.x - (((1.0 - shdw->fig->mat->transpare) * col.r) / 512.0);
	kof.y = kof.y - (((1.0 - shdw->fig->mat->transpare) * col.g) / 512.0);
	kof.z = kof.z - (((1.0 - shdw->fig->mat->transpare) * col.b) / 512.0);
	return (transpare_shadow(shdw->next, kof));
}


t_vec3	get_shadow(t_lst *lst, t_trc *trc, t_l_prm b, t_lght *c_lght)
{
	t_isec	*shdw;
	t_vec3	s;

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
		s = transpare_shadow(shdw, cre_vec3(1,1,1));
		free_isec_list(shdw);
		return (s);
	}
	free(shdw);
	return (cre_vec3(c_lght->col.r / 255.0,c_lght->col.g / 255.0,c_lght->col.b / 255.0));
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
			t_vec3 kof;
			kof = get_shadow(lst, &trc, b, c_lght);
			if (kof.x > 0 && kof.y > 0.5 && kof.z > 0.5)
			{
				ints.x += kof.x * get_diffuse(trc, b, c_lght);
				ints.x += kof.x * get_specular(trc, b, fig->mat->spec, c_lght->ints);
			}
			if (kof.y > 0 && kof.x > 0.5 && kof.z > 0.5)
			{
				ints.y += kof.y * get_diffuse(trc, b, c_lght);
				ints.y += kof.y * get_specular(trc, b, fig->mat->spec, c_lght->ints);
			}
			if (kof.z > 0 && kof.y > 0.5 && kof.x > 0.5)
			{
				ints.z += kof.z * get_diffuse(trc, b, c_lght);
				ints.z += kof.z * get_specular(trc, b, fig->mat->spec, c_lght->ints);
			}
		}
		c_lght = c_lght->next;
	}
	return (ints);
}
