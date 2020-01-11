/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_fig1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 13:43:40 by vkaron            #+#    #+#             */
/*   Updated: 2020/01/12 01:18:44 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		cre_fig(t_lst *lst)
{
	t_fig	*fig;
	t_fig	*cur_fig;

	if (!(fig = (t_fig*)malloc(sizeof(t_fig))))
		return (0);
	fig->next = NULL;
	if (!lst->scn->figs)
		lst->scn->figs = fig;
	else
	{
		cur_fig = lst->scn->figs;
		while (cur_fig->next)
			cur_fig = cur_fig->next;
		cur_fig->next = fig;
	}
	lst->scn->cur_fig = fig;
	return (1);
}

int		set_type_fig(t_lst *lst, char *word)
{
	const char	f_type[4][10] = {"sphere", "cylinder", "plane", "conus"};
	int			i;
	int			type;

	i = -1;
	type = -1;
	while (++i < 4)
	{
		if (ft_strcmp(word, f_type[i]) == 0)
		{
			type = i;
			break ;
		}
	}
	if (type < 0)
		return (0);
	lst->scn->cur_fig->type = type;
	return (1);
}

int		set_pos_fig(t_lst *lst, char *word)
{
	char	**coord;
	t_fig	*fig;

	if (!word)
		return (0);
	fig = lst->scn->cur_fig;
	coord = ft_strsplit(word, ' ');
	if (coord[0] && coord[1] && coord[2])
	{
		fig->begin_pos.x = ft_atof(coord[0]);
		fig->begin_pos.y = ft_atof(coord[1]);
		fig->begin_pos.z = ft_atof(coord[2]);
		fig->pos.x = fig->begin_pos.x;
		fig->pos.y = fig->begin_pos.y;
		fig->pos.z = fig->begin_pos.z;
		return (free_words(coord, 1));
	}
	return (free_words(coord, 0));
}

int		set_dir_fig(t_lst *lst, char *word)
{
	char	**coord;
	t_fig	*fig;

	if (!word)
		return (0);
	fig = lst->scn->cur_fig;
	coord = ft_strsplit(word, ' ');
	if (coord[0] && coord[1] && coord[2])
	{
		fig->begin.x = ft_atof(coord[0]);
		fig->begin.y = ft_atof(coord[1]);
		fig->begin.z = ft_atof(coord[2]);
		fig->dir.x = fig->begin.x;
		fig->dir.y = fig->begin.y;
		fig->dir.z = fig->begin.z;
		fig->look = cre_vec3(0,0,1);
		return (free_words(coord, 1));
	}
	return (free_words(coord, 0));
}

int		set_rot_fig(t_lst *lst, char *word)
{
	char	**coord;
	t_fig	*fig;

	if (!word)
		return (0);
	fig = lst->scn->cur_fig;
	coord = ft_strsplit(word, ' ');
	if (coord[0] && coord[1] && coord[2])
	{
		fig->alpha.x = ft_atof(coord[0]);
		fig->alpha.y = ft_atof(coord[1]);
		fig->alpha.z = ft_atof(coord[2]);
		set_m4_rx(&fig->mat_x, fig->alpha.x);
		set_m4_ry(&fig->mat_y, fig->alpha.y);
		set_m4_rz(&fig->mat_z, fig->alpha.z);
		mult_m3(&lst->scn->cur_fig->dir,
		lst->scn->cur_fig->begin, lst->scn->cur_fig->mat_z);
		mult_m3(&lst->scn->cur_fig->dir,
		lst->scn->cur_fig->dir, lst->scn->cur_fig->mat_x);
		mult_m3(&lst->scn->cur_fig->dir,
		lst->scn->cur_fig->dir, lst->scn->cur_fig->mat_y);
		return (free_words(coord, 1));
	}
	return (free_words(coord, 0));
}

int		set_mat_fig(t_lst *lst, char *word)
{
	char	**name;
	t_mat	*mat;
	char	mat_name[18];
	int		flag;

	if (!word || !(*word))
		return (0);
	if (!(name = ft_strsplit(word, ' ')) || !(name[0]))
	{
		lst->scn->cur_fig->mat = lst->scn->mats;
		return (free_words(name, 1));
	}
	ft_strncpy(mat_name, name[0], 18);
	mat = lst->scn->mats;
	flag = 0;
	while (mat)
	{
		if (ft_strcmp(mat_name, mat->name) == 0)
		{
			lst->scn->cur_fig->mat = mat;
			flag = 1;
			break; 
		}
		mat = mat->next;
	}
	if (!flag)
		lst->scn->cur_fig->mat = lst->scn->mats;
	return (free_words(name, 1));
}
