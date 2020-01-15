/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 00:01:56 by vkaron            #+#    #+#             */
/*   Updated: 2020/01/14 16:21:03 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		check_valid_count(t_tag *ctag)
{
	if ((ctag->read_tag == SCENE && ctag->count < SCENE_LINES)
		|| (ctag->read_tag == FIGURE && ctag->count < FIGURE_LINES)
		|| (ctag->read_tag == LIGHT && ctag->count < LIGHT_LINES)
		|| (ctag->read_tag == MATERIAL && ctag->count < MATERIAL_LINES))
		return (0);
	return (1);
}

int		cre_cur_obj(t_lst *l, t_tag *ctag)
{
	if (ctag->cur_tag > 0)
	{
		if (!(check_valid_count(ctag)))
			return (0);
		if (ctag->cur_tag == MATERIAL)
		{
			if (!(l->set->cre_mat(l)))
				return (0);
		}
		if (ctag->cur_tag == FIGURE)
		{
			if (!(l->set->cre_fig(l)))
				return (0);
		}
		else if (ctag->cur_tag == LIGHT)
		{
			if (!(l->set->cre_lght(l)))
				return (0);
		}
	}
	return (1);
}

int		check_valid_iter(t_tag *ctag)
{
	if ((ctag->read_tag == SCENE && ctag->count > (SCENE_LINES - 1))
		|| (ctag->read_tag == FIGURE && ctag->count > (FIGURE_LINES - 1))
		|| (ctag->read_tag == LIGHT && ctag->count > (LIGHT_LINES - 1))
		|| (ctag->read_tag == MATERIAL && ctag->count > (MATERIAL_LINES - 1)))
		return (0);
	return (1);
}

int		check_tag(t_lst *l, char **word, t_tag *ctag)
{
	const char	tag[TAGS][11] = {"[scene]", "[material]", "[figure]", "[light]"};
	int res;

	res = 0;
	ctag->cur_tag = SCENE;
	while (ctag->cur_tag < TAGS)
	{
		if (ft_strcmp(word[0], tag[ctag->cur_tag]) == 0)
		{
			if (!(cre_cur_obj(l, ctag)))
				return (0);
			ctag->read_tag = ctag->cur_tag;
			ctag->count = 0;
			return (1);
		}
		ctag->cur_tag += 1;
	}
	if (!check_valid_iter(ctag))
		return (0);
	if (ctag->read_tag == SCENE)
		res = l->set->f_scn[ctag->count](l, word[1]);
	else if (ctag->read_tag == MATERIAL)
		res = l->set->f_mat[ctag->count](l, word[1]);
	else if (ctag->read_tag == FIGURE)
		res = l->set->f_fig[ctag->count](l, word[1]);
	else if (ctag->read_tag == LIGHT)
		res = l->set->f_lght[ctag->count](l, word[1]);
	ctag->count += 1;
	return (res);
}

void	reset_ctag(t_tag *ctag)
{
	ctag->read_tag = SCENE;
	ctag->cur_tag = SCENE;
	ctag->count = 0;
}

int		read_scene(t_lst *l, char *file)
{
	t_tag		ctag;
	char		**line;
	int			fd;
	char		**word;
	int			ch;

	if (!cre_mat(l) || (fd = open(file, 0x0000)) < 0)
		return (0);
	line = malloc(sizeof(char*));
	reset_ctag(&ctag);
	while ((ch = get_next_line(fd, line)) > 0)
	{
		word = ft_strsplit(*line, ':');
		if (!(*line) || !(word[0]) || word[0][0] == '\0')
		{
			free_word_line(line, word);
			continue ;
		}
		if (!(check_tag(l, word, &ctag)))
			return (close_free(fd, line, word, 0));
		free_word_line(line, word);
	}
	if (ch < 0)
		return (close_dir(fd, line, 0));
	return (close_free(fd, line, NULL, 1));
}
