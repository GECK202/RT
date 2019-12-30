/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtopencl.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/16 18:40:54 by vkaron            #+#    #+#             */
/*   Updated: 2019/12/30 22:53:43 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTOPENCL_H
# define RTOPENCL_H

# include <stdio.h>
# include <stdlib.h>

const char *g_kernel_source =
"#define RATIO ((cl_float)S_W / (cl_float)S_H)\n"
"#define POT (16)\n"
""
"typedef struct s_mng		t_mng;\n"
"typedef struct s_transf	t_transf;\n"
"typedef struct s_mat		t_mat;\n"
"typedef struct s_obj		t_obj;\n"
"typedef enum e_type_obj	t_type_obj;\n"
"typedef struct s_intersect	t_intersect;\n"
""
"struct	s_intersect"
"{"
"	float	t;"
"	t_obj	*obj;"
"};"
""
"enum	e_type_obj"
"{"
"	sphere,"
"	plane,"
"	cylinder,"
"	conus,"
"	ambient,"
"	directional,"
"	point"
"};"
""
"struct s_mat"
"{"
"	float4	color;"
"	int		specular;"
"	float	reflective;"
"	float	intensity;"
"};"
""
"struct s_transf"
"{"
"	float3	pos;"
"	float3	up;"
"	float3	look;"
"	float3	right;"
"	float3	direction;"
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
"float4			Trace(t_obj *obj, t_obj *light, float3 O, float3 D, float tmin, float tmax, int depth);"
"float2			IntersectRaySphere(float3 O, float3 D, t_obj *obj);"
"t_intersect	ClosestIntersection(t_obj *obj, float3 O, float3 D, float tmin, float tmax);"
"float			ComputeLighting(t_obj *obj, t_obj *light, float3 P, float3 N, float3 V, int s);"
"float3			ReflectRay(float3 R, float3 N);"
""
"float3			ReflectRay(float3 R, float3 N)"
"{"
"	return ((N * (2 * dot(N, R))) - R);"
"}"
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
"t_intersect	ClosestIntersection(t_obj *obj, float3 O, float3 D, float tmin, float tmax)"
"{"
"	float2 t;"
"	t_intersect	intersect;"
"	intersect.t = INFINITY;"
"	intersect.obj = 0;"
// "	++obj;"
// "	for (int i = 1; i < 5; i++)"
"	while (obj != 0)"
"	{"
"		t = IntersectRaySphere(O, D, obj);"
"		if (t.x >= tmin && t.x <= tmax && t.x < intersect.t)"
"			{intersect.t = t.x;"
"			intersect.obj = obj;}"
"		if (t.y >= tmin && t.y <= tmax && t.y < intersect.t)"
"			{intersect.t = t.y;"
"			intersect.obj = obj;}"
"		obj = obj->next;"
"	}"
"	return(intersect);"
"}"
""
"float	ComputeLighting(t_obj *obj, t_obj *light, float3 P, float3 N, float3 V, int s)"
"{"
"	float 	intens = 0.0;"
"	float3 	L;"
"	float	n_dot_l;"
"	float3	R;"
"	float	r_dot_v;"
"	t_intersect shadow;"
"	float	tmax;"
""
"	for (int i = 1; i < 4; i++)"
"	{"
"		if (light[i].type == ambient)"
"			intens += light[i].mat.intensity;"
"		else"
"		{"
"			if (light[i].type == point){"
"				L = light[i].tr.pos - P;"
"				tmax = 1.0f;"
"			} else {"
"				L = light[i].tr.direction;"
"				tmax = INFINITY;"
"			}"
""
"			/* shadow */"
""
"			shadow = ClosestIntersection(obj, P, L, 0.001, tmax);"
"			if (shadow.obj != 0)"
"				continue;"
""
"			/* diffuse */"
""
"			n_dot_l = dot(N, L);"
"			if (n_dot_l > 0)"
"				intens += (light[i].mat.intensity * n_dot_l / (length(N) * length(L)));"
""
"			/* specular */"
""
"			if (s != -1)"
"			{"
"				R = ReflectRay(L, N);"
"				r_dot_v = dot(R, V);"
"				if (r_dot_v > 0)"
"					intens += (light[i].mat.intensity * pown(r_dot_v / (length(R) * length(V)), s));"
"			}"
"		}"
"	}"
"	return (intens);"
"}"
""
"float4	Trace(t_obj *obj, t_obj *light, float3 O, float3 D, float tmin, float tmax, int depth){"
"	float4 BGC = (float4)(200,255,255,255);"
"	t_intersect	isec;"
"	float3	P;"
"	float3	N;"
"	float4	color;"
"	float4	r_color;"
"	float	rf;"
""
"	isec = ClosestIntersection(obj, O, D, tmin, tmax);"
"	if (isec.obj == 0)"
"	{"
"		return (BGC);"
"	}"
"	P = O + (isec.t * D);"
"	N = P - isec.obj->tr.pos;"
"	N = N / length(N);"
"	color = isec.obj->mat.color * ComputeLighting(obj, light, P, N, -D, isec.obj->mat.specular);"
"	color = clamp(color, 0.0f, 255.0f);"

"	rf = isec.obj->mat.reflective;"
"	if (depth <= 0 || rf <= 0)"
"		return (color);"
"	while (1)"
"	{"
"		O = P;"
"		D = ReflectRay(-D, N);"
"		depth -= 1;"
"		tmin = 0.05f;"
"		tmax = INFINITY;"
"		isec = ClosestIntersection(obj, O, D, tmin, tmax);"
"		if (isec.obj == 0)"
"		{"
"			r_color = BGC;"
"			color = color * (1 - rf) + r_color * rf;"
"			color = clamp(color, 0.0f, 255.0f);"
"			return(color);"
"		}"
"		"
"		P = O + (isec.t * D);"
"		N = P - isec.obj->tr.pos;"
"		N = N / length(N);"
"		r_color = isec.obj->mat.color * ComputeLighting(obj, light, P, N, -D, isec.obj->mat.specular);"
// "		r_color = clamp(r_color, 0.0f ,255.0f);"
"		color = color * (1 - rf) + r_color * rf;"
"		color = clamp(color, 0.0f, 255.0f);"
"		rf = isec.obj->mat.reflective;"
"		if (depth <= 0 || rf <= 0)"
"			return (color);"
"	}"
"	return (color);"
"}"
""

