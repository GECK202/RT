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

void	sel_fig_check(t_lst *lst, t_hit *hit, t_trc trc, t_fig *cur_fig)
{
	hit->isec1 = NULL;
	hit->isec2 = NULL;
	hit->count = 0;

	if (cur_fig->type == sphere)
		intersec_sph(lst, hit, trc, cur_fig);
	else if (cur_fig->type == cylinder)
		intersec_cyl(lst, hit, trc, cur_fig);
	else if (cur_fig->type == conus)
		intersec_con(lst, hit, trc, cur_fig);
	else if (cur_fig->type == plane)
		intersec_pln(lst, hit, trc, cur_fig);
}


t_isec *check_invert_fig(t_lst *lst, t_trc trc, float t)
{
	t_fig *cur_fig;

	cur_fig = lst->scn->figs;
	while(cur_fig)
	{
		// if (cur_fig->type == inv_sph)
		// {

		// }
		cur_fig = cur_fig->next;
	}
}

/*
** intersection with sphere
*/

void	intersec_sph(t_lst *lst, t_hit *hit, t_trc trc, t_fig *sph)
{
	t_vec3	oc;
	t_vec3	k;
	float	discr;


	

	oc = minus_vec3(trc.o, sph->pos);
	k.x = dot(trc.d, trc.d);
	k.y = dot(oc, trc.d);
	k.z = dot(oc, oc) - sph->rad * sph->rad;
	discr = k.y * k.y - k.x * k.z;
	if (discr < 0)
		return ;
	discr = sqrt(discr);
	k.x = 1 / k.x;

	float t = (discr - k.y) * k.x;



	hit->isec1 = malloc(sizeof(t_isec));
	hit->isec1->fig = sph;
	hit->isec1->t = t;

	t_vec3 Vp;
	float x;

	Vp = minus_vec3(sph->pos, plus_vec3(trc.o, mult_vec3f(trc.d, hit->isec1->t)));
	Vp = div_vec3f(Vp, len_vec3(Vp));
	x = -dot(sph->dir, Vp);
	hit->isec1->uv.y = acos(x);
	x = sin(hit->isec1->uv.y);
	hit->isec1->uv.x = (acos(dot(Vp, sph->look)/x)) / (2 * M_PI);
	// if (dot(cross(sph->dir, sph->look), Vp) > 0)
	if (dot(sph->right, Vp) > 0)
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

	Vp = minus_vec3(sph->pos, plus_vec3(trc.o, mult_vec3f(trc.d, hit->isec2->t)));
	Vp = div_vec3f(Vp, len_vec3(Vp));
	x = -dot(sph->dir, Vp);
	hit->isec2->uv.y = acos(x);
	x = sin(hit->isec2->uv.y);
	hit->isec2->uv.x = (acos(dot(Vp, sph->look)/x)) / (2 * M_PI);
	// if (dot(cross(sph->dir, sph->look), Vp) > 0)
	if (dot(sph->right, Vp) > 0)
		hit->isec2->uv.x = 1.0 - hit->isec2->uv.x;
	hit->isec2->uv.y = hit->isec2->uv.y / M_PI;

	hit->isec2->n = invert_vec3(Vp);
	hit->count = 2;
}

/*
** intersection with plane
*/

void	intersec_pln(t_lst *lst, t_hit *hit, t_trc trc, t_fig *pln)
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

	// hit->isec1 = NULL;
	// hit->isec2 = NULL;
	// hit->count = 0;

	t_vec3 n;
	n = set_vec3(pln->dir);
	if (dot(trc.d, v) < 0)
		n = invert_vec3(n);
	{
		oc = invert_vec3(minus_vec3(trc.o, pln->pos));
		t = dot(oc, v) / dot(trc.d, v);
		Vp = minus_vec3(pln->pos, plus_vec3(trc.o, mult_vec3f(trc.d, t)));


		// mult_m3(&Vp, Vp, pln->mat_z);
		// mult_m3(&Vp, Vp, pln->mat_x);
		// mult_m3(&Vp, Vp, pln->mat_y);
		// Vp = div_vec3f(Vp, len_vec3(Vp));
		t_vec3 uv;
		uv.y = (dot(pln->look, Vp)) * scale;
		uv.x = (dot(pln->right, Vp)) * scale;
		
		// printf("%f, %f\n", uv.x, uv.y);
		// Vz = cross(Vp, pln->dir);

		if ((uv.x >= 0 && uv.x <= pln->limit.x && uv.y >=0 && uv.y <= pln->limit.y)
			|| pln->limit.x == 0 || pln->limit.y == 0)
		{
			hit->isec1 = malloc(sizeof(t_isec));
			hit->isec1->fig = pln;
			hit->isec1->t = t;
			hit->isec1->n = set_vec3(n);
			hit->count = 1;
			
			
			int tmp;
			tmp = (int)uv.x;
			uv.x -= tmp;
			tmp = (int)uv.y;
			uv.y -= (int)uv.y;//tmp;

			if (uv.x < 0)
				uv.x = -uv.x;

			if (uv.y < 0)
				uv.y = -uv.y;

			if (dot(Vp, pln->right)>0)
				uv.x = 1.0 - uv.x;

			if (dot(Vp, pln->look)<0)
				uv.y = 1.0 - uv.y;

			hit->isec1->uv.y = uv.y;
			hit->isec1->uv.x = uv.x;

		}
	}
}

