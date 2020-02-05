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


t_isec	*check_inv_sph(t_lst *lst, t_trc trc, float t, t_fig *isph)
{
	t_vec3	oc;
	t_vec3	k;
	float	discr;
	float	p;
	t_isec	*isec;

	if (len_vec3(minus_vec3(isph->pos, trc.p)) > isph->rad)
		return (NULL);
	oc = minus_vec3(trc.o, isph->pos);
	k.x = dot(trc.d, trc.d);
	k.y = dot(oc, trc.d);
	k.z = dot(oc, oc) - isph->rad * isph->rad;
	discr = k.y * k.y - k.x * k.z;
	if (discr < 0)
		return (NULL);
	discr = sqrt(discr);
	k.x = 1 / k.x;
	p = (-discr - k.y) * k.x;
	if (p > t)
		return (NULL);
	isec = malloc(sizeof(t_isec));
	isec->t = p;
		
	float x;

	trc.p = minus_vec3(isph->pos, plus_vec3(trc.o, mult_vec3f(trc.d, isec->t)));
	trc.p = div_vec3f(trc.p, len_vec3(trc.p));
	x = -dot(isph->dir, trc.p);
	isec->uv.y = acos(x);
	x = sin(isec->uv.y);
	isec->uv.x = (acos(dot(trc.p, isph->look)/x)) / (2 * M_PI);

	if (dot(isph->right, trc.p) > 0)
		isec->uv.x = 1.0 - isec->uv.x;
	isec->uv.y = isec->uv.y / M_PI;

	if (dot(trc.d, trc.p) < 0)
		isec->n = set_vec3(trc.p);
	else
		isec->n = invert_vec3(trc.p);
	return (isec);
}

t_isec	*check_inv_figs(t_lst *lst, t_trc trc, float t)
{
	t_fig	*cur_fig;
	t_isec	*isec;

	isec = NULL;
	cur_fig = lst->scn->figs;
	while (cur_fig)
	{
		if (cur_fig->type == inv_sph)
		{
			if ((isec = check_inv_sph(lst, trc, t, cur_fig)))
				return (isec);
		}
		cur_fig = cur_fig->next;
	}
	return (NULL);
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

	hit->isec1 = malloc(sizeof(t_isec));
	hit->isec1->fig = sph;
	hit->isec1->t = (discr - k.y) * k.x;

	// t_vec3 Vp;
	float x;

	trc.p = minus_vec3(sph->pos, plus_vec3(trc.o, mult_vec3f(trc.d, hit->isec1->t)));
	trc.p = div_vec3f(trc.p, len_vec3(trc.p));
	x = -dot(sph->dir, trc.p);
	hit->isec1->uv.y = acos(x);
	x = sin(hit->isec1->uv.y);
	hit->isec1->uv.x = (acos(dot(trc.p, sph->look)/x)) / (2 * M_PI);

	if (dot(sph->right, trc.p) > 0)
		hit->isec1->uv.x = 1.0 - hit->isec1->uv.x;
	hit->isec1->uv.y = hit->isec1->uv.y / M_PI;

	if (dot(trc.d, trc.p) < 0)
		hit->isec1->n = set_vec3(trc.p);
	else
		hit->isec1->n = invert_vec3(trc.p);


	hit->count = 1;
	if (discr == 0)
		return ;

	hit->isec2 = malloc(sizeof(t_isec));
	hit->isec2->fig = sph;
	hit->isec2->t = (-discr - k.y) * k.x;

	trc.p = minus_vec3(sph->pos, plus_vec3(trc.o, mult_vec3f(trc.d, hit->isec2->t)));
	trc.p = div_vec3f(trc.p, len_vec3(trc.p));
	x = -dot(sph->dir, trc.p);
	hit->isec2->uv.y = acos(x);
	x = sin(hit->isec2->uv.y);
	hit->isec2->uv.x = (acos(dot(trc.p, sph->look)/x)) / (2 * M_PI);
	// if (dot(cross(sph->dir, sph->look), Vp) > 0)
	if (dot(sph->right, trc.p) > 0)
		hit->isec2->uv.x = 1.0 - hit->isec2->uv.x;
	hit->isec2->uv.y = hit->isec2->uv.y / M_PI;

	if (dot(trc.d, trc.p) < 0)
		hit->isec2->n = set_vec3(trc.p);
	else
		hit->isec2->n = invert_vec3(trc.p);
	hit->count = 2;
}

