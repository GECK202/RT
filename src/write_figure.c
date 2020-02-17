/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_figure.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/08 23:24:29 by vabraham          #+#    #+#             */
/*   Updated: 2020/02/18 00:13:10 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	write_figure0(int fd, t_fig *fig)
{
	char *buf;

	buf = get_thre_float(fig->alpha.x,
		fig->alpha.y, fig->alpha.z);
	ft_putstr_fd(buf, fd);
	ft_putstr_fd("\n4	radius:", fd);
	buf = get_fnbr_to_string(fig->rad);
	ft_putstr_fd(buf, fd);
	free(buf);
	ft_putstr_fd("\n5	angle:", fd);
	buf = get_fnbr_to_string(fig->ang);
	ft_putstr_fd(buf, fd);
	free(buf);
	ft_putstr_fd("\n6	limit:", fd);
	buf = get_fnbr_to_string(fig->limit.x);
	ft_putstr_fd(buf, fd);
	free(buf);
	ft_putchar_fd(' ', fd);
	buf = get_fnbr_to_string(fig->limit.y);
	ft_putstr_fd(buf, fd);
	free(buf);
	ft_putstr_fd("\n7	material: ", fd);
	ft_putstr_fd(fig->mat->name, fd);
	ft_putstr_fd("\n8	uv scale:", fd);
	buf = get_fnbr_to_string(fig->uvt.scale.x);
	ft_putstr_fd(buf, fd);
	free(buf);
	ft_putchar_fd(' ', fd);
	buf = get_fnbr_to_string(fig->uvt.scale.y);
	ft_putstr_fd(buf, fd);
	free(buf);
	ft_putstr_fd("\n9	uv rotation: ", fd);
	buf = get_fnbr_to_string(fig->uvt.rot);
	ft_putstr_fd(buf, fd);
	free(buf);
	ft_putstr_fd("\n10	uv move: ", fd);
	buf = get_fnbr_to_string(fig->uvt.move.x);
	ft_putstr_fd(buf, fd);
	free(buf);
	ft_putchar_fd(' ', fd);
	buf = get_fnbr_to_string(fig->uvt.move.y);
	ft_putstr_fd(buf, fd);
	free(buf);
	ft_putstr_fd("\n\n", fd);
}

void	write_figure(int fd, t_lst *lst)
{
	t_fig	*fig;
	char	*buf;

	fig = lst->scn->figs;
	while (fig)
	{
		ft_putstr_fd("[figure]:\n0	type:", fd);
		if (fig->type == sphere)
			ft_putstr_fd("sphere", fd);
		else if (fig->type == cylinder)
			ft_putstr_fd("cylinder", fd);
		else if (fig->type == plane)
			ft_putstr_fd("plane", fd);
		else if (fig->type == conus)
			ft_putstr_fd("conus", fd);
		else if (fig->type == inv_sph)
			ft_putstr_fd("inv_sph", fd);
		ft_putstr_fd("\n1	position:", fd);
		buf = get_thre_float(fig->pos.x, fig->pos.y, fig->pos.z);
		ft_putstr_fd(buf, fd);
		free(buf);
		ft_putstr_fd("\n2	center:", fd);
		buf = get_thre_float(fig->dir.x, fig->dir.y, fig->dir.z);
		ft_putstr_fd(buf, fd);
		free(buf);
		ft_putstr_fd("\n3	rotation:", fd);
		write_figure0(fd, fig);
		fig = fig->next;
	}
}
