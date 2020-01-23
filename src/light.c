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

float	get_shadow(t_lst *lst, t_trc *trc, t_l_prm b, t_lght *c_lght)
{
	t_isec	*shdw;

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
	shdw = NULL;
	cls_isec(&shdw, lst, *trc);
	if (shdw && lst->scn->shadow && shdw->fig != NULL)
	{
		return 1;
	}
	return (0);
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

float	light(t_lst *lst, t_l_prm b, t_fig *fig)
{
	float	ints;
	t_trc	trc;
	t_lght	*c_lght;

	ints = 0.0f;
	trc.min = MIN_OFFSET;
	c_lght = lst->scn->lghts;
	trc.o = set_vec3(b.p);
	while (c_lght)
	{
		if (c_lght->type == ambient)
			ints += c_lght->ints;
		else
		{
			float kof;
			if ((kof = get_shadow(lst, &trc, b, c_lght)) <= 1.0)
			{
				ints += (kof) * get_diffuse(trc, b, c_lght);
				ints += (kof) * get_specular(trc, b, fig->mat->spec, c_lght->ints);
			}
		}
		c_lght = c_lght->next;
	}
	return (ints);
}
