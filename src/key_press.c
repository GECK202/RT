/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_press.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 22:07:39 by vabraham          #+#    #+#             */
/*   Updated: 2020/01/09 19:38:16 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	cycle(t_lst *lst)
{
	while (lst->scn->cur_lght)
	{
		lst->scn->cur_lght = lst->scn->cur_lght->next;
		if (lst->scn->cur_lght && lst->scn->cur_lght->type == point)
			break ;
	}
	if (!lst->scn->cur_lght)
	{
		lst->scn->cur_lght = lst->scn->lghts;
		while (lst->scn->cur_lght)
		{
			if (lst->scn->cur_lght->type == point)
				break ;
			lst->scn->cur_lght = lst->scn->cur_lght->next;
		}
	}
}

int		mouse_light(t_lst *lst)
{
	if (!lst->mouse_light)
	{
		lst->scn->cur_lght = lst->scn->lghts;
		while (lst->scn->cur_lght)
		{
			if (lst->scn->cur_lght->type == point)
				break ;
			lst->scn->cur_lght = lst->scn->cur_lght->next;
		}
		if (!lst->scn->cur_lght)
			return (0);
		lst->mouse_light = 1;
	}
	else
		cycle(lst);
	return (1);
}

int		key_press1(SDL_Keycode key, t_lst *lst)
{
	if (key == SDLK_f)
	{
		if (lst->mouse_light)
			lst->mouse_light = 0;
		else if (lst->scn->cur_fig && lst->scn->cur_fig->next)
			lst->scn->cur_fig = lst->scn->cur_fig->next;
		else
			lst->scn->cur_fig = lst->scn->figs;
		return (0);
	}
	else if (key == SDLK_SPACE)
		lst->scn->shadow = (lst->scn->shadow) ? 0 : 1;
	else if (key == SDLK_l)
		multy_y(lst, 3);
	else if (key == SDLK_y)
		multy_z(lst, -3);
	else if (key == SDLK_t)
		multy_z(lst, 3);
	else if (key == SDLK_g && !mouse_light(lst))
		return (0);
	//rain(lst);
	//mlx_put_image_to_window(lst->mlx, lst->win, lst->img, 0, 0);
	return (1);
}

int		key_press0(SDL_Keycode key, t_lst *lst)
{
	if (key == SDLK_w || key == SDLK_s)
	{
		lst->alpha_cam.x += 3;
		if (key == SDLK_w)
			lst->alpha_cam.x -= 6;
		ret(lst);
		set_m4_rx(&lst->camera_x, lst->alpha_cam.x);
	}
	else if (key == SDLK_a || key == SDLK_d)
	{
		lst->alpha_cam.y -= 3;
		if (key == SDLK_d)
			lst->alpha_cam.y += 6;
		ret(lst);
		set_m4_ry(&lst->camera_y, lst->alpha_cam.y);
	}
	else if (key == SDLK_q || key == SDLK_e)
	{
		lst->alpha_cam.z -= 3;
		if (key == SDLK_q)
			lst->alpha_cam.z += 6;
		ret(lst);
		set_m4_rz(&lst->camera_z, lst->alpha_cam.z);
	}
	return (key_press1(key, lst));
}

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

char	*get_nbr(int n)
{
	char	*str;
	int		i;
	int		tmp;

	tmp = n;
	i = 1;
	while (tmp / 10 > 0)
	{
		tmp /= 10;
		i++;
	}
	if (!(str = malloc(sizeof(char) * (i + 1))))
		return ("none");
	str[i] = '\0';
	str[0] = '0';
	while (n > 0)
	{
		str[--i] = n % 10 + 48;
		n /= 10;
	}
	return (str);
}

int		scrin(t_lst *lst)
{

	SDL_Surface	*surface;
	char		*str;
	char		*tmp;
	int			fd;
	
	surface = SDL_CreateRGBSurfaceFrom(lst->img->pixels, lst->img->w, lst->img->h, lst->img->format->BitsPerPixel, lst->img->w * lst->img->format->BytesPerPixel, lst->img->format->Rmask, lst->img->format->Gmask, lst->img->format->Bmask, lst->img->format->Amask);
	int i = 0;
    int j = 0;
    for(i = 0; i < surface->h; i++){
        for(j = 0; j < surface->w;j++){
            Uint8 red = 0;
            Uint8 green = 0;
            Uint8 blue = 0;
            Uint32 pixel = getpixel(surface,j,i);
            SDL_GetRGB(pixel, surface->format, &red, &green, &blue);
            pixel = SDL_MapRGB(surface->format, red, green, blue);
            putpixel(surface,j,i,pixel);
        }
    }
    while (1)
    {
    	str = get_nbr(lst->num_file_for_screen);
    	tmp = ft_strjoin("screenshots/screen", str);
    	free(str);
    	if ((fd = open(tmp, O_RDONLY)) == -1)//проверить на утечки
    		break ;
		close(fd);
    	free(tmp);
    	lst->num_file_for_screen += 1;
	}
	IMG_SavePNG(surface, tmp);
	free(tmp);
	SDL_FreeSurface(surface);
	lst->num_file_for_screen += 1;
	return (0);
}

int		key_press(SDL_Keycode key, t_lst *lst)
{
	//t_lst	*lst;

	//lst = (t_lst*)l;
	//if (key == 53)
	//	exit(1);
	//else 
	// printf("%d\n", key);

	if (key == 8)
		return (saveScene(lst));
	else if (key == 13)//скриншот на enter
		return (scrin(lst));
	else if (key == 91){//смена пост-еффекта
		lst->postEffects -= 1;
		if (lst->postEffects < 0)
			lst->postEffects = END_FOR_POST_EFFECTS;
		return (0);
	}
	else if (key == 93){//смена пост-еффекта
		lst->postEffects += 1;
		if (lst->postEffects > END_FOR_POST_EFFECTS)
			lst->postEffects = 0;
		return (0);
	}

	else if (key == SDLK_LEFT)//123)
		lst->scn->cam_pos0.x -= 1;
	else if (key == SDLK_RIGHT)//124)
		lst->scn->cam_pos0.x += 1;
	else if (key == SDLK_UP)//126)
		lst->scn->cam_pos0.z += 1;
	else if (key == SDLK_DOWN)//125)
		lst->scn->cam_pos0.z -= 1;
	else if (key == SDLK_COMMA)//43)
		lst->scn->cam_pos0.y -= 1;
	else if (key == SDLK_PERIOD)//47)
		lst->scn->cam_pos0.y += 1;
	else if (key == SDLK_i)//34)
		multy_x(lst, 3);
	else if (key == SDLK_o)//31)
		multy_x(lst, -3);
	else if (key == SDLK_k)//40)
		multy_y(lst, -3);
	return (key_press0(key, lst));
}
