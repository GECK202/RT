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

int					set_cam_focus_dist(t_lst *lst, char *word)
{
	if (!word)
		return (0);
	lst->scn->cam_focus_dist = ft_atof(word);
	return (1);
}

int		set_col_bgc(t_lst *lst, char *word)
{
	char		**col;

	if (!word)
		return (0);
	col = ft_strsplit(word, ' ');
	if (col[0] && col[1] && col[2])
	{
		lst->scn->bgc.r = clamp(ft_atoi(col[0]), 0, 255);
		lst->scn->bgc.g = clamp(ft_atoi(col[1]), 0, 255);
		lst->scn->bgc.b = clamp(ft_atoi(col[2]), 0, 255);
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

int					set_fog_enable(t_lst *lst, char *word)
{
	float e;

	if (!word)
		return (0);
	e = ft_atof(word);
	if (e != 1)
		e = 0;
	lst->scn->fog.enable = e;
	return (1);
}

int					set_fog_near(t_lst *lst, char *word)
{
	if (!word)
		return (0);
	lst->scn->fog.near = ft_atof(word);
	return (1);
}

int					set_fog_max_tr(t_lst *lst, char *word)
{
	if (!word)
		return (0);
	lst->scn->fog.max_tr = clampf(ft_atof(word), 0, 1);
	return (1);
}

int					set_fog_color(t_lst *lst, char *word)
{
	char	**col;
	t_fog	*fog;

	if (!word)
		return (0);
	fog = &lst->scn->fog;
	col = ft_strsplit(word, ' ');
	if (col[0] && col[1] && col[2])
	{
		fog->col.r = clamp(ft_atoi(col[0]), 0, 255);
		fog->col.g = clamp(ft_atoi(col[1]), 0, 255);
		fog->col.b = clamp(ft_atoi(col[2]), 0, 255);
		return (free_words(col, 1));
	}
	return (free_words(col, 0));
}
