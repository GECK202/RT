/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 15:16:15 by vkaron            #+#    #+#             */
/*   Updated: 2020/01/11 23:01:48 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		free_words(char **words, int res)
{
	int i;

	i = 0;
	while (words[i])
		free(words[i++]);
	free(words);
	return (res);
}

void	free_figs(t_fig *figs)
{
	t_fig *cur;
	t_fig *prev;

	if (!figs)
		return ;
	prev = figs;
	while (prev->next)
	{
		cur = prev->next;
		free(prev);
		prev = cur;
	}
	free(prev);
}

void	free_mats(t_mat *mats)
{
	t_mat *cur;
	t_mat *prev;

	if (!mats)
		return ;
	prev = mats;
	while (prev->next)
	{
		cur = prev->next;
		free(prev);
		prev = cur;
	}
	free(prev);
}

void	free_lghts(t_lght *lghts)
{
	t_lght *cur;
	t_lght *prev;

	if (!lghts)
		return ;
	prev = lghts;
	while (prev->next)
	{
		cur = prev->next;
		free(prev);
		prev = cur;
	}
	free(prev);
}

void	free_l(t_lst *lst)
{
	if (lst->set)
		free(lst->set);
	if (lst->scn)
	{
		if (lst->scn->figs)
			free_figs(lst->scn->figs);
		if (lst->scn->lghts)
			free_lghts(lst->scn->lghts);
		if (lst->scn->mats)
			free_mats(lst->scn->mats);
		free(lst->scn);
	}
	free(lst);
}

int		free_word_line(char **line, char **word)
{
	int i;

	i = 0;
	if (word)
	{
		if (word[i])
		{
			while (word[i])
			{
				free(word[i]);
				word[i] = NULL;
				++i;
			}
		}
		free(word);
		word = NULL;
	}
	if (*line)
	{
		free(*line);
		*line = NULL;
	}
	return (i);
}
