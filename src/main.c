/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 14:24:06 by vabraham          #+#    #+#             */
/*   Updated: 2020/01/14 16:24:50 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	mult(t_lst *lst, t_trc *trc, int x, int y)
{
	SDL_Color col;

	trc->d.x = (float)x * RATIO / H_W;
	trc->d.y = (float)y / H_H;
	trc->d.z = lst->scn->cam_focus_dist;
	mult_m3(&trc->d, trc->d, lst->camera_z);
	mult_m3(&trc->d, trc->d, lst->camera_x);
	mult_m3(&trc->d, trc->d, lst->camera_y);
	col = trace(lst, *trc, RECURCE_DEPTH);
	lst->data[(S_H - y - H_H - 1) * S_W + x + H_W] = (col.r << 16) + (col.g << 8) + col.b;
		// trace(lst, *trc, RECURCE_DEPTH);
		
	lst->data_dop[(S_H - y - H_H - 1) * S_W + x + H_W] = lst->data[(S_H - y - H_H - 1) * S_W + x + H_W];
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
		// pixel((void *)(&data[lst->pot]));
		rc = pthread_create(&threads[lst->pot],
			NULL, pixel, (void *)(&data[lst->pot]));
	}
	pthread_attr_destroy(&attr);
	lst->pot = -1;
	while (++(lst->pot) < POT)
		rc = pthread_join(threads[lst->pot], &status);
}

void close_sdl(t_lst *lst)
{
	SDL_FreeSurface(lst->img);
	lst->img = 0;
	SDL_DestroyWindow(lst->win);
	lst->win = 0;
	SDL_Quit();
}

int		main(int ac, char *av[])
{
	t_lst		*lst;
	int			quit;
	SDL_Event	e;
	int			repaint;
	int			first;
	
	if (ac == 2)
	{
		lst = (t_lst *)malloc(sizeof(t_lst));
		lst->postEffects = 0; //закинуть в init
		lst->data_dop = malloc(sizeof(int) * (S_H * S_W)); //закинуть в init
		lst->num_file_for_screen = 0; //закинуть в init
		if (scene_init(lst, av[1]) && init_sdl(lst))
		{
			///////////////
			lst->t = 0;
			t_fig *f;
			f = lst->scn->figs;
			while (f)
			{
				lst->t++;
				f = f->next;
			}
			///////////////
			lst->norm = 0;
			//load_tex_sdl(lst);
			quit = 0;
			first = 1;
			while (!quit)
			{
				repaint = 0;
				if( SDL_PollEvent( &e ) != 0 )
				{
					if(e.type == SDL_QUIT)
						quit = 1;
					else if (e.type == SDL_KEYDOWN)
					{
						if (e.key.keysym.sym == SDLK_ESCAPE)
							quit = 1;
						else
						{
							if (key_press(e.key.keysym.sym, lst))
								repaint = 1;
							else {
								postEffects(lst);
								SDL_UpdateWindowSurface(lst->win);
							}
						}
					}
					else if (e.type == SDL_MOUSEBUTTONDOWN)
					{
						mouse_press(&(e.button), lst);
						repaint = 1;
					}
					else if (e.type == SDL_MOUSEMOTION)
					{
						mouse_move(&(e.motion), lst);
						if (lst->change)
							repaint = 1;
					}
					else if (e.type == SDL_MOUSEWHEEL)
					{
						mouse_weel(e.wheel.y, lst);
						repaint = 1;
					}
					if (repaint || first)
					{
						rain(lst);
						postEffects(lst);
						SDL_UpdateWindowSurface(lst->win);
						first = 0;
						printf("%f %f %f\n", lst->scn->cur_fig->alpha.x, lst->scn->cur_fig->alpha.y, lst->scn->cur_fig->alpha.z);
					}
				}
			}
			// rain(lst);//хз, зачем
		}
		else
			write(1, "Usage:./RT filename\n", 20);
		close_sdl(lst);
		free_l(lst);
	}
	return (0);
}
