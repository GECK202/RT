/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 00:01:56 by vkaron            #+#    #+#             */
/*   Updated: 2020/01/09 18:24:35 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		check_valid_count(t_col *iter)
{
	if ((iter->r == 0 && iter->b < 3) || (iter->r == 1 && iter->b < 9)
		|| (iter->r == 2 && iter->b < 4))
		return (0);
	return (1);
}

int		cre_cur_obj(t_lst *l, t_col *iter)
{
	if (iter->g == 1 || iter->g == 2)
	{
		if (!(check_valid_count(iter)))
			return (0);
		if (iter->g == 1)
		{
			if (!(l->set->cre_fig(l)))
				return (0);
		}
		else
		{
			if (!(l->set->cre_lght(l)))
				return (0);
		}
	}
	return (1);
}

int		check_valid_iter(t_col *iter)
{
	if ((iter->r == 0 && iter->b > 2) || (iter->r == 1 && iter->b > 8)
		|| (iter->r == 2 && iter->b > 3))
		return (0);
	return (1);
}

int		check_tag(t_lst *l, char **word, t_col *iter)
{
	const char	tag[3][9] = {"[scene]", "[figure]", "[light]"};

	iter->g = 0;
	while (iter->g < 3)
	{
		if (ft_strcmp(word[0], tag[iter->g]) == 0)
		{
			if (!(cre_cur_obj(l, iter)))
				return (0);
			iter->r = iter->g;
			iter->b = 0;
			return (1);
		}
		iter->g += 1;
	}
	if (!check_valid_iter(iter))
		return (0);
	if (iter->r == 0)
		l->set->f_scn[iter->b](l, word[1]);
	else if (iter->r == 1)
		l->set->f_fig[iter->b](l, word[1]);
	else if (iter->r == 2)
		l->set->f_lght[iter->b](l, word[1]);
	iter->b += 1;
	return (1);
}

int		read_scene(t_lst *l, char *file)
{
	t_col	iter;
	char	**line;
	int		fd;
	char	**word;
	int		ch;

	if ((fd = open(file, 0x0000)) < 0)
		return (0);
	line = malloc(sizeof(char*));
	iter = set_col(0, 0, 0);
	while ((ch = get_next_line(fd, line)) > 0)
	{
		word = ft_strsplit(*line, ':');
		if (!(*line) || !(word[0]) || word[0][0] == '\0')
		{
			free_word_line(line, word);
			continue ;
		}
		if (!(check_tag(l, word, &iter)))
			return (close_free(fd, line, word, 0));
		free_word_line(line, word);
	}
	if (ch < 0)
		return (close_dir(fd, line, 0));
	return (close_free(fd, line, NULL, 1));
}
