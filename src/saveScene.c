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

char	*get_Inbr_to_String(int  min, int num, char *s)
{
	char *c;

	c = malloc(sizeof(char) * 2);
	if (num < 0)
	{
		min *= -1;
		num *= -1;
	}
	*c = num % 10 + 48;
	c[1] = '\0';
	num /= 10;
	if (num == 0)
	{
		if (min == -1)
			return (ft_strjoin("-", ft_strjoin(c, s)));
		return (ft_strjoin(c, s));
	}
	return (get_Inbr_to_String(min, num, ft_strjoin(c, s)));
}

char	*get_Fnbr_to_String(int  min, float num, char *s)
{
	float tmp;

	tmp = num - (float)(int)num;
	if (tmp < 0)
		tmp *= -1;
	while (tmp != (float)(int)tmp)
		tmp *= 10;
	return (ft_strjoin(get_Inbr_to_String(1, (int)num, "."), get_Inbr_to_String(1, (tmp), "")));
}

int		get_file_scene()
{
	int		fd;
	int		numScene;
	char	*str;
	char	*strScene;
	
	str = "scenes/scene";
	numScene = 0;
	strScene = NULL;
	while (1)
	{
		if (strScene)
			free(strScene);
		strScene = ft_strjoin(str, get_Inbr_to_String(1, numScene, ""));
		if ((fd = open(strScene, O_WRONLY)) == -1)
			break ;
		close(fd);
		numScene++;
	}
	fd = open(strScene, O_WRONLY | O_CREAT);
	ft_putstr(strScene);
	return (fd);
}

char	*getThreeInt(int x, int y, int z)
{
	char *s;
	char *tmp;

	s = "";
	tmp = ft_strjoin(s, get_Inbr_to_String(1, x, ""));
	s = tmp;
	tmp = ft_strjoin(s, " ");
	free(s);
	s = tmp;

	tmp = ft_strjoin(s, get_Inbr_to_String(1, y, ""));
	free(s);
	s = tmp;
	tmp = ft_strjoin(s, " ");
	free(s);
	s = tmp;

	tmp = ft_strjoin(s, get_Inbr_to_String(1, z, ""));
	free(s);
	return (tmp);
}

char	*getThreeFloat(float x, float y, float z)
{
	char *s;
	char *tmp;

	s = "";
	tmp = ft_strjoin(s, get_Fnbr_to_String(1, x, ""));
	s = tmp;
	tmp = ft_strjoin(s, " ");
	free(s);
	s = tmp;

	tmp = ft_strjoin(s, get_Fnbr_to_String(1, y, ""));
	free(s);
	s = tmp;
	tmp = ft_strjoin(s, " ");
	free(s);
	s = tmp;

	tmp = ft_strjoin(s, get_Fnbr_to_String(1, z, ""));
	free(s);
	return (tmp);
}

