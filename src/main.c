/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 14:24:06 by vabraham          #+#    #+#             */
/*   Updated: 2019/12/22 18:27:24 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include <stdio.h>
#include "rtopencl.h"

extern const char	*g_kernel_source;

void					run_opencl(t_fig *obj, cl_int *new_array)
{
	cl_platform_id		platform;
	cl_device_id		device;
	char				device_name[1024];
	cl_context			context;
	cl_command_queue	queue;
	cl_mem				in_buf;
	cl_mem				out_buf;
	cl_program			program;
	cl_kernel			mulkernel;
	cl_int				num;

	//get first available OpenCL platform
	clGetPlatformIDs(1, &platform, NULL);
	//get first available device
	clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	//get device name
	clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(device_name),
		&device_name, NULL);
	printf("CL_DEVICE_NAME: %s\n", device_name);
	cl_context_properties properties[] = {CL_CONTEXT_PLATFORM,
		(cl_context_properties)platform, 0};
	//create a context
	context = clCreateContext(properties, 1, &device, NULL, NULL, NULL);
	//create a command-queue on the device
	queue = clCreateCommandQueue(context, device, 0, NULL);
	//allocate device memory
	in_buf = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_float3), NULL, NULL);
	out_buf = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_int) * SIZE, NULL, NULL);
	//create OpenCL program from source code
	program = clCreateProgramWithSource(context, 1,
		&g_kernel_source, NULL, NULL);
	//build program
	clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	//get compiled OpenCL kernel
	mulkernel = clCreateKernel(program, "mul", NULL);
	//set up kernel arguments
	num = 1;
	clSetKernelArg(mulkernel, 0, sizeof(cl_mem), (void*)&(in_buf));
	clSetKernelArg(mulkernel, 1, sizeof(cl_mem), (void*)&out_buf);
	clSetKernelArg(mulkernel, 2, sizeof(num), (void*)&num);
	
	//launch the kernel on the device
	size_t work_group_size[1] = {SIZE};
	
	clEnqueueWriteBuffer(queue, in_buf, CL_FALSE, 0, sizeof(cl_float3), &(obj->pos), 0, NULL, NULL);	
	
	clEnqueueNDRangeKernel(queue, mulkernel, 1, NULL, work_group_size, NULL,
		0, NULL, NULL);
	//copy output from device to host memory
	
	clEnqueueReadBuffer(queue, out_buf, CL_TRUE, 0, SIZE * sizeof(cl_int),
		new_array, 0, NULL, NULL);
	//cleanup
	clReleaseMemObject(out_buf);
	clReleaseMemObject(in_buf);
	clReleaseKernel(mulkernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);
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
	t_fig fig;

	fig.pos.x = 10;
	fig.pos.x = 20;
	fig.pos.x = 30;
	run_opencl(&fig, lst->data);
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
			
			rain(lst);
			mlx_put_image_to_window(lst->mlx, lst->win, lst->img, 0, 0);
			mlx_loop(lst->mlx);
		}
		free_l(lst);
	}
	ft_putstr("Usage : ./RTv1 scene_file\n");
	return (0);
}
