/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 13:12:56 by vkaron            #+#    #+#             */
/*   Updated: 2020/01/14 15:36:47 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "stdio.h"

/*
** selection figure for check
*/

void	sel_fig_check(t_hit *hit, t_vec3 o, t_vec3 d, t_fig *cur_fig)
{
	if (cur_fig->type == sphere)
		intersec_sph(hit, o, d, cur_fig);
	else if (cur_fig->type == cylinder)
		intersec_cyl(hit, o, d, cur_fig);
	else if (cur_fig->type == conus)
		intersec_con(hit, o, d, cur_fig);
	else if (cur_fig->type == plane)	
		intersec_pln(hit, o, d, cur_fig);
}



void	fill_isec_sphere(t_isec *i, t_vec3 o, t_vec3 d, t_fig *sph)
{
	t_vec3	vp;
	float	x;

	i->fig = sph;
	i->n = minus_vec3(plus_vec3(mult_vec3f(d, i->t), o), sph->pos);
	i->n = div_vec3f(i->n, len_vec3(i->n));
	vp = minus_vec3(sph->pos, plus_vec3(o, mult_vec3f(d, i->t)));
	vp = div_vec3f(vp, len_vec3(vp));
	x = -dot(sph->dir, vp);
	i->uv->y = acos(x);
	x = sin(i->uv->y);
	i->uv->x = (acos(dot(vp, sph->look)/x)) / (2 * M_PI);
	if (dot(cross(sph->dir, sph->look), vp) > 0)
		i->uv->x = 1.0 - i->uv->x;
	i->uv->y = i->uv->y / M_PI;
}

/*
** intersection with sphere
*/

void	intersec_sph(t_hit *hit, t_vec3 o, t_vec3 d, t_fig *sph)
{
	t_vec3	oc;
	t_vec3	k;
	float	discr;

	oc = minus_vec3(o, sph->pos);
	k.x = dot(d, d);
	k.y = dot(oc, d);
	k.z = dot(oc, oc) - sph->rad * sph->rad;
	discr = k.y * k.y - k.x * k.z;
	if (discr < 0)
		return ;
	discr = sqrt(discr);
	k.x = 1 / k.x;
	hit->isec1 = malloc(sizeof(t_isec));
	hit->isec1->t = (discr - k.y) * k.x;
	hit->count = 1;
	fill_isec_sphere(hit->isec1, o, d, sph);
	if (discr > 0)
	{
		hit->isec2 = malloc(sizeof(t_isec));
		hit->isec2->t = (-discr - k.y) * k.x;
		hit->count = 1;
		fill_isec_sphere(hit->isec2, o, d, sph);
	}
}



void	fill_isec_cyl(t_isec *i, t_vec3 o, t_vec3 d, t_fig *cyl)
{
	float	scale;
	t_vec3	vt;
	float	m;
	t_vec3	p;
	t_vec3	dir;

	scale = 0.1f;
	i->fig = cyl;
	dir = mult_vec3f(cyl->dir, -1);
	vt = mult_vec3f(dir, t->x);
	m = dot(d, vt) + dot(dir, minus_vec3(o, cyl->pos));
	p = plus_vec3(mult_vec3f(d, t->x), o);
	i->n = minus_vec3(minus_vec3(p, cyl->pos), mult_vec3f(dir, m));
	i->n = div_vec3f(i->n, len_vec3(i->n));
	i->uv->y = (acos(dot(cyl->look, i->n)) / (M_PI));
	m *= scale;
	m -= (int)m;
	i->uv->x = m;
	if (m < 0)
		i->uv->x = - m;
}

/*
** intersection with hyper_cylinder
*/

