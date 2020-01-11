/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 13:12:56 by vkaron            #+#    #+#             */
/*   Updated: 2020/01/09 20:49:14 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
** selection figure for check
*/

void	sel_fig_check(t_vec3 *t, t_vec3 o, t_vec3 d, t_fig *cur_fig)
{
	if (cur_fig->type == sphere)
		intersec_sph(t, o, d, cur_fig);
	else if (cur_fig->type == cylinder)
		intersec_cyl(t, o, d, cur_fig);
	else if (cur_fig->type == conus)
		intersec_con(t, o, d, cur_fig);
	else if (cur_fig->type == plane)
		intersec_pln(t, o, d, cur_fig);
}

/*
** intersection with sphere
*/

void	intersec_sph(t_vec3 *t, t_vec3 o, t_vec3 d, t_fig *sph)
{
	t_vec3	oc;
	t_vec3	k;
	float	discr;

	t->x = INFINITY;
	t->y = INFINITY;
	t->z = INFINITY;
	oc = minus_vec3(o, sph->pos);
	k.x = dot(d, d);
	k.y = dot(oc, d);
	k.z = dot(oc, oc) - sph->rad * sph->rad;
	discr = k.y * k.y - k.x * k.z;
	if (discr < 0)
		return ;
	discr = sqrt(discr);
	k.x = 1 / k.x;
	t->x = (discr - k.y) * k.x;
	t->y = (-discr - k.y) * k.x;
	
	if (t->x > t->y)
		t->x = t->y;
	
	t_vec3 Vn = cre_vec3(0, 1, 0);
	t_vec3 Ve = cre_vec3(0, 0, 1);
	t_vec3 Vp = minus_vec3(sph->pos, plus_vec3(o, mult_vec3f(d, t->x)));
	float lenvp = len_vec3(Vp);
	Vp = div_vec3f(Vp, lenvp);
	float x = -dot(sph->dir, Vp);
	t->z = acos(x);
	x = sin(t->z);
	t->w = (acos(dot(Vp, sph->look)/x)) / (2 * M_PI);
	if (dot(cross(sph->dir, sph->look), Vp) > 0)
		t->w = 1.0 - t->w;
	t->z = t->z / M_PI;
}

/*
** intersection with hyper_cylinder
*/

void	intersec_cyl(t_vec3 *t, t_vec3 o, t_vec3 d, t_fig *cyl)
{
	t_vec3	oc;
	t_vec3	k;
	t_vec3	v;
	float	discr;

	v = div_vec3f(cyl->dir, len_vec3(cyl->dir));
	t->x = INFINITY;
	t->y = INFINITY;
	oc = minus_vec3(o, cyl->pos);
	k.x = dot(d, d) - pow(dot(d, cyl->dir), 2);
	k.y = 2 * (dot(oc, d) - dot(d, cyl->dir) * dot(oc, cyl->dir));
	k.z = dot(oc, oc) - pow(dot(oc, cyl->dir), 2) - cyl->rad * cyl->rad;
	discr = k.y * k.y - 4 * k.x * k.z;
	if (discr < 0)
		return ;
	discr = sqrt(discr);
	k.x *= 2;
	t->x = (discr - k.y) / k.x;
	t->y = (-discr - k.y) / k.x;
}

/*
** intersection with plane
*/

void	intersec_pln(t_vec3 *t, t_vec3 o, t_vec3 d, t_fig *pln)
{
	t_vec3	oc;
	t_vec3	v;

	v = invert_vec3(div_vec3f(pln->dir, len_vec3(pln->dir)));
	t->x = INFINITY;
	t->y = INFINITY;
	if (dot(d, v) > 0)
	{
		oc = invert_vec3(minus_vec3(o, pln->pos));
		t->x = dot(oc, v) / dot(d, v);
	}
}

/*
** intersection with hyper_conus
*/

void	intersec_con(t_vec3 *t, t_vec3 o, t_vec3 d, t_fig *con)
{
	t_vec3	oc;
	t_vec3	k;
	t_vec3	v;
	float	discr;

	v = div_vec3f(con->dir, len_vec3(con->dir));
	t->x = INFINITY;
	t->y = INFINITY;
	oc = minus_vec3(o, con->pos);
	k.x = dot(d, d) - con->rad * pow(dot(d, con->dir), 2);
	k.y = 2 * (dot(oc, d) - con->rad * dot(d, con->dir) * dot(oc, con->dir));
	k.z = dot(oc, oc) - con->rad * pow(dot(oc, con->dir), 2);
	discr = k.y * k.y - 4 * k.x * k.z;
	if (discr < 0)
		return ;
	discr = sqrt(discr);
	k.x *= 2;
	t->x = (discr - k.y) / k.x;
	t->y = (-discr - k.y) / k.x;
}
