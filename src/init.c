/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 13:37:19 by vkaron            #+#    #+#             */
/*   Updated: 2019/11/30 19:00:19 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	init_f_read(t_lst *lst)
{
	lst->set->f_scn[0] = set_pos_cam;
	lst->set->f_scn[1] = set_rot_cam;
	lst->set->f_scn[2] = set_col_bgc;
	lst->set->cre_fig = cre_fig;
	lst->set->f_fig[0] = set_type_fig;
	lst->set->f_fig[1] = set_pos_fig;
	lst->set->f_fig[2] = set_dir_fig;
	lst->set->f_fig[3] = set_rot_fig;
	lst->set->f_fig[4] = set_rad_fig;
	lst->set->f_fig[5] = set_ang_fig;
	lst->set->f_fig[6] = set_col_fig;
	lst->set->f_fig[7] = set_spec_fig;
	lst->set->f_fig[8] = set_refl_fig;
	lst->set->cre_lght = cre_lght;
	lst->set->f_lght[0] = set_type_lght;
	lst->set->f_lght[1] = set_pos_lght;
	lst->set->f_lght[2] = set_dir_lght;
	lst->set->f_lght[3] = set_ints_lght;
}

void	init_mlx(t_lst *lst)
{
	int	bpp;
	int	sline;
	int	endian;

	lst->mlx = mlx_init();
	lst->win = mlx_new_window(lst->mlx, S_W, S_H, "RTV1");
	lst->img = mlx_new_image(lst->mlx, S_W, S_H);
	lst->data = (int *)mlx_get_data_addr(lst->img,
		&bpp, &sline, &endian);
	mlx_hook(lst->win, 2, 0, key_press, (void *)lst);
	mlx_hook(lst->win, 17, 0, close_window, (void *)0);
	mlx_hook(lst->win, 6, 0, mouse_move, (void *)lst);
	mlx_hook(lst->win, 4, 0, mouse_press, (void *)lst);
}

int		scene_init(t_lst *lst, char *file)
{
	if (!(lst->set = (t_read*)malloc(sizeof(t_read))))
		return (0);
	init_f_read(lst);
	if (!(lst->scn = (t_scn*)malloc(sizeof(t_scn))))
		return (0);
	lst->scn->bgc = 0;
	lst->scn->cur_fig = NULL;
	lst->scn->cur_lght = NULL;
	lst->scn->figs = NULL;
	lst->scn->lghts = NULL;
	lst->scn->shadow = 1;
	lst->mouse_light = 0;
	lst->change = 0;
	lst->scn->cam_pos0.x = 0;
	lst->scn->cam_pos0.y = 0;
	lst->scn->cam_pos0.z = 0;
	if (!(read_scene(lst, file)))
		return (0);
	if (!lst->scn->cur_fig || !lst->scn->cur_lght)
		return (0);
	return (1);
}
