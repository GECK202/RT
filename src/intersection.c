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

/*
** intersection with sphere
*/

void	intersec_sph(t_hit *hit, t_vec3 o, t_vec3 d, t_fig *sph)
{
	t_vec3	oc;
	t_vec3	k;
	float	discr;


	hit->isec1 = NULL;
	hit->isec2 = NULL;
	hit->count = 0;

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
	hit->isec1->fig = sph;
	hit->isec1->t = (discr - k.y) * k.x;

	t_vec3 Vp;
	float x;

	Vp = minus_vec3(sph->pos, plus_vec3(o, mult_vec3f(d, hit->isec1->t)));
	Vp = div_vec3f(Vp, len_vec3(Vp));
	x = -dot(sph->dir, Vp);
	hit->isec1->uv.y = acos(x);
	x = sin(hit->isec1->uv.y);
	hit->isec1->uv.x = (acos(dot(Vp, sph->look)/x)) / (2 * M_PI);
	if (dot(cross(sph->dir, sph->look), Vp) > 0)
		hit->isec1->uv.x = 1.0 - hit->isec1->uv.x;
	hit->isec1->uv.y = hit->isec1->uv.y / M_PI;

	// trc.p = plus_vec3(mult_vec3f(trc.d, cisec->t), (trc.o));
	// n = minus_vec3(trc.p, fig.pos);
	hit->isec1->n = invert_vec3(Vp);

	hit->count = 1;
	if (discr == 0)
		return ;

	hit->isec2 = malloc(sizeof(t_isec));
	hit->isec2->fig = sph;
	hit->isec2->t = (-discr - k.y) * k.x;

	Vp = minus_vec3(sph->pos, plus_vec3(o, mult_vec3f(d, hit->isec2->t)));
	Vp = div_vec3f(Vp, len_vec3(Vp));
	x = -dot(sph->dir, Vp);
	hit->isec2->uv.y = acos(x);
	x = sin(hit->isec2->uv.y);
	hit->isec2->uv.x = (acos(dot(Vp, sph->look)/x)) / (2 * M_PI);
	if (dot(cross(sph->dir, sph->look), Vp) > 0)
		hit->isec2->uv.x = 1.0 - hit->isec2->uv.x;
	hit->isec2->uv.y = hit->isec2->uv.y / M_PI;

	hit->isec2->n = invert_vec3(Vp);
	hit->count = 2;
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
	float	t;

	// float	width = 10;
	// float	height = 20;

	scale = 0.10f;
	v = invert_vec3(div_vec3f(pln->dir, len_vec3(pln->dir)));

	hit->isec1 = NULL;
	hit->isec2 = NULL;
	hit->count = 0;

	if (dot(d, v) > 0)
	{
		oc = invert_vec3(minus_vec3(o, pln->pos));
		t = dot(oc, v) / dot(d, v);
		Vp = minus_vec3(pln->pos, plus_vec3(o, mult_vec3f(d, t)));


		
		mult_m3(&Vp, Vp, pln->mat_z);
		mult_m3(&Vp, Vp, pln->mat_x);
		mult_m3(&Vp, Vp, pln->mat_y);

		// Vz = cross(Vp, pln->dir);

		if ((Vp.x >= 0 && Vp.x <= pln->limit.x && Vp.z >=0 && Vp.z <= pln->limit.y)
			|| pln->limit.x == 0 || pln->limit.y == 0)
		{
			hit->isec1 = malloc(sizeof(t_isec));
			hit->isec1->fig = pln;
			hit->isec1->t = t;
			hit->isec1->n = set_vec3(pln->dir);
			hit->count = 1;
			
			hit->isec1->uv.y = Vp.z * scale;
			hit->isec1->uv.x = Vp.x * scale;

			int tmp = hit->isec1->uv.y;
			hit->isec1->uv.y -= tmp;
			if (hit->isec1->uv.y < 0)
				hit->isec1->uv.y += 1.0;
			
			tmp = hit->isec1->uv.x;
			hit->isec1->uv.x -= tmp;
			if (hit->isec1->uv.x < 0)
				hit->isec1->uv.x += 1.0;
		}
	}
}

/*
** intersection with hyper_cylinder
*/