/*
** intersection with plane
*/

void	intersec_pln(t_lst *lst, t_hit *hit, t_trc trc, t_fig *pln)
{
	t_vec3	oc;
	// t_vec3	Vp;
	int		tmp;
	float	scale;
	float	t;
	t_vec3	n;
	t_vec3	uv;

	scale = 0.10f;

	n = set_vec3(pln->dir);
	if (dot(trc.d, n) > 0)
		n = invert_vec3(n);
	{
		oc = invert_vec3(minus_vec3(trc.o, pln->pos));
		t = dot(oc, n) / dot(trc.d, n);
		trc.p = minus_vec3(pln->pos, plus_vec3(trc.o, mult_vec3f(trc.d, t)));

		uv.y = (dot(pln->look, trc.p)) * scale;
		uv.x = (dot(pln->right, trc.p)) * scale;

		if ((uv.x >= 0 && uv.x <= pln->limit.x && uv.y >=0 && uv.y <= pln->limit.y)
			|| pln->limit.x == 0 || pln->limit.y == 0)
		{
			if ((hit->isec1 = check_inv_figs(lst, trc, t)))
			{
				//printf("ok");
				// hit->isec1->fig = pln;
				// hit->count = 1;
				free(hit->isec1);
				hit->isec1 = NULL;
				return ;
			}
			hit->isec1 = malloc(sizeof(t_isec));
			hit->isec1->fig = pln;
			hit->isec1->t = t;
			hit->isec1->n = set_vec3(n);
			hit->count = 1;
			uv.x -= (int)uv.x;
			uv.y -= (int)uv.y;
			if (uv.x < 0)
				uv.x = -uv.x;
			if (uv.y < 0)
				uv.y = -uv.y;
			if (dot(trc.p, pln->right)>0)
				uv.x = 1.0 - uv.x;
			if (dot(trc.p, pln->look)<0)
				uv.y = 1.0 - uv.y;
			hit->isec1->uv.y = uv.y;
			hit->isec1->uv.x = uv.x;
		}
	}
}




t_isec	*get_isec_cyl(float t, t_trc trc, t_fig *cyl)
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
	m = dot(trc.d, vt) + dot(dir, minus_vec3(trc.o, c));

	if (cyl->limit.x == 0 || (m >= 0 && m <= cyl->limit.x))
	{
		isec = malloc(sizeof(t_isec));
		isec->fig = cyl;
		isec->t = t;

		p = plus_vec3(mult_vec3f(trc.d, isec->t), trc.o);
		isec->n = minus_vec3(minus_vec3(p, c), mult_vec3f(dir, m));
		if (dot(trc.d, isec->n)>0)
			isec->n = invert_vec3(isec->n);
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

	hit->isec1 = get_isec_cyl(t, trc, cyl);
	if (hit->isec1)
		hit->count = 1;

	if (discr == 0)
		return ;

	t = (-discr - k.y) / k.x;
	
	if (hit->isec1)
	{
		hit->isec2 = get_isec_cyl(t, trc, cyl);
		if (hit->isec2)
			hit->count = 2;
	}
	else
	{
		hit->isec1 = get_isec_cyl(t, trc, cyl);
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

	hit->isec1 = get_isec_cyl(t, trc, con);
	if (hit->isec1)
		hit->count = 1;

	if (discr == 0)
		return ;

	t = (-discr - k.y) / k.x;
	
	if (hit->isec1)
	{
		hit->isec2 = get_isec_cyl(t, trc, con);
		if (hit->isec2)
			hit->count = 2;
	}
	else
	{
		hit->isec1 = get_isec_cyl(t, trc, con);
		if (hit->isec1)
			hit->count = 1;
	}
}
