/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_menu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/12 19:15:01 by vkaron            #+#    #+#             */
/*   Updated: 2020/02/13 23:03:06 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		print_ttf(SDL_Surface *sdest, char *msg, int size, SDL_Rect *dest)
{
	SDL_Color	col;
	TTF_Font	*fnt;
	SDL_Surface	*stext;
	const char	*f = "prgres/courier.ttf";

	col = (SDL_Color){0, 0, 0, 255};
	fnt = NULL;
	fnt = TTF_OpenFont(f, size);
	if (!fnt)
		printf("TTF_OpenFontRW: %s\n", TTF_GetError());
	stext = TTF_RenderText_Blended(fnt, msg, col);
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

void		draw_shader(t_lst *l, int fsize, int x, int y)
{
	SDL_Rect d;

	d.x = x;
	d.y = y;
	if (!l->shd)
		print_ttf(l->mimg, "SIMPLE", fsize, &d);
	else if (l->shd == TEXTURE)
		print_ttf(l->mimg, "TEXTURE", fsize, &d);
	else if (l->shd == TRASPARENT)
		print_ttf(l->mimg, "TRASPARE", fsize, &d);
	else if (l->shd == REFRLECT)
		print_ttf(l->mimg, "REFRLECT", fsize, &d);
	else if (l->shd == REFRACT)
		print_ttf(l->mimg, "REFRACT", fsize, &d);
	d.x += 70;
	if (l->scn->shadow)
		print_ttf(l->mimg, "SHW ON", fsize, &d);
	else
		print_ttf(l->mimg, "SHW OFF", fsize, &d);
}

char		*c_lim(char *a)
{
	int i;

	i = 0;
	while(a[i])
	{
		if (a[i] == '.')
		{
			a[i + 2] = 0;
			break ;
		}
		++i;
	}
	return (a);
}

void		draw_cam_pos(t_lst *l, int fsize, int x, int y)
{
	SDL_Rect	d;
	char		*a;

	d.x = x;
	d.y = y;
	a = c_lim(get_fnbr_to_string(l->scn->cam_pos0.x + l->scn->cam_pos.x));
	print_ttf(l->mimg, a, fsize, &d);
	free(a);
	d.x += 35;
	a = c_lim(get_fnbr_to_string(l->scn->cam_pos0.y + l->scn->cam_pos.y));
	print_ttf(l->mimg, a, fsize, &d);
	free(a);
	d.x += 35;
	a = c_lim(get_fnbr_to_string(l->scn->cam_pos0.z + l->scn->cam_pos.z));
	print_ttf(l->mimg, a, fsize, &d);
	free(a);
}

void		draw_vec(t_lst *l, int fsize, SDL_Rect *d, t_vec3 *v)
{
	char		*a;

	a = c_lim(get_fnbr_to_string(v->x));
	print_ttf(l->mimg, a, fsize, d);
	free(a);
	d->x += 50;
	a = c_lim(get_fnbr_to_string(v->y));
	print_ttf(l->mimg, a, fsize, d);
	free(a);
	d->x += 50;
	a = c_lim(get_fnbr_to_string(v->z));
	print_ttf(l->mimg, a, fsize, d);
	free(a);
}

void		draw_col(t_lst *l, int fsize, SDL_Rect *d, SDL_Color *c)
{
	char		*a;

	a = get_thre_int(c->r, c->g, c->b);
	print_ttf(l->mimg, a, fsize, d);
	free(a);
}

void		draw_type_fig(t_lst *l, int fsize, int x, int y)
{
	SDL_Rect d;

	d.x = x;
	d.y = y;
	if (l->scn->cur_fig->type == sphere)
		print_ttf(l->mimg, "SPHERE", fsize, &d);
	else if (l->scn->cur_fig->type == plane)
		print_ttf(l->mimg, "PLANE", fsize, &d);
	else if (l->scn->cur_fig->type == conus)
		print_ttf(l->mimg, "CONUS", fsize, &d);
	else if (l->scn->cur_fig->type == cylinder)
		print_ttf(l->mimg, "CYLINDER", fsize, &d);
	else if (l->scn->cur_fig->type == inv_sph)
		print_ttf(l->mimg, "INVERT", fsize, &d);
}

/*
** scene panel
*/

void		draw_text_menu1(t_lst *l)
{
	SDL_Rect	d;
	int			*data;
	int			h;

	data = (int *)l->mimg->pixels;
	h = -1;
	while (++h < S_W * S_H)
		data[h] = 0x88ffffff;
	print_ttf(l->mimg, "Shader:", 14, set_rect(&d, l->mrect.x - 5,
		l->mrect.y));
	draw_shader(l, 14, l->mrect.x + 55, l->mrect.y);
	print_ttf(l->mimg, "Camera:", 14, set_rect(&d, l->mrect.x + 35,
		l->mrect.y + 15));
	print_ttf(l->mimg, "position:", 14, set_rect(&d, l->mrect.x - 5,
		l->mrect.y + 30));
	draw_cam_pos(l, 14, l->mrect.x + 75, l->mrect.y + 30);
}

