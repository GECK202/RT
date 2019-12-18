/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtopencl.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/16 18:40:54 by vkaron            #+#    #+#             */
/*   Updated: 2019/12/16 22:48:05 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTOPENCL_H
# define RTOPENCL_H

# include <OpenCL/cl.h>
# include <stdio.h>
# include <stdlib.h>



// "//typedef struct s_fig t_fig;\n"
// "\n"
// "//struct s_fig\n"
// "//{\n"
// "//	float3	pos;\n"
// "//	float3	up;\n"
// "//	int		type;\n"
// "//};\n"

//"__kernel void mul(__global t_fig *figs, int num)\n"

// (figs->pos.x + figs->pos.y + figs->pos.z) * 


// "struct s_transf\n"
// "{\n"
// "	cl_float4	pos;\n"
// "	cl_float4	up;\n"
// "	cl_float4	look;\n"
// "	cl_float4	right;\n"
// "};\n"


// "\n"

const char *g_kernel_source =
"#define S_W (400)\n"
"#define S_H (400)\n"
"#define H_W (S_W / 2)\n"
"#define H_H (S_H / 2)\n"
"#define RATIO ((cl_float)S_W / (cl_float)S_H)\n"
"#define POT (16)\n"
""
"typedef struct s_mng		t_mng;\n"
"typedef struct s_transf	t_transf;\n"
"typedef struct s_mat		t_mat;\n"
"typedef struct s_obj		t_obj;\n"
"typedef enum e_type_obj	t_type_obj;\n"
""
"enum	e_type_obj"
"{"
"	sphere,"
"	plane,"
"	cylinder,"
"	conus,"
"	ambient,"
"	direction,"
"	point"
"};"
""
"struct s_mat"
"{"
"	int		color;"
"	int		specular;"
"	float	reflection;"
"};"
""
"struct s_transf"
"{"
"	float3	pos;"
"	float3	up;"
"	float3	look;"
"	float3	right;"
"};"
""
"struct s_obj"
"{"
"	t_transf		tr;"
"	float			radius;"
"	t_mat			mat;"
"	t_type_obj		type;"
"	struct s_obj	*next;"
"};"
""
"struct s_mng"
"{"
"	t_obj	cam;"
"	t_obj	*fig;"
"	int		figs;"
"	t_obj	*lght;"
"	int		lghts;"
"};"
""
"int	Trace(t_obj *obj, float3 O, float3 D, float tmin, float tmax);"
"float2	IntersectRaySphere(float3 O, float3 D, t_obj *obj);"
""
"float2	IntersectRaySphere(float3 O, float3 D, t_obj *obj)"
"{"
"	float2 t = {INFINITY, INFINITY};"
"	float3	C = obj->tr.pos;"
"	float	r = obj->radius;"
"	float3	oc = O - C;"
"	float k1 = dot(D,D);"
"	float k2 = 2*dot(oc, D);"
"	float k3 = dot(oc,oc) - r*r;"
"	float discr = k2*k2 - 4*k1*k3;"
"	if (discr < 0)"
"		return (t);"
"	t.x = (-k2 + sqrt(discr)) / (2*k1);"
"	t.y = (-k2 - sqrt(discr)) / (2*k1);"
"	return (t);"
"}"
""
"int	Trace(t_obj *obj, float3 O, float3 D, float tmin, float tmax){"
"	int BGC = 0xffffff;"
"	t_obj *closest_sphere = 0;"
"	float closest_t = INFINITY;"
"	float2 t;"
"	for (int i = 0; i < 4; i++)"
"	{"
"		t = IntersectRaySphere(O, D, &(obj[i]));"
"		if (t.x >= tmin && t.x <= tmax && t.x < closest_t)"
"			{closest_t = t.x;"
"			closest_sphere = &(obj[i]);}"
"		if (t.y >= tmin && t.y <= tmax && t.y < closest_t)"
"			{closest_t = t.y;"
"			closest_sphere = &(obj[i]);}"
"	}"
"	if (closest_sphere == 0)"
"	{"
"		return (BGC);"
"	}"
"	return (closest_sphere->mat.color);"
"}"
""
"__kernel void mul(__global float3 *input, __global int *output, int num)"
"{"
"	int i = get_global_id(0);"
"	int y = i / S_H;"
"	int x = (i - (y * S_H));"
"	y = H_H - y;"
"	x = x - H_W;"
"	output[i] = 0;"
"	float3	O = {0,0,0};"
"	float3	D = (float3)((float)x/S_W, (float)y/S_H, 1);"
"	t_obj	obj[4];"
"	obj[0].tr.pos = (float3)(0, 0, 0);"
"	obj[0].radius = 0;"
"	obj[0].mat.color = 0;"
"	obj[1].tr.pos = (float3)(0, -1, 3);"
"	obj[1].radius = 1;"
"	obj[1].mat.color = 0xff0000;"
"	obj[2].tr.pos = (float3)(2, 0, 4);"
"	obj[2].radius = 1;"
"	obj[2].mat.color = 0xff;"
"	obj[3].tr.pos = (float3)(-2, 0, 4);"
"	obj[3].radius = 1;"
"	obj[3].mat.color = 0xff00;"

"	int color = Trace(obj, O, D, 1, INFINITY);"
"	output[i] = color;"
"}";

// "	if (color == 0xffffff)"
// "	{"
// "		if (y > 0)"
// "			color = 0xff;"
// "		else"
// "			color = 0xff00;"
// "	}"


























#endif
