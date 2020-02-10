/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_figure.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabraham <vabraham@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/08 23:24:29 by vabraham          #+#    #+#             */
/*   Updated: 2020/02/10 22:50:56 by vabraham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	write_figure0(int fd, t_lst *lst, t_fig *fig)
{
	ft_putstr_fd(get_thre_float(fig->alpha.x,
		fig->alpha.y, fig->alpha.z), fd);
	ft_putstr_fd("\n4	radius:", fd);
	ft_putstr_fd(get_fnbr_to_string(1, fig->rad, ""), fd);
	ft_putstr_fd("\n5	angle:", fd);
	ft_putstr_fd(get_fnbr_to_string(1, fig->ang, ""), fd);
	ft_putstr_fd("\n6	limit:", fd);
	ft_putstr_fd(get_fnbr_to_string(1, fig->limit.x, ""), fd);
	ft_putchar_fd(' ', fd);
	ft_putstr_fd(get_fnbr_to_string(1, fig->limit.y, ""), fd);
	ft_putstr_fd("\n7	material: ", fd);
	ft_putstr_fd(fig->mat->name, fd);
	ft_putstr_fd("\n8	uv scale:", fd);
	ft_putstr_fd(get_fnbr_to_string(1, fig->uvt.scale.x, ""), fd);
	ft_putchar_fd(' ', fd);
	ft_putstr_fd(get_fnbr_to_string(1, fig->uvt.scale.y, ""), fd);
	ft_putstr_fd("\n9	uv rotation: ", fd);
	ft_putstr_fd(get_fnbr_to_string(1, fig->uvt.rot, ""), fd);
	ft_putstr_fd("\n10	uv move: ", fd);
	ft_putstr_fd(get_fnbr_to_string(1, fig->uvt.move.x, ""), fd);
	ft_putchar_fd(' ', fd);
	ft_putstr_fd(get_fnbr_to_string(1, fig->uvt.move.y, ""), fd);
	ft_putstr_fd("\n\n", fd);
}

void	write_figure(int fd, t_lst *lst)
{
	t_fig *fig;

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
		ft_putstr_fd(get_thre_float(fig->pos.x, fig->pos.y, fig->pos.z), fd);
		ft_putstr_fd("\n2	center:", fd);
		ft_putstr_fd(get_thre_float(fig->dir.x, fig->dir.y, fig->dir.z), fd);
		ft_putstr_fd("\n3	rotation:", fd);
		write_figure0(fd, lst, fig);
		fig = fig->next;
	}
}
