/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 13:39:18 by vkaron            #+#    #+#             */
/*   Updated: 2019/11/23 18:19:52 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

float	dot(t_vec3 v1, t_vec3 v2)
{
	float d;

	d = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	return (d);
}

float	len_vec3(t_vec3 v)
{
	float len;

	len = sqrt(dot(v, v));
	return (len);
}

t_vec3	set_vec3(t_vec3 src)
{
	t_vec3 dst;

	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
	return (dst);
}

t_col	set_col(int r, int g, int b)
{
	t_col c;

	c.r = r;
	c.g = g;
	c.b = b;
	return (c);
}
