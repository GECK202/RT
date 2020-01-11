/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_material1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 13:43:40 by vkaron            #+#    #+#             */
/*   Updated: 2020/01/11 22:53:29 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	load_map(t_map *map, char *filename)
{
	map->map = 0;
	map->data = 0;
	if (!filename)
		return ;
	map->map = IMG_Load(filename);
	if (map->map)
		map->data = (int*)(map->map->pixels);
}

int		cre_mat(t_lst *lst)
{
	t_mat	*mat;
	t_mat	*cur_mat;

	if (!(mat = (t_mat*)malloc(sizeof(t_mat))))
		return (0);
	mat->next = NULL;
	if (!lst->scn->mats)
	{
		ft_strcpy(mat->name, "default");
		mat->col = set_col(128, 128, 128, 255);
		load_map(&(mat->diff_map), 0);
		load_map(&(mat->norm_map), 0);
		mat->refl = 1000;
		mat->spec = 0;	
		lst->scn->mats = mat;
	}
	else
	{
		cur_mat = lst->scn->mats;
		while (cur_mat->next)
			cur_mat = cur_mat->next;
		cur_mat->next = mat;
	}
	lst->scn->cur_mat = mat;
	return (1);
}

int		set_name_mat(t_lst *lst, char *word)
{
	if (!word || !(*word))
		return (0);
	if (ft_strlen(word) > 17)
		return (0);
	ft_strncpy(lst->scn->cur_mat->name, word, 18);
	return (1);
}

int		set_diff_map_mat(t_lst *lst, char *word)
{
	if (!word || !(*word))
		return (0);
	load_map(&(lst->scn->cur_mat->diff_map), word);
	return (1);
}

int		set_norm_map_mat(t_lst *lst, char *word)
{
	if (!word || !(*word))
		return (0);
	load_map(&(lst->scn->cur_mat->norm_map), word);
	return (1);
}