void	intersec_cyl(t_hit *hit, t_vec3 o, t_vec3 d, t_fig *cyl)
{
	t_vec3	oc;
	t_vec3	k;
	t_vec3	v;
	float	discr;

	float	t;

	v = div_vec3f(cyl->dir, len_vec3(cyl->dir));

	hit->isec1 = NULL;
	hit->isec2 = NULL;
	hit->count = 0;

	oc = minus_vec3(o, cyl->pos);
	k.x = dot(d, d) - pow(dot(d, cyl->dir), 2);
	k.y = 2 * (dot(oc, d) - dot(d, cyl->dir) * dot(oc, cyl->dir));
	k.z = dot(oc, oc) - pow(dot(oc, cyl->dir), 2) - cyl->rad * cyl->rad;
	discr = k.y * k.y - 4 * k.x * k.z;
	if (discr < 0)
		return ;
	discr = sqrt(discr);
	k.x *= 2;

	t = (discr - k.y) / k.x;

	float	scale = 0.1f;
	t_vec3 dir;
	t_vec3 vt;
	t_vec3 c;
	float m;
	t_vec3 p;
	// t_vec3 n;

	dir = mult_vec3f(cyl->dir, -1);
	vt = mult_vec3f(dir, t);
	c = set_vec3(cyl->pos);
	m = dot(d, vt) + dot(dir, minus_vec3(o, c));

	// if (cyl->limit.x == 0 || (m >= 0 && m <= cyl->limit.x))
	// {

		hit->isec1 = malloc(sizeof(t_isec));
		hit->isec1->fig = cyl;
		hit->isec1->t = t;

		p = plus_vec3(mult_vec3f(d, hit->isec1->t), o);
		hit->isec1->n = minus_vec3(minus_vec3(p, c), mult_vec3f(dir, m));
		hit->isec1->n = div_vec3f(hit->isec1->n, len_vec3(hit->isec1->n));
	
		hit->isec1->uv.y = (acos(dot(cyl->look, hit->isec1->n)) / (M_PI));
		m *= scale;
		m -= (int)m;
		hit->isec1->uv.x = m;
		// if (m < 0)
		// 	hit->isec1->uv.x = - m;
		// if (dot(cross(cyl->dir, cyl->look), p) > 0)
		// 	hit->isec1->uv.y = 1.0 - hit->isec1->uv.y;
		hit->count = 1;
	
		if (discr == 0)
			return ;
	// }

	t = (-discr - k.y) / k.x;
	
	vt = mult_vec3f(dir, t);
	c = set_vec3(cyl->pos);
	m = dot(d, vt) + dot(dir, minus_vec3(o, c));


	hit->isec2 = malloc(sizeof(t_isec));
	hit->isec2->fig = cyl;
	hit->isec2->t = t;
	
	p = plus_vec3(mult_vec3f(d, hit->isec2->t), o);
	hit->isec2->n = minus_vec3(minus_vec3(p, c), mult_vec3f(dir, m));
	hit->isec2->n = div_vec3f(hit->isec2->n, len_vec3(hit->isec2->n));
	
	hit->isec2->uv.y = (acos(dot(cyl->look, hit->isec2->n)) / (M_PI));
	m *= scale;
	m -= (int)m;
	hit->isec2->uv.x = m;
	// if (m < 0)
	// 	hit->isec2->uv.x = - m;
	// if (dot(cross(cyl->dir, cyl->look), p) > 0)
	// 	hit->isec2->uv.y = 1.0 - hit->isec2->uv.y;
	hit->count = 2;



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


	hit->isec1 = NULL;
	hit->isec2 = NULL;
	hit->count = 0;

	oc = minus_vec3(o, con->pos);
	k.x = dot(d, d) - con->rad * pow(dot(d, con->dir), 2);
	k.y = 2 * (dot(oc, d) - con->rad * dot(d, con->dir) * dot(oc, con->dir));
	k.z = dot(oc, oc) - con->rad * pow(dot(oc, con->dir), 2);
	discr = k.y * k.y - 4 * k.x * k.z;
	if (discr < 0)
		return ;
	discr = sqrt(discr);
	k.x *= 2;
	///////////////////////////////////////////////////////////////
	hit->isec1 = malloc(sizeof(t_isec));
	hit->isec1->fig = con;
	hit->isec1->t = (discr - k.y) / k.x;

	

	// if (hit->isec1.t > hit->isec2.t)
	// 	hit->isec1.t = hit->isec2.t;

	float	scale = 0.1f;
	t_vec3 dir;
	t_vec3 vt;
	t_vec3 c;
	float m;
	t_vec3 p;
	// t_vec3 n;

	dir = mult_vec3f(con->dir, -1);
	vt = mult_vec3f(dir, hit->isec1->t);
	c = set_vec3(con->pos);
	m = dot(d, vt) + dot(dir, minus_vec3(o, c));
	p = plus_vec3(mult_vec3f(d, hit->isec1->t), o);
	hit->isec1->n = minus_vec3(minus_vec3(p, c), mult_vec3f(dir, m));
	hit->isec1->n = div_vec3f(hit->isec1->n, len_vec3(hit->isec1->n));
	
	hit->isec1->uv.y = (acos(dot(con->look, hit->isec1->n)) / (M_PI));
	m *= scale;
	m -= (int)m;
	hit->isec1->uv.x = m;
	if (m < 0)
		hit->isec1->uv.x = - m;
	hit->count = 1;
	if (discr == 0)
		return ;

	hit->isec2 = malloc(sizeof(t_isec));
	hit->isec2->fig = con;
	hit->isec2->t = (-discr - k.y) / k.x;
	
	vt = mult_vec3f(dir, hit->isec2->t);
	c = set_vec3(con->pos);
	m = dot(d, vt) + dot(dir, minus_vec3(o, c));
	p = plus_vec3(mult_vec3f(d, hit->isec2->t), o);
	hit->isec2->n = minus_vec3(minus_vec3(p, c), mult_vec3f(dir, m));
	hit->isec2->n = div_vec3f(hit->isec2->n, len_vec3(hit->isec2->n));
	
	// if (hit->isec1->t > hit->isec2->t)
	// 	hit->isec1->t = hit->isec2->t;

	hit->isec2->uv.y = (acos(dot(con->look, hit->isec2->n)) / (M_PI));
	m *= scale;
	m -= (int)m;
	hit->isec2->uv.x = m;
	if (m < 0)
		hit->isec2->uv.x = - m;
	hit->count = 2;
}