void	write_scene(int fd, t_lst *lst)
{
	ft_putstr_fd("[scene]:\n1	camera position: ", fd);
	ft_putstr_fd(getThreeInt((int)(lst->scn->cam_pos.x + lst->scn->cam_pos0.x), (int)(lst->scn->cam_pos.y + lst->scn->cam_pos0.y), (int)(lst->scn->cam_pos.z + lst->scn->cam_pos0.z)), fd);
	ft_putstr_fd("\n2	camera rotation: ", fd);
	ft_putstr_fd(getThreeInt((int)lst->alpha_cam.z, (int)lst->alpha_cam.y, (int)lst->alpha_cam.z), fd);
	ft_putstr_fd("\n3	camera focus_distance: ", fd);
	ft_putstr_fd(get_Fnbr_to_String(1, lst->scn->cam_focus_dist, ""), fd);
	ft_putstr_fd("\n4	background color: ", fd);
	ft_putstr_fd(getThreeInt(lst->scn->bgc.r, lst->scn->bgc.g, lst->scn->bgc.b), fd);
	ft_putstr_fd("\n5	diffuse map: ", fd);
	ft_putstr_fd(lst->scn->diff_map.name_map, fd);
	ft_putstr_fd("\n6	fog enable:", fd);
	ft_putstr_fd(get_Inbr_to_String(1, lst->scn->fog.enable, ""), fd);
	ft_putstr_fd("\n7	fog near:", fd);
	ft_putstr_fd(get_Fnbr_to_String(1, lst->scn->fog.near, ""), fd);
	ft_putstr_fd("\n8	fog max transparent:", fd);
	ft_putstr_fd(get_Fnbr_to_String(1, lst->scn->fog.max_tr, ""), fd);
	ft_putstr_fd("\n9	fog color:", fd);
	ft_putstr_fd(getThreeInt(lst->scn->fog.col.r, lst->scn->fog.col.g, lst->scn->fog.col.b), fd);
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
		ft_putstr_fd(getThreeInt(mat->col.r, mat->col.g, mat->col.b), fd);
		ft_putstr_fd("\n2	diffuse: ", fd);
		ft_putstr_fd(mat->diff_map.name_map, fd);
		ft_putstr_fd("\n3	normal: ", fd);
		ft_putstr_fd(mat->norm_map.name_map, fd);
		ft_putstr_fd("\n4	mask: ", fd);
		ft_putstr_fd(mat->mask_map.name_map, fd);
		ft_putstr_fd("\n5	specular: ", fd);
		ft_putstr_fd(get_Inbr_to_String(1, mat->spec, ""), fd);
		ft_putstr_fd("\n6	reflection: ", fd);
		ft_putstr_fd(get_Fnbr_to_String(1, mat->refl, ""), fd);
		ft_putstr_fd("\n7	transpare: ", fd);
		ft_putstr_fd(get_Fnbr_to_String(1, mat->transpare, ""), fd);
		ft_putstr_fd("\n\n", fd);
		mat = mat->next;
	}
}

void	write_figure(int fd, t_lst *lst)
{
	t_fig *fig = lst->scn->figs;
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
		////////////
		// fig->ang = 20;
		////////////
		ft_putstr_fd("\n1	position:", fd);
		ft_putstr_fd(getThreeFloat(fig->pos.x, fig->pos.y, fig->pos.z), fd);
		ft_putstr_fd("\n2	direction:", fd);
		ft_putstr_fd(getThreeFloat(fig->dir.x, fig->dir.y, fig->dir.z), fd);
		ft_putstr_fd("\n3	rotation:", fd);
		ft_putstr_fd(getThreeFloat(fig->alpha.x, fig->alpha.y, fig->alpha.z), fd);
		ft_putstr_fd("\n4	radius:", fd);
		ft_putstr_fd(get_Fnbr_to_String(1, fig->rad, ""), fd);
		ft_putstr_fd("\n5	angle:", fd);
		ft_putstr_fd(get_Fnbr_to_String(1, fig->ang, ""), fd);
		ft_putstr_fd("\n6	limit:", fd);
		ft_putstr_fd(get_Fnbr_to_String(1, fig->limit.x, ""), fd);
		ft_putchar_fd(' ', fd);
		ft_putstr_fd(get_Fnbr_to_String(1, fig->limit.y, ""), fd);
		ft_putstr_fd("\n7	material: ", fd);
		ft_putstr_fd(fig->mat->name, fd);
		ft_putstr_fd("\n\n", fd);
		fig = fig->next;
	}
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
		ft_putstr_fd(getThreeFloat(light->pos.x, light->pos.y, light->pos.z), fd);
		ft_putstr_fd("\n2	direction:", fd);
		ft_putstr_fd(getThreeFloat(light->dir.x, light->dir.y, light->dir.z), fd);
		ft_putstr_fd("\n3	angle:", fd);
		ft_putstr_fd(get_Fnbr_to_String(1, light->angle, ""), fd);
		ft_putstr_fd("\n4	intens:", fd);
		ft_putstr_fd(get_Fnbr_to_String(1, light->ints, ""), fd);
		ft_putstr_fd("\n5	color:", fd);
		ft_putstr_fd(getThreeInt(light->col.r, light->col.g, light->col.b), fd);
		ft_putstr_fd("\n\n", fd);
		light = light->next;
	}
}

int		saveScene(t_lst *lst)
{
	int		fd;
	
	fd = get_file_scene();
	write_scene(fd, lst);
	write_materials(fd, lst);
	write_figure(fd, lst);
	write_light(fd, lst);
	// write_scene(1, lst);
	// write_materials(1, lst);
	// write_figure(1, lst);
	// write_light(1, lst);
	close(fd);
	return (0);
}
