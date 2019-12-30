/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 14:24:06 by vabraham          #+#    #+#             */
/*   Updated: 2019/12/30 22:52:20 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include <stdio.h>
#include "rtopencl.h"

extern const char	*g_kernel_source;

void				init_opencl(t_opencl *ocl, cl_float3 *inp)
{
	cl_platform_id		platform;
	cl_device_id		device;
	char				device_name[1024];

	clGetPlatformIDs(1, &platform, NULL);
	clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(device_name),
		&device_name, NULL);
	printf("CL_DEVICE_NAME: %s\n", device_name);
	cl_context_properties properties[] = {CL_CONTEXT_PLATFORM,
		(cl_context_properties)platform, 0};
	ocl->context = clCreateContext(properties, 1, &device, NULL, NULL, NULL);
	ocl->queue[0] = clCreateCommandQueue(ocl->context, device, 0, NULL);
	ocl->queue[1] = clCreateCommandQueue(ocl->context, device, 0, NULL);
	ocl->in_buf = clCreateBuffer(ocl->context, CL_MEM_READ_ONLY, sizeof(cl_float3), NULL, NULL);
	ocl->out_buf = clCreateBuffer(ocl->context, CL_MEM_WRITE_ONLY, sizeof(cl_int) * SIZE, NULL, NULL);
	ocl->in_gbuf = clCreateBuffer(ocl->context, CL_MEM_READ_ONLY, sizeof(cl_float3), NULL, NULL);
	ocl->obj_buf = clCreateBuffer(ocl->context, CL_MEM_READ_WRITE, sizeof(struct s_obj) * 5, NULL, NULL);
	ocl->program = clCreateProgramWithSource(ocl->context, 1,
		&g_kernel_source, NULL, NULL);
	clBuildProgram(ocl->program, 0, NULL, NULL, NULL, NULL);
	ocl->work_group_size[0] = SIZE;
	cl_int err;
	
	ocl->inp = *inp;
	
	cl_kernel load_kernel = clCreateKernel(ocl->program, "loadd", &err);
	printf("err = %d\n", err);
	float c_fig = 5;
	clSetKernelArg(load_kernel, 0, sizeof(float), (void*)&(c_fig));
	clSetKernelArg(load_kernel, 1, sizeof(cl_mem), (void*)&(ocl->in_gbuf));
	clSetKernelArg(load_kernel, 2, sizeof(cl_mem), (void*)&(ocl->obj_buf));
	clEnqueueWriteBuffer(ocl->queue[0], ocl->in_gbuf, CL_FALSE, 0, sizeof(cl_float3), &(ocl->inp), 0, NULL, NULL);	
	clEnqueueNDRangeKernel(ocl->queue[0], load_kernel, 1, NULL, ocl->work_group_size, NULL, 0, NULL, NULL);
	// clEnqueueBarrier (ocl->queue[1]);
	

	
	ocl->mulkernel = clCreateKernel(ocl->program, "mul", &err);
	printf("err = %d\n", err);

	cl_int4				size;
	cl_int				depth;
	size.s[0] = S_W;
	size.s[1] = S_H;
	size.s[2] = H_W;
	size.s[3] = H_H;
	ocl->depth = RECURCE_DEPTH;
	clSetKernelArg(ocl->mulkernel, 2, sizeof(cl_int4), (void*)&size);
	clSetKernelArg(ocl->mulkernel, 3, sizeof(cl_int), (void*)&(ocl->depth));
	clSetKernelArg(ocl->mulkernel, 4, sizeof(cl_mem), (void*)&(ocl->in_gbuf));
	clSetKernelArg(ocl->mulkernel, 0, sizeof(cl_mem), (void*)&(ocl->in_buf));
	clSetKernelArg(ocl->mulkernel, 1, sizeof(cl_mem), (void*)&(ocl->out_buf));
	clSetKernelArg(ocl->mulkernel, 5, sizeof(cl_mem), (void*)&(ocl->obj_buf));
	
}

void					run_opencl(t_opencl *ocl, t_transf *tr, cl_int *new_array)
{
	clEnqueueWriteBuffer(ocl->queue[0], ocl->in_buf, CL_FALSE, 0, sizeof(cl_float3), &(tr->pos), 0, NULL, NULL);
	clEnqueueNDRangeKernel(ocl->queue[0], ocl->mulkernel, 1, NULL, ocl->work_group_size, NULL, 0, NULL, NULL);	
	clEnqueueReadBuffer(ocl->queue[0], ocl->out_buf, CL_TRUE, 0, SIZE * sizeof(cl_int), new_array, 0, NULL, NULL);
	printf("READ OK: \n");
}

void	clean_opencl(t_opencl *ocl)
{
	//cleanup
	
	clReleaseMemObject(ocl->out_buf);
	clReleaseMemObject(ocl->in_buf);
	clReleaseKernel(ocl->mulkernel);
	clReleaseProgram(ocl->program);
	clReleaseCommandQueue(ocl->queue[0]);
	// clReleaseCommandQueue(ocl->queue[1]);
	clReleaseContext(ocl->context);
}

void	mult(t_lst *lst, t_trc *trc, int x, int y)
{
	trc->d.x = (float)x * RATIO / H_W;
	trc->d.y = (float)y / H_H;
	trc->d.z = 2;
	mult_m3(&trc->d, trc->d, lst->camera_z);
	mult_m3(&trc->d, trc->d, lst->camera_x);
	mult_m3(&trc->d, trc->d, lst->camera_y);
	lst->data[(S_H - y - H_H - 1) * S_W + x + H_W] =
		trace(lst, *trc, RECURCE_DEPTH);
}

void	*pixel(void *l)
{
	t_lst	*lst;
	int		x;
	int		y;
	t_trc	trc;

	lst = (t_lst*)l;
	trc.min = 1.0f;
	trc.max = INFINITY;
	y = -S_H / 2 + lst->pot * S_H / POT - 1;
	trc.o = set_vec3(lst->scn->cam_pos0);
	mult_m3(&trc.o, trc.o, lst->camera_z);
	mult_m3(&trc.o, trc.o, lst->camera_x);
	mult_m3(&trc.o, trc.o, lst->camera_y);
	trc.o = plus_vec3(trc.o, lst->scn->cam_pos);
	while (++y < -S_H / 2 + (lst->pot + 1) * S_H / POT)
	{
		x = -S_W / 2 - 1;
		while (++x < S_W / 2)
			mult(lst, &trc, x, y);
	}
	return (0);
}

void rain(t_lst *lst)
{
	t_transf tr;

	tr.pos[0] = 150;
	tr.pos[1] = 150;
	tr.pos[2] = 250;
	
	run_opencl(&lst->ocl, &tr, lst->data);
	// for (int i=0; i< SIZE; i++)
	// {
	// 	printf("%d=%d\n",i, lst->data[i]);
	// }
}

void	rain2(t_lst *lst)
{
	t_lst			data[POT];
	pthread_t		threads[POT];
	pthread_attr_t	attr;
	int				rc;
	void			*status;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	lst->pot = -1;
	while (++(lst->pot) < POT)
	{
		ft_memcpy((void*)&data[lst->pot], (void *)lst, sizeof(t_lst));
		rc = pthread_create(&threads[lst->pot],
			NULL, pixel, (void *)(&data[lst->pot]));
	}
	pthread_attr_destroy(&attr);
	lst->pot = -1;
	while (++(lst->pot) < POT)
		rc = pthread_join(threads[lst->pot], &status);
}

int		main(int ac, char *av[])
{
	t_lst	*lst;

	if (ac == 2)
	{
		lst = (t_lst *)malloc(sizeof(t_lst));
		if (scene_init(lst, av[1]))
		{
			init_mlx(lst);
			lst->norm = 0;
			cl_float3	*inp;
			inp = malloc(sizeof(cl_float3));
			inp->s[0] = 255;
			inp->s[1] = 0;
			inp->s[2] = 150;
			init_opencl(&lst->ocl, inp);
			rain(lst);
			mlx_put_image_to_window(lst->mlx, lst->win, lst->img, 0, 0);
			mlx_loop(lst->mlx);
		}
		free_l(lst);
	}
	ft_putstr("Usage : ./RTv1 scene_file\n");
	return (0);
}
