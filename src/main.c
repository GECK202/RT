/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabraham <vabraham@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 14:24:06 by vabraham          #+#    #+#             */
/*   Updated: 2020/02/10 18:55:59 by vabraham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	mult(t_lst *lst, t_trc *trc, int x, int y)
{
	SDL_Color	col;
	t_isec		*cisec;

	trc->d.x = (float)x * RATIO / H_W;
	trc->d.y = (float)y / H_H;
	trc->d.z = lst->scn->cam_focus_dist;
	mult_m3(&trc->d, trc->d, lst->camera_z);
	mult_m3(&trc->d, trc->d, lst->camera_x);
	mult_m3(&trc->d, trc->d, lst->camera_y);
	col = trace(lst, *trc, RECURCE_DEPTH, cisec);
	lst->data[(S_H - y - H_H - 1) * S_W + x + H_W] =
		(COLR << 16) + (col.g << 8) + COLB;
	lst->data_dop[(S_H - y - H_H - 1) * S_W + x + H_W] =
		lst->data[(S_H - y - H_H - 1) * S_W + x + H_W];
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

void	rain(t_lst *lst)
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
	t_lst		*lst;

	if (ac == 2)
	{
		lst = (t_lst *)malloc(sizeof(t_lst));
		if (scene_init(lst, av[1]) && init_sdl(lst))
			sdl_cycle(lst);
		else
			write(1, "Usage:./RT filename\n", 20);
		close_sdl(lst);
		free_l(lst);
	}
	return (0);
}
