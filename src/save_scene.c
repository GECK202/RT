/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabraham <vabraham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/05 16:07:50 by vabraham          #+#    #+#             */
/*   Updated: 2020/02/18 17:23:21 by vabraham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	write_scene(int fd, t_lst *lst)
{
	char *buf;

	buf = get_thre_int((int)(lst->scn->cam_pos.x + lst->scn->cam_pos0.x),
		(int)(lst->scn->cam_pos.y + lst->scn->cam_pos0.y),
		(int)(lst->scn->cam_pos.z + lst->scn->cam_pos0.z));
	ft_putstr_fd(buf, fd);
	free(buf);
	ft_putstr_fd("\n2	camera rotation: ", fd);
	buf = get_thre_int((int)lst->alpha_cam.z,
		(int)lst->alpha_cam.y, (int)lst->alpha_cam.z);
	ft_putstr_fd(buf, fd);
	free(buf);
	ft_putstr_fd("\n3	camera focus_distance: ", fd);
	buf = get_fnbr_to_string(lst->scn->cam_focus_dist);
	ft_putstr_fd(buf, fd);
	free(buf);
	ft_putstr_fd("\n4	background color: ", fd);
	buf = get_thre_int(lst->scn->bgc.r,
		lst->scn->bgc.g, lst->scn->bgc.b);
	ft_putstr_fd(buf, fd);
	free(buf);
	ft_putstr_fd("\n5	diffuse map: ", fd);
	ft_putstr_fd(lst->scn->diff_map.name_map, fd);
	ft_putstr_fd("\n6	fog enable:", fd);
	buf = get_inbr_to_string(lst->scn->fog.enable, "");
	ft_putstr_fd(buf, fd);
	free(buf);
	ft_putstr_fd("\n7	fog near:", fd);
	buf = get_fnbr_to_string(lst->scn->fog.near);
	ft_putstr_fd(buf, fd);
	free(buf);
	ft_putstr_fd("\n8	fog max transparent:", fd);
	buf = get_fnbr_to_string(lst->scn->fog.max_tr);
	ft_putstr_fd(buf, fd);
	free(buf);
	ft_putstr_fd("\n9	fog color:", fd);
	buf = get_thre_int(lst->scn->fog.col.r,
		lst->scn->fog.col.g, lst->scn->fog.col.b);
	ft_putstr_fd(buf, fd);
	free(buf);
	ft_putstr_fd("\n10	invert intersect surface: ", fd);
	buf = get_inbr_to_string(lst->scn->inv_surf, "");
	ft_putstr_fd(buf, fd);
	free(buf);
	ft_putstr_fd("\n\n", fd);
}

void	write_materials(int fd, t_lst *lst, t_mat *mat)
{
	char *buf;
	mat = lst->scn->mats->next;
	while (mat)
	{
		ft_putstr_fd("[material]:\nname: ", fd);
		ft_putstr_fd(mat->name, fd);
		ft_putstr_fd("\n1	color: ", fd);
		buf = get_thre_int(mat->col.r, mat->col.g, mat->col.b);
		ft_putstr_fd(buf, fd);
		free(buf);
		ft_putstr_fd("\n2	diffuse: ", fd);
		ft_putstr_fd(mat->diff_map.name_map, fd);
		ft_putstr_fd("\n3	normal: ", fd);
		ft_putstr_fd(mat->norm_map.name_map, fd);
		ft_putstr_fd("\n4	mask: ", fd);
		ft_putstr_fd(mat->mask_map.name_map, fd);
		ft_putstr_fd("\n5	specular: ", fd);
		buf = get_inbr_to_string(mat->spec, "");
		ft_putstr_fd(buf, fd);
		free(buf);
		ft_putstr_fd("\n6	reflection: ", fd);
		buf = get_fnbr_to_string(mat->refl);
		ft_putstr_fd(buf, fd);
		free(buf);
		ft_putstr_fd("\n7	refraction: ", fd);
		buf = get_fnbr_to_string(mat->refr);
		ft_putstr_fd(buf, fd);
		free(buf);
		ft_putstr_fd("\n8	transpare: ", fd);
		buf = get_fnbr_to_string(mat->transpare);
		ft_putstr_fd(buf, fd);
		free(buf);
		ft_putstr_fd("\n\n", fd);
		mat = mat->next;
	}
}

void	write_light0(int fd, t_lght *light)
{
	char *buf;

	ft_putstr_fd("\n3	angle:", fd);
	buf = get_fnbr_to_string(light->angle);
	ft_putstr_fd(buf, fd);
	free(buf);
	ft_putstr_fd("\n4	intens:", fd);
	buf = get_fnbr_to_string(light->ints);
	ft_putstr_fd(buf, fd);
	free(buf);
	ft_putstr_fd("\n5	color:", fd);
	buf = get_thre_int(light->col.r,
		light->col.g, light->col.b);
	ft_putstr_fd(buf, fd);
	free(buf);
	ft_putstr_fd("\n\n", fd);
}

void	write_light(int fd, t_lst *lst)
{
	t_lght	*light;
	char	*buf;

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
		buf = get_thre_float(light->pos.x,
			light->pos.y, light->pos.z);
		ft_putstr_fd(buf, fd);
		free(buf);
		ft_putstr_fd("\n2	direction:", fd);
		buf = get_thre_float(light->dir.x,
			light->dir.y, light->dir.z);
		ft_putstr_fd(buf, fd);
		free(buf);
		write_light0(fd, light);
		light = light->next;
	}
}

int		save_scene(t_lst *lst)
{
	int		fd;
	t_mat	*mat;

	mat = NULL;
	fd = get_file_scene();
	ft_putstr_fd("[scene]:\n1	camera position: ", fd);
	write_scene(fd, lst);
	write_materials(fd, lst, mat);
	write_figure(fd, lst);
	write_light(fd, lst);
	close(fd);
	return (0);
}