void	draw_cre_button(t_lst *l, int fig, SDL_Rect	*d)
{
	int			*data;
	int			x;
	int			y;

	data = (int *)l->mimg->pixels;
	y = d->y - 5;
	while (y < d->y + 18)
	{
		x = d->x - 5;
		while(x < d->x + 70)
		{
			data[y * S_W + x] = 0x99ff0000;
			++x;
		}
		++y;
	}
	if (fig == sphere)
		print_ttf(l->mimg, "SPHERE", 14, d);
	else if (fig == plane)
		print_ttf(l->mimg, "PLANE", 14, d);
	else if (fig == conus)
		print_ttf(l->mimg, "CONUS", 14, d);
	else if (fig == cylinder)
		print_ttf(l->mimg, "CYLINDER", 14, d);
	else if (fig == inv_sph)
		print_ttf(l->mimg, "INVERT", 14, d);
}

/*
** figure panel
*/

void		draw_text_menu2(t_lst *l)
{
	SDL_Rect	d;
	int			*data;
	int			h;

	data = (int *)l->mimg->pixels;
	h = -1;
	while (++h < S_W * S_H)
		data[h] = 0x88ffffff;
	print_ttf(l->mimg, "Figure:", 14, set_rect(&d, l->mrect.x - 5,
		l->mrect.y));
	draw_type_fig(l, 14, l->mrect.x + 55, l->mrect.y);
	print_ttf(l->mimg, "position:", 14, set_rect(&d, l->mrect.x - 5,
		l->mrect.y + 15));
	draw_vec(l, 14, set_rect(&d, l->mrect.x + 75,
		l->mrect.y + 15), &(l->scn->cur_fig->pos));
	print_ttf(l->mimg, "rotation:", 14, set_rect(&d, l->mrect.x - 5,
		l->mrect.y + 30));
	print_ttf(l->mimg, "CREATE FIGURE:", 14, set_rect(&d, l->mrect.x - 5,
		l->mrect.y + 50));
	draw_vec(l, 14, set_rect(&d, l->mrect.x + 75,
		l->mrect.y + 30), &(l->scn->cur_fig->alpha));
	draw_cre_button(l, sphere, set_rect(&d, l->mrect.x,
		l->mrect.y + 70));
	draw_cre_button(l, plane, set_rect(&d, l->mrect.x + 85,
		l->mrect.y + 70));
	draw_cre_button(l, conus, set_rect(&d, l->mrect.x,
		l->mrect.y + 100));
	draw_cre_button(l, inv_sph, set_rect(&d, l->mrect.x + 85,
		l->mrect.y + 100));
	draw_cre_button(l, cylinder, set_rect(&d, l->mrect.x,
		l->mrect.y + 130));
}

void		draw_type_lght(t_lst *l, int fsize, int x, int y)
{
	SDL_Rect d;

	d.x = x;
	d.y = y;
	if (l->scn->cur_lght->type == ambient)
		print_ttf(l->mimg, "AMBIENT", fsize, &d);
	else if (l->scn->cur_lght->type == direct)
		print_ttf(l->mimg, "DIRECT", fsize, &d);
	else if (l->scn->cur_lght->type == point)
		print_ttf(l->mimg, "POINT", fsize, &d);
	else if (l->scn->cur_lght->type == lconus)
		print_ttf(l->mimg, "CONUS", fsize, &d);
}

/*
** light panel
*/

void		draw_text_menu3(t_lst *l)
{
	SDL_Rect	d;
	int			*data;
	int			h;

	data = (int *)l->mimg->pixels;
	h = -1;
	while (++h < S_W * S_H)
		data[h] = 0x88ffffff;
	print_ttf(l->mimg, "Light:", 14, set_rect(&d, l->mrect.x - 5,
		l->mrect.y));
	draw_type_lght(l, 14, l->mrect.x + 55, l->mrect.y);
	print_ttf(l->mimg, "position:", 14, set_rect(&d, l->mrect.x - 5,
		l->mrect.x + 15));
	draw_vec(l, 14, set_rect(&d, l->mrect.x + 75,
		l->mrect.x + 15), &(l->scn->cur_lght->pos));
}

/*
** material panel
*/

void		draw_text_menu4(t_lst *l)
{
	SDL_Rect	d;
	int			*data;
	t_mat		*mat;
	int			h;

	data = (int *)l->mimg->pixels;
	h = -1;
	while (++h < S_W * S_H)
		data[h] = 0x88ffffff;
	mat = l->scn->cur_fig->mat;
	print_ttf(l->mimg, "Material:", 14, set_rect(&d, l->mrect.x - 5,
		l->mrect.y - 95));
	print_ttf(l->mimg, mat->name, 14, set_rect(&d, l->mrect.x + 75,
		l->mrect.y- 95));
	print_ttf(l->mimg, "color:", 14, set_rect(&d, l->mrect.x - 5,
		l->mrect.y - 75));
	draw_col(l, 14, set_rect(&d, l->mrect.x + 75,
		l->mrect.y - 75), &(l->scn->cur_fig->mat->col));
}