void	intersec_cyl(t_hit *hit, t_vec3 o, t_vec3 d, t_fig *cyl)
{
	t_vec3	oc;
	t_vec3	k;
	// t_vec3	v;
	float	discr;

	// v = div_vec3f(cyl->dir, len_vec3(cyl->dir));
	oc = minus_vec3(o, cyl->pos);
	k.x = dot(d, d) - pow(dot(d, cyl->dir), 2);
	k.y = 2 * (dot(oc, d) - dot(d, cyl->dir) * dot(oc, cyl->dir));
	k.z = dot(oc, oc) - pow(dot(oc, cyl->dir), 2) - cyl->rad * cyl->rad;
	discr = k.y * k.y - 4 * k.x * k.z;
	if (discr < 0)
		return ;
	discr = sqrt(discr);
	k.x *= 2;
	hit->isec1 = malloc(sizeof(t_isec));
	hit->isec1->t = (discr - k.y) / k.x;
	hit->count = 1;
	fill_isec_cyl(hit->isec1, o, d, cyl);
	if (discr > 0)
	{
		hit->isec1 = malloc(sizeof(t_isec));
		hit->isec1->t = (-discr - k.y) / k.x;
		hit->count = 1;
		fill_isec_cyl(hit->isec1, o, d, cyl);
	}
}

/*
** intersection with plane
*/

void	intersec_pln(t_hit *hit, t_vec3 o, t_vec3 d, t_fig *pln)
{
	t_vec3	oc;
	t_vec3	v;
	t_vec3	Vp;
	int		tmp;
	float	scale;

	scale = 0.10f;
	v = invert_vec3(div_vec3f(pln->dir, len_vec3(pln->dir)));
	if (dot(d, v) > 0)
	{
		hit->isec1 = malloc(sizeof(t_isec));
		hit->count = 1;
		hit->isec1->fig = pln;
		oc = invert_vec3(minus_vec3(o, pln->pos));
		hit->isec1->t = dot(oc, v) / dot(d, v);
		vp = minus_vec3(pln->pos, plus_vec3(o, mult_vec3f(d, t->x)));
		mult_m3(&vp, vp, pln->mat_z);
		mult_m3(&vp, vp, pln->mat_x);
		mult_m3(&vp, vp, pln->mat_y);
		hit->isec1.uv.y = vp.z * scale;
		hit->isec1.uv.x = vp.x * scale;
		tmp = hit->isec1.uv.y;
		hit->isec1.uv.y -= tmp;
		if (hit->isec1.uv.y < 0)
			hit->isec1.uv.y += 1.0;
		tmp = hit->isec1.uv.x;
		hit->isec1.uv.x -= tmp;
		if (hit->isec1.uv.x < 0)
			hit->isec1.uv.x += 1.0;
	}
}

/*
** intersection with hyper_conus
*/

void	intersec_con(t_hit *hit, t_vec3 o, t_vec3 d, t_fig *con)
{
	t_vec3	oc;
	t_vec3	k;
	t_vec3	v;
	float	discr;

	v = div_vec3f(con->dir, len_vec3(con->dir));
	t->x = INFINITY;
	t->y = INFINITY;
	t->z = INFINITY;
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

	if (arr_fig == 1)
	{
		if (t->y > t->x)
			t->x = t->y;
		else
			t->y = t->x;
	}

		// if (t->x > t->y)
		// t->x = t->y;
	
	float	scale = 0.1f;

	t_vec3 dir = mult_vec3f(con->dir, -1);
	t_vec3 vt = mult_vec3f(dir, t->y);
	t_vec3 c = set_vec3(con->pos);
	float m = dot(d, vt) + dot(dir, minus_vec3(o, c));
	if (m<0 || m>5.0)
	{
		t->y = t->x;
		t->x = INFINITY;
		t_vec3 vt = mult_vec3f(dir, t->y);
		t_vec3 c = set_vec3(con->pos);
		m = dot(d, vt) + dot(dir, minus_vec3(o, c));
		if (m<0 || m>5.0)
		{
			t->y = INFINITY;
			return ;
		}
	}
	t_vec3 p = plus_vec3(mult_vec3f(d, t->y), o);
	t_vec3 n = minus_vec3(minus_vec3(p, c), mult_vec3f(dir, m));
	n = div_vec3f(n, len_vec3(n));
	
	t->z = (acos(dot(con->look, n)) / (M_PI));
	m *= scale;
	m -= (int)m;
	t->w = m;
	if (m < 0)
		t->w = - m;
}
