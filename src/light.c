/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabraham <vabraham@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 14:02:07 by vkaron            #+#    #+#             */
/*   Updated: 2020/02/10 19:04:58 by vabraham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
** calculate vector of reflection
*/

t_vec3		get_shadow(t_lst *lst, t_trc *trc, t_l_prm b, t_lght *c_lght)
{
	t_isec	*shdw;
	t_vec3	s;
	t_vec3	b_col;

	b_col = cre_vec3(c_lght->col.r * c_lght->ints / 255.0, c_lght->col.g
		* c_lght->ints / 255.0, c_lght->col.b * c_lght->ints / 255.0);
	if (c_lght->type == point || c_lght->type == lconus)
		trc_init(trc, c_lght, b);
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

float		get_diffuse(t_trc trc, t_l_prm b, t_lght *c_lght)
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

float		get_specular(t_trc trc, t_l_prm b, float f_spec, float l_ints)
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

t_vec3		get_sds(t_vec3 kof, float diff, float spec, t_vec3 ints)
{
	if (diff == -1 && spec == -1)
	{
		ints.x = clampf(ints.x, 0, 1);
		ints.y = clampf(ints.y, 0, 1);
		ints.z = clampf(ints.z, 0, 1);
	}
	else
	{
		if (kof.x > 0)
		{
			ints.x += kof.x * diff;
			ints.x += kof.x * spec;
		}
		if (kof.y > 0)
		{
			ints.y += kof.y * diff;
			ints.y += kof.y * spec;
		}
		if (kof.z > 0)
		{
			ints.z += kof.z * diff;
			ints.z += kof.z * spec;
		}
	}
	return (ints);
}

t_vec3		light(t_lst *lst, t_l_prm b, t_fig *fig, t_lght *c_lght)
{
	t_vec3	ints;
	t_trc	trc;

	trc = get_all_really(&ints, b, &c_lght, 1);
	while (c_lght)
	{
		if (c_lght->type == ambient)
			ints = plus_vec3(ints, (t_vec3){c_lght->ints * c_lght->col.r / 255,
c_lght->ints * c_lght->col.g / 255.0, c_lght->ints * c_lght->col.b / 255.0, 1});
		else
		{
			if (c_lght->type == lconus)
			{
				if (acos(dot(div_vec3f(minus_vec3(c_lght->pos, b.p), len_vec3(
minus_vec3(c_lght->pos, b.p))), c_lght->dir)) * 180 / M_PI > c_lght->angle)
					if (get_all_really(&ints, b, &c_lght, 0).min)
						continue;
			}
			ints = get_sds(get_shadow(lst, &trc, b, c_lght), get_diffuse(trc,
		b, c_lght), get_specular(trc, b, fig->mat->spec, c_lght->ints), ints);
		}
		ints = get_sds((t_vec3){0, 0, 0, 0}, -1, -1, ints);
		c_lght = c_lght->next;
	}
	return (ints);
}
