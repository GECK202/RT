/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 13:37:19 by vkaron            #+#    #+#             */
/*   Updated: 2020/01/14 16:50:42 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	init_f_read(t_lst *lst)
{
	lst->set->f_scn[0] = set_pos_cam;
	lst->set->f_scn[1] = set_rot_cam;
	lst->set->f_scn[2] = set_col_bgc;
	lst->set->f_scn[3] = set_diff_map_scn;
	lst->set->cre_fig = cre_fig;
	lst->set->f_fig[0] = set_type_fig;
	lst->set->f_fig[1] = set_pos_fig;
	lst->set->f_fig[2] = set_dir_fig;
	lst->set->f_fig[3] = set_rot_fig;
	lst->set->f_fig[4] = set_rad_fig;
	lst->set->f_fig[5] = set_ang_fig;
	lst->set->f_fig[6] = set_mat_fig;
	lst->set->cre_lght = cre_lght;
	lst->set->f_lght[0] = set_type_lght;
	lst->set->f_lght[1] = set_pos_lght;
	lst->set->f_lght[2] = set_dir_lght;
	lst->set->f_lght[3] = set_ints_lght;
	lst->set->cre_mat = cre_mat;
	lst->set->f_mat[0] = set_name_mat;
	lst->set->f_mat[1] = set_col_mat;
	lst->set->f_mat[2] = set_diff_map_mat;
	lst->set->f_mat[3] = set_norm_map_mat;
	lst->set->f_mat[4] = set_spec_mat;
	lst->set->f_mat[5] = set_refl_mat;
	lst->set->f_mat[6] = set_transpare_mat;
}

int	init_sdl(t_lst *lst)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return (0);
	lst->win = 0;
	lst->win = SDL_CreateWindow("RT", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, S_W, S_H, SDL_WINDOW_SHOWN);
	if (!lst->win)
		return (0);
	lst->img = SDL_GetWindowSurface(lst->win);
	lst->data = (int *)lst->img->pixels;
	return (1);
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
	lst->scn->mats = NULL;
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
