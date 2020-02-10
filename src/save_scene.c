/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   saveScene.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabraham <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 16:07:50 by vabraham          #+#    #+#             */
/*   Updated: 2020/02/05 16:07:51 by vabraham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	write_scene(int fd, t_lst *lst)
{
	ft_putstr_fd("[scene]:\n1	camera position: ", fd);
	ft_putstr_fd(get_thre_int((int)(lst->scn->cam_pos.x + lst->scn->cam_pos0.x),
		(int)(lst->scn->cam_pos.y + lst->scn->cam_pos0.y),
		(int)(lst->scn->cam_pos.z + lst->scn->cam_pos0.z)), fd);
	ft_putstr_fd("\n2	camera rotation: ", fd);
	ft_putstr_fd(get_thre_int((int)lst->alpha_cam.z,
		(int)lst->alpha_cam.y, (int)lst->alpha_cam.z), fd);
	ft_putstr_fd("\n3	camera focus_distance: ", fd);
	ft_putstr_fd(get_fnbr_to_string(1, lst->scn->cam_focus_dist, ""), fd);
	ft_putstr_fd("\n4	background color: ", fd);
	ft_putstr_fd(get_thre_int(lst->scn->bgc.r,
		lst->scn->bgc.g, lst->scn->bgc.b), fd);
	ft_putstr_fd("\n5	diffuse map: ", fd);
	ft_putstr_fd(lst->scn->diff_map.name_map, fd);
	ft_putstr_fd("\n6	fog enable:", fd);
	ft_putstr_fd(get_inbr_to_string(1, lst->scn->fog.enable, ""), fd);
	ft_putstr_fd("\n7	fog near:", fd);
	ft_putstr_fd(get_fnbr_to_string(1, lst->scn->fog.near, ""), fd);
	ft_putstr_fd("\n8	fog max transparent:", fd);
	ft_putstr_fd(get_fnbr_to_string(1, lst->scn->fog.max_tr, ""), fd);
	ft_putstr_fd("\n9	fog color:", fd);
	ft_putstr_fd(get_thre_int(lst->scn->fog.col.r,
		lst->scn->fog.col.g, lst->scn->fog.col.b), fd);
	ft_putstr_fd("\n\n", fd);
}

void	write_materials(int fd, t_lst *lst)
{
	t_mat *mat;

	mat = lst->scn->mats->next;
	while (mat)
	{
		ft_putstr_fd("[material]:\nname: ", fd);
		ft_putstr_fd(mat->name, fd);
		ft_putstr_fd("\n1	color: ", fd);
		ft_putstr_fd(get_thre_int(mat->col.r, mat->col.g, mat->col.b), fd);
		ft_putstr_fd("\n2	diffuse: ", fd);
		ft_putstr_fd(mat->diff_map.name_map, fd);
		ft_putstr_fd("\n3	normal: ", fd);
		ft_putstr_fd(mat->norm_map.name_map, fd);
		ft_putstr_fd("\n4	mask: ", fd);
		ft_putstr_fd(mat->mask_map.name_map, fd);
		ft_putstr_fd("\n5	specular: ", fd);
		ft_putstr_fd(get_inbr_to_string(1, mat->spec, ""), fd);
		ft_putstr_fd("\n6	reflection: ", fd);
		ft_putstr_fd(get_fnbr_to_string(1, mat->refl, ""), fd);
		ft_putstr_fd("\n7	transpare: ", fd);
		ft_putstr_fd(get_fnbr_to_string(1, mat->transpare, ""), fd);
		ft_putstr_fd("\n\n", fd);
		mat = mat->next;
	}
}

void	write_light0(int fd, t_lst *lst, t_lght *light)
{
	ft_putstr_fd("\n3	angle:", fd);
	ft_putstr_fd(get_fnbr_to_string(1, light->angle, ""), fd);
	ft_putstr_fd("\n4	intens:", fd);
	ft_putstr_fd(get_fnbr_to_string(1, light->ints, ""), fd);
	ft_putstr_fd("\n5	color:", fd);
	ft_putstr_fd(get_thre_int(light->col.r,
		light->col.g, light->col.b), fd);
	ft_putstr_fd("\n\n", fd);
}

void	write_light(int fd, t_lst *lst)
{
	t_lght *light;

	light = lst->scn->lghts;
	while (light)
	{
		ft_putstr_fd("[light]:\n0	type:", fd);
		if (light->type == ambient)
			ft_putstr_fd("ambient", fd);
		else if (light->type == point)
			ft_putstr_fd("point", fd);
		else if (light->type == direct)
			ft_putstr_fd("direct", fd);
		else if (light->type == lconus)
			ft_putstr_fd("lconus", fd);
		ft_putstr_fd("\n1	position:", fd);
		ft_putstr_fd(get_thre_float(light->pos.x,
			light->pos.y, light->pos.z), fd);
		ft_putstr_fd("\n2	direction:", fd);
		ft_putstr_fd(get_thre_float(light->dir.x,
			light->dir.y, light->dir.z), fd);
		write_light0(fd, lst, light);
		light = light->next;
	}
}

int		save_scene(t_lst *lst)
{
	int		fd;

	fd = get_file_scene();
	write_scene(fd, lst);
	write_materials(fd, lst);
	write_figure(fd, lst);
	write_light(fd, lst);
	close(fd);
	return (0);
}