""
"__kernel void loadd(	int count_fig,"
"						__global float3 *inp,"
"						__global t_obj *obj)"
"{"
"	inp->x = 0;"

// "	obj[0].tr.pos = (float3)(0, 0, 0);"
// "	obj[0].radius = 0;"
// "	obj[0].mat.color = (float4)(0,0,0,0);"
// "	obj[0].mat.specular = -1;"
// "	obj[0].mat.reflective = 0;"
// "	obj[0].next = &(obj[1]);"

"	obj[1].tr.pos = (float3)(0, -1, 3);"
"	obj[1].radius = 1;"
"	obj[1].mat.color = (float4)(255, 0, 0, 255);"
"	obj[1].mat.specular = 500;"
"	obj[1].mat.reflective = 0;"

// "	obj[1].next = &(obj[2]);"
// ""
// "	obj[2].tr.pos = (float3)(2, 0, 4);"
// "	obj[2].radius = 1;"
// "	obj[2].mat.color = (float4)(0, 0, 255, 255);"
// "	obj[2].mat.specular = 1000;"
// "	obj[2].mat.reflective = 0.2;"
// "	obj[2].next = &(obj[3]);"
// ""
// "	obj[3].tr.pos = (float3)(-2, 0, 4);"
// "	obj[3].radius = 1;"
// "	obj[3].mat.color = (float4)(0, 255, 0, 255);"
// "	obj[3].mat.specular = 500;"
// "	obj[3].mat.reflective = 0.2;"
// // "	obj[3].next = 0;"
// "	obj[3].next = &(obj[4]);"
// ""
// "	obj[4].tr.pos = (float3)(0, -5001, 0);"
// "	obj[4].radius = 5000;"
// "	obj[4].mat.color = (float4)(255, 255, 0, 255);"
// "	obj[4].mat.specular = 1000;"
// "	obj[4].mat.reflective = 0.5;"
// "	obj[4].next = 0;"
"}"
""
"__kernel void mul(	__global float3 *input,"
"					__global int *output,"
"					int4 size,"
"					int depth,"
"					__global float3 *inp,"
"					__global t_obj *obj2)"
"{"
"	int i = get_global_id(0);"
"	int y = i / size.x;"
"	int x = (i - (y * size.y));"
"	y = size.w - y;"
"	x = x - size.z;"
"	output[i] = 0;"
"	float3	O = {0,0,0};"
"	float3	D = (float3)((float)x/size.x, (float)y/size.y, 1);"
"	t_obj	obj[5];"
// "	obj = obj2;"
"	"
"	obj[0].tr.pos = (float3)(0, 0, 0);"
"	obj[0].radius = 0;"
"	obj[0].mat.color = (float4)(0,0,0,0);"
"	obj[0].mat.specular = -1;"
"	obj[0].mat.reflective = 0;"
"	obj[0].next = &(obj[1]);"
""
// "	obj[1].tr.pos = (float3)(0, -1, 3);"
// "	obj[1].radius = 1;"
// "	obj[1].mat.color = (float4)(255, 0, 0, 255);"
// "	obj[1].mat.specular = 500;"
// "	obj[1].mat.reflective = 0;"
"	obj[1] = obj2[1];"


"	obj[1].next = &(obj[2]);"
""
"	obj[2].tr.pos = (float3)(2, 0, 4);"
"	obj[2].radius = 1;"
"	obj[2].mat.color = (float4)(0, 0, 255, 255);"
"	obj[2].mat.specular = 1000;"
"	obj[2].mat.reflective = 0.2;"
"	obj[2].next = &(obj[3]);"
""
"	obj[3].tr.pos = (float3)(-2, 0, 4);"
"	obj[3].radius = 1;"
"	obj[3].mat.color = (float4)(0, 255, 0, 255);"
"	obj[3].mat.specular = 500;"
"	obj[3].mat.reflective = 0.2;"
"	obj[3].next = &(obj[4]);"
""
"	obj[4].tr.pos = (float3)(0, -5001, 0);"
"	obj[4].radius = 5000;"
"	obj[4].mat.color = (float4)(255, 255, 0, 255);"
"	obj[4].mat.specular = 1000;"
"	obj[4].mat.reflective = 0.5;"
"	obj[4].next = 0;"
""
"	t_obj	light[4];"
"	light[0].type = ambient;"
"	light[0].mat.intensity = 0;"
"	light[1].type = ambient;"
"	light[1].mat.intensity = 0.2;"
"	light[2].type = point;"
"	light[2].mat.intensity = 0.6;"
"	light[2].tr.pos = (float3)(2, 1, 0);"
"	light[3].type = directional;"
"	light[3].mat.intensity = 0.2;"
"	light[3].tr.direction = (float3)(1, 4, 4);"


// "	float4 color = (float4)(inp->x, inp->y, inp->z, 255);"
"	float4 color = Trace(obj, light, O, D, 1, INFINITY, depth);"
// "	float4 color = (float4)(input->x, input->y, input->z, 255);"
"	output[i] = (((int)(color.x) << 16) & 0xff0000) | (((int)(color.y) << 8) & 0xff00) | ((int)(color.z) & 0xff);"
"}";


#endif