t_isec	*get_isec_cyl(float t, t_vec3 o, t_vec3 d, t_fig *cyl)
{
	t_isec *isec;

	float	scale = 0.1f;
	t_vec3 dir;
	t_vec3 vt;
	t_vec3 c;
	float m;
	t_vec3 p;

	dir = mult_vec3f(cyl->dir, -1);
	vt = mult_vec3f(dir, t);
	c = set_vec3(cyl->pos);
	m = dot(d, vt) + dot(dir, minus_vec3(o, c));

	if (cyl->limit.x == 0 || (m >= 0 && m <= cyl->limit.x))
	{
		isec = malloc(sizeof(t_isec));
		isec->fig = cyl;
		isec->t = t;

		p = plus_vec3(mult_vec3f(d, isec->t), o);
		isec->n = minus_vec3(minus_vec3(p, c), mult_vec3f(dir, m));
		isec->n = div_vec3f(isec->n, len_vec3(isec->n));
	
		isec->uv.x = (acos(dot(cyl->look, isec->n)) / (2*M_PI));
		m *= scale;
		m -= (int)m;
		if (m<0.0)
			m = -m;
		isec->uv.y = m;

		if (dot(cyl->right, isec->n) > 0)
			isec->uv.x = 1.0 - isec->uv.x;


		return (isec);
	}
	return (NULL);
}

/*
** intersection with hyper_cylinder
*/

void	intersec_cyl(t_lst *lst, t_hit *hit, t_trc trc, t_fig *cyl)
{
	t_vec3	oc;
	t_vec3	k;
	t_vec3	v;
	float	discr;

	float	t;

	v = div_vec3f(cyl->dir, len_vec3(cyl->dir));

	// hit->isec1 = NULL;
	// hit->isec2 = NULL;
	// hit->count = 0;

	oc = minus_vec3(trc.o, cyl->pos);
	k.x = dot(trc.d, trc.d) - pow(dot(trc.d, cyl->dir), 2);
	k.y = 2 * (dot(oc, trc.d) - dot(trc.d, cyl->dir) * dot(oc, cyl->dir));
	k.z = dot(oc, oc) - pow(dot(oc, cyl->dir), 2) - cyl->rad * cyl->rad;
	discr = k.y * k.y - 4 * k.x * k.z;
	if (discr < 0)
		return ;
	discr = sqrt(discr);
	k.x *= 2;

	t = (discr - k.y) / k.x;

	hit->isec1 = get_isec_cyl(t, trc.o, trc.d, cyl);
	if (hit->isec1)
		hit->count = 1;

	if (discr == 0)
		return ;

	t = (-discr - k.y) / k.x;
	
	if (hit->isec1)
	{
		hit->isec2 = get_isec_cyl(t, trc.o, trc.d, cyl);
		if (hit->isec2)
			hit->count = 2;
	}
	else
	{
		hit->isec1 = get_isec_cyl(t, trc.o, trc.d, cyl);
		if (hit->isec1)
			hit->count = 1;
	}
}

/*
** intersection with hyper_conus
*/

void	intersec_con(t_lst *lst, t_hit *hit, t_trc trc, t_fig *con)
{
	t_vec3	oc;
	t_vec3	k;
	t_vec3	v;
	float	discr;
	float t;

	v = div_vec3f(con->dir, len_vec3(con->dir));


	// hit->isec1 = NULL;
	// hit->isec2 = NULL;
	// hit->count = 0;

	oc = minus_vec3(trc.o, con->pos);
	k.x = dot(trc.d, trc.d) - con->rad * pow(dot(trc.d, con->dir), 2);
	k.y = 2 * (dot(oc, trc.d) - con->rad * dot(trc.d, con->dir) * dot(oc, con->dir));
	k.z = dot(oc, oc) - con->rad * pow(dot(oc, con->dir), 2);
	discr = k.y * k.y - 4 * k.x * k.z;
	if (discr < 0)
		return ;
	discr = sqrt(discr);
	k.x *= 2;


	t = (discr - k.y) / k.x;

	hit->isec1 = get_isec_cyl(t, trc.o, trc.d, con);
	if (hit->isec1)
		hit->count = 1;

	if (discr == 0)
		return ;

	t = (-discr - k.y) / k.x;
	
	if (hit->isec1)
	{
		hit->isec2 = get_isec_cyl(t, trc.o, trc.d, con);
		if (hit->isec2)
			hit->count = 2;
	}
	else
	{
		hit->isec1 = get_isec_cyl(t, trc.o, trc.d, con);
		if (hit->isec1)
			hit->count = 1;
	}

}