/////////Нормальная функция конуса
// void	intersec_con(t_vec3 *t, t_vec3 o, t_vec3 d, t_fig *con)
// {
// 	t_vec3	oc;
// 	t_vec3	k;
// 	t_vec3	v;
// 	float	discr;

// 	v = div_vec3f(con->dir, len_vec3(con->dir));
// 	t->x = INFINITY;
// 	t->y = INFINITY;
// 	t->z = INFINITY;
// 	oc = minus_vec3(o, con->pos);
// 	k.x = dot(d, d) - con->rad * pow(dot(d, con->dir), 2);
// 	k.y = 2 * (dot(oc, d) - con->rad * dot(d, con->dir) * dot(oc, con->dir));
// 	k.z = dot(oc, oc) - con->rad * pow(dot(oc, con->dir), 2);
// 	discr = k.y * k.y - 4 * k.x * k.z;
// 	if (discr < 0)
// 		return ;
// 	discr = sqrt(discr);
// 	k.x *= 2;
// 	t->x = (discr - k.y) / k.x;
// 	t->y = (-discr - k.y) / k.x;

// 		if (t->x > t->y)
// 		t->x = t->y;
	
// 	float	scale = 0.1f;

// 	t_vec3 dir = mult_vec3f(con->dir, -1);
// 	t_vec3 vt = mult_vec3f(dir, t->x);
// 	t_vec3 c = set_vec3(con->pos);
// 	float m = dot(d, vt) + dot(dir, minus_vec3(o, c));
// 	t_vec3 p = plus_vec3(mult_vec3f(d, t->x), o);
// 	t_vec3 n = minus_vec3(minus_vec3(p, c), mult_vec3f(dir, m));
// 	n = div_vec3f(n, len_vec3(n));
	
// 	t->z = (acos(dot(con->look, n)) / (M_PI));
// 	m *= scale;
// 	m -= (int)m;
// 	t->w = m;
// 	if (m < 0)
// 		t->w = - m;
// }