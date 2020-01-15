/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_scn.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 15:30:34 by vkaron            #+#    #+#             */
/*   Updated: 2020/01/14 16:47:30 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		set_pos_cam(t_lst *lst, char *word)
{
	char	**coord;

	if (!word)
		return (0);
	coord = ft_strsplit(word, ' ');
	if (coord[0] && coord[1] && coord[2])
	{
		lst->scn->cam_pos.x = ft_atof(coord[0]);
		lst->scn->cam_pos.y = ft_atof(coord[1]);
		lst->scn->cam_pos.z = ft_atof(coord[2]);
		return (free_words(coord, 1));
	}
	return (free_words(coord, 0));
}

int		set_rot_cam(t_lst *lst, char *word)
{
	char	**coord;

	if (!word)
		return (0);
	coord = ft_strsplit(word, ' ');
	if (coord[0] && coord[1] && coord[2])
	{
		lst->alpha_cam.x = ft_atof(coord[0]);
		lst->alpha_cam.y = ft_atof(coord[1]);
		lst->alpha_cam.z = ft_atof(coord[2]);
		set_m4_rx(&lst->camera_x, lst->alpha_cam.x);
		set_m4_ry(&lst->camera_y, lst->alpha_cam.y);
		set_m4_rz(&lst->camera_z, lst->alpha_cam.z);
		return (free_words(coord, 1));
	}
	return (free_words(coord, 0));
}

int		set_col_bgc(t_lst *lst, char *word)
{
	char		**col;
	SDL_Color	color;

	if (!word)
		return (0);
	col = ft_strsplit(word, ' ');
	if (col[0] && col[1] && col[2])
	{
		color.r = clamp(ft_atoi(col[0]), 0, 255);
		color.g = clamp(ft_atoi(col[1]), 0, 255);
		color.b = clamp(ft_atoi(col[2]), 0, 255);
		lst->scn->bgc = (color.r << 16) | (color.g << 8) | color.b;
		return (free_words(col, 1));
	}
	return (free_words(col, 0));
}

int		set_diff_map_scn(t_lst *lst, char *word)
{
	char **filename;

	if (!word || !(*word))
		return (0);
	filename = ft_strsplit(word, ' ');
	load_map(&(lst->scn->diff_map), filename[0]);
	return (free_words(filename, 1));
}
