/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   act_sdl.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/08 23:44:09 by vabraham          #+#    #+#             */
/*   Updated: 2020/02/12 20:27:08 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include <stdio.h>

void	close_sdl(t_lst *lst)
{
	SDL_FreeSurface(lst->img);
	lst->img = 0;
	SDL_DestroyWindow(lst->win);
	lst->win = 0;
	TTF_Quit();
	SDL_Quit();
}

void	sld_events0(t_lst *lst, SDL_Event e, int *repaint)
{
	if (e.type == SDL_MOUSEMOTION)
	{
		mouse_move(&(e.motion), lst);
		if (lst->change)
			*repaint = 1;
	}
	else if (e.type == SDL_MOUSEWHEEL)
	{
		mouse_weel(e.wheel.y, lst);
		*repaint = 1;
	}
}

void	sld_events(t_lst *lst, SDL_Event e, int *quit, int *repaint)
{
	if (e.type == SDL_QUIT)
		*quit = 1;
	else if (e.type == SDL_KEYDOWN)
	{
		if (e.key.keysym.sym == SDLK_ESCAPE)
			*quit = 1;
		else
		{
			if (key_press(e.key.keysym.sym, lst))
				*repaint = 1;
			else
			{
				post_effects(lst);
				SDL_UpdateWindowSurface(lst->win);
			}
		}
	}
	else if (e.type == SDL_MOUSEBUTTONDOWN)
	{
		mouse_press(&(e.button), lst);
		*repaint = 1;
	}
}

void	show_menu(t_lst *lst)
{
	SDL_Rect r;

	r.x = 0;
	r.y = 0;
	r.w = 200;
	r.h = 285;
	lst->mrect.y = 10;
	draw_text_menu1(lst);
	SDL_BlitSurface(lst->mimg, &r, lst->img, &lst->mrect);
	draw_text_menu2(lst);
	lst->mrect.y = 305;
	SDL_BlitSurface(lst->mimg, &r, lst->img, &lst->mrect);
}

void	sdl_cycle(t_lst *lst)
{
	int			quit;
	SDL_Event	e;
	int			repaint;
	int			first;

	quit = 0;
	first = 1;
	while (!quit)
	{
		repaint = 0;
		if (SDL_PollEvent(&e) != 0 || repaint)
		{
			sld_events(lst, e, &quit, &repaint);
			sld_events0(lst, e, &repaint);
			if (repaint || first)
			{
				rain(lst);
				post_effects(lst);
				if (lst->show_menu)
					show_menu(lst);
				SDL_UpdateWindowSurface(lst->win);
				first = 0;
			}
		}
	}
}
