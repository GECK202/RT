/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_menu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/12 19:15:01 by vkaron            #+#    #+#             */
/*   Updated: 2020/02/12 20:32:10 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		print_ttf(SDL_Surface *sdest, char *msg, int size, SDL_Rect *dest)
{
	SDL_Color	col;
	TTF_Font	*fnt;
	SDL_Surface	*stext;
	const char	*f = "prgres/courier.ttf";

	col.r = 0;
	col.g = 0;
	col.b = 0;
	fnt = NULL;
	fnt = TTF_OpenFont(f, size);
	if (!fnt)
		printf("TTF_OpenFontRW: %s\n", TTF_GetError());
	stext = TTF_RenderText_Solid(fnt, msg, col);
	SDL_BlitSurface(stext, NULL, sdest, dest);
	SDL_FreeSurface(stext);
	TTF_CloseFont(fnt);
}

SDL_Rect	*set_rect(SDL_Rect *rect, int x, int y)
{
	rect->x = x;
	rect->y = y;
	return (rect);
}

void		draw_text_menu1(t_lst *l)
{
	SDL_Rect	d;

	SDL_FillRect(l->mimg, NULL, SDL_MapRGB(l->mimg->format,
		180, 180, 180));
	print_ttf(l->mimg, "Camera:", 14, set_rect(&d, l->mrect.x + 5,
		l->mrect.y + 5));
	print_ttf(l->mimg, "position:", 14, set_rect(&d, l->mrect.x + 10,
		l->mrect.x + 18));
}

void		draw_text_menu2(t_lst *l)
{
	SDL_Rect	d;

	SDL_FillRect(l->mimg, NULL, SDL_MapRGB(l->mimg->format,
		180, 180, 180));
	print_ttf(l->mimg, "Figure:", 14, set_rect(&d, l->mrect.x + 5,
		l->mrect.y + 5));
	print_ttf(l->mimg, "position:", 14, set_rect(&d, l->mrect.x + 10,
		l->mrect.x + 18));
}
