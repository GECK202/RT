/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 14:24:16 by vabraham          #+#    #+#             */
/*   Updated: 2020/01/14 16:50:14 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

# define S_W (800)
# define S_H (600)
# define H_W (S_W / 2)
# define H_H (S_H / 2)
# define RATIO ((float)S_W / (float)S_H)
# define POT (16)

# define INFINITY (1e999)
# define MIN_OFFSET (0.01f)
# define RECURCE_DEPTH (3)

# define TAGS (4)
# define SCENE (0)
# define FIGURE (2)
# define LIGHT (3)
# define MATERIAL (1)

# define SCENE_LINES (4)
# define FIGURE_LINES (7)
# define LIGHT_LINES (4)
# define MATERIAL_LINES (7)

# define END_FOR_POST_EFFECTS 10

# include <pthread.h>
# include <math.h>
# include <stdlib.h>
# include <unistd.h>
# include "libft.h"
//# include <mlx.h>
# include <SDL2/SDL.h>
# include <SDL2/SDL_image.h>
//# include <SDL2/SDL_ttf.h>

typedef struct		s_tag
{
	int				read_tag;
	int				cur_tag;
	int				count;
}					t_tag;

typedef	struct		s_vec3
{
	float			x;
	float			y;
	float			z;
	float			w;
}					t_vec3;

typedef struct		s_mat3
{
	t_vec3			f1;
	t_vec3			f2;
	t_vec3			f3;
}					t_mat3;

typedef struct		s_map
{
	SDL_Surface		*map;
	int				*data;
}					t_map;


typedef struct		s_mat
{
	char			name[18];
	SDL_Color		col;
	t_map			diff_map;
	t_map			norm_map;
	int				spec;
	float			refl;
	float			transpare;
	struct s_mat	*next;
}					t_mat;


typedef enum		e_tfig
{
	sphere,
	cylinder,
	plane,
	conus
}					t_tfig;

typedef struct		s_fig
{
	t_tfig			type;
	t_vec3			pos;
	t_vec3			begin_pos;
	t_vec3			dir;
	t_vec3			begin;
	t_vec3			look;
	float			rad;
	float			ang;
	t_vec3			alpha;
	t_mat3			mat_x;
	t_mat3			mat_y;
	t_mat3			mat_z;
	t_mat			*mat;
	struct s_fig	*next;
}					t_fig;

typedef enum		e_tlght
{
	ambient,
	point,
	direct
}					t_tlght;

typedef struct		s_lght
{
	t_tlght			type;
	t_vec3			pos;
	t_vec3			begin_pos;
	t_vec3			dir;
	float			ints;
	struct s_lght	*next;
}					t_lght;

typedef struct		s_isec
{
	float			t;
	t_fig			*fig;
	float			check;
	t_vec3			n;
	t_vec3			uv;
}					t_isec;

typedef struct		s_scn
{
	t_fig			*figs;
	t_lght			*lghts;
	t_mat			*mats;
	t_fig			*cur_fig;
	t_lght			*cur_lght;
	t_mat			*cur_mat;
	t_vec3			cam_pos;
	t_vec3			cam_pos0;
	int				bgc;
	int				shadow;
	t_map			diff_map;
}					t_scn;

typedef struct		s_lst
{
	void			*mlx;
	SDL_Window		*win;
	SDL_Surface		*img;
	int				*data;
	
	SDL_Point		cursor;

	int				postEffects;
	int				*data_dop;
	int				num_file_for_screen;
	int				t;
	
	int				n0;
	int				n1;
	int				n2;
	int				change;
	int				pot;
	int				mouse_light;
	t_vec3			alpha_cam;
	t_mat3			camera_x;
	t_mat3			camera_y;
	t_mat3			camera_z;
	t_scn			*scn;
	struct s_read	*set;
	int				norm;
}					t_lst;

typedef int			(*t_r_fig)(t_lst*, char*);
typedef int			(*t_r_mat)(t_lst*, char*);
typedef int			(*t_r_lght)(t_lst*, char*);
typedef int			(*t_r_scn)(t_lst*, char*);

typedef struct		s_read
{
	t_r_scn			f_scn[5];
	int				(*cre_mat)(t_lst*);
	t_r_mat			f_mat[7];//Тест с прозрачностью
	int				(*cre_fig)(t_lst*);
	t_r_fig			f_fig[7];
	int				(*cre_lght)(t_lst*);
	t_r_lght		f_lght[4];
}					t_read;

typedef struct		s_trc
{
	t_vec3			o;
	t_vec3			d;
	t_vec3			p;
	float			min;
	float			max;
}					t_trc;

typedef struct		s_l_prm
{
	t_vec3			p;
	t_vec3			n;
	t_vec3			v;
}					t_l_prm;

void				postEffects(t_lst *lst);
int					set_transpare_mat(t_lst *lst, char *word);

int					scene_init(t_lst *lst, char *file);
void				init_f_read(t_lst *lst);
int					check_tag(t_lst *l, char **word, t_tag *ctag);
int					read_scene(t_lst *l, char *file);
int					set_diff_map_scn(t_lst *lst, char *word);

int					init_sdl(t_lst *lst);

void				free_figs(t_fig *figs);
void				free_lghts(t_lght *lghts);
void				free_mats(t_mat *mats);
void				free_l(t_lst *lst);
int					free_word_line(char **line, char **word);
int					close_free(int fd, char **line, char **word, int ret);
int					free_words(char **words, int res);
int					close_dir(int fd, char **line, int ret);

int					key_press(SDL_Keycode key, t_lst *lst);
int					close_window(void *lst);
void				mouse_move(SDL_MouseMotionEvent *e, t_lst *lst);
void				mouse_press(SDL_MouseButtonEvent *e, t_lst *lst);
void				mouse_weel(Sint32 y, t_lst *lst);

int					clamp(int n, int min, int max);
float				dot(t_vec3 v1, t_vec3 v2);
t_vec3				cross(t_vec3 v1, t_vec3 v2);
float				len_vec3(t_vec3 v);
float				modul(t_vec3 v);
t_vec3				set_vec3(t_vec3 src);
t_vec3				cre_vec3(float x, float y, float z);
t_vec3				minus_vec3(t_vec3 src1, t_vec3 src2);
t_vec3				plus_vec3(t_vec3 src1, t_vec3 src2);
t_vec3				mult_vec3f(t_vec3 src, float n);
t_vec3				div_vec3f(t_vec3 src, float n);
t_vec3				invert_vec3(t_vec3 src);
SDL_Color			set_col(int r, int g, int b, int a);
t_vec3				refl_r(t_vec3 l, t_vec3 n);

void				sel_fig_check(t_vec3 *t, t_vec3 o, t_vec3 d,
									t_fig *cur_fig);
void				intersec_sph(t_vec3 *t, t_vec3 o, t_vec3 d, t_fig *sph);
void				intersec_cyl(t_vec3 *t, t_vec3 o, t_vec3 d, t_fig *cyl);
void				intersec_pln(t_vec3 *t, t_vec3 o, t_vec3 d, t_fig *pln);
void				intersec_con(t_vec3 *t, t_vec3 o, t_vec3 d, t_fig *con);

void				cls_isec(t_isec *cisec, t_lst *lst, t_trc trc);
int					trace(t_lst *lst, t_trc trc, int depth);
float				light(t_lst *lst, t_l_prm b, t_fig *fig);

void				rain(t_lst *lst);
void				set_m4_rz(t_mat3 *m, float fi);
void				set_m4_rx(t_mat3 *m, float fi);
void				set_m4_ry(t_mat3 *m, float fi);
void				mult_m3(t_vec3 *d, t_vec3 s, t_mat3 m);
void				rot_v3z(t_vec3 *dst, t_vec3 *src, float fi);
void				rot_v3x(t_vec3 *dst, t_vec3 *src, float fi);
void				rot_v3y(t_vec3 *dst, t_vec3 *src, float fi);

float				ft_atof(char *f);
int					set_pos_cam(t_lst *lst, char *word);
int					set_rot_cam(t_lst *lst, char *word);
int					set_col_bgc(t_lst *lst, char *word);

int					cre_mat(t_lst *lst);
void				load_map(t_map *map, char *filename);
int					set_name_mat(t_lst *lst, char *word);
int					set_col_mat(t_lst *lst, char *word);
int					set_diff_map_mat(t_lst *lst, char *word);
int					set_norm_map_mat(t_lst *lst, char *word);
int					set_spec_mat(t_lst *lst, char *word);
int					set_refl_mat(t_lst *lst, char *word);

int					cre_fig(t_lst *lst);
int					set_type_fig(t_lst *lst, char *word);
int					set_pos_fig(t_lst *lst, char *word);
int					set_dir_fig(t_lst *lst, char *word);
int					set_rot_fig(t_lst *lst, char *word);
int					set_rad_fig(t_lst *lst, char *word);
int					set_ang_fig(t_lst *lst, char *word);
int					set_mat_fig(t_lst *lst, char *word);

int					cre_lght(t_lst *lst);
int					set_type_lght(t_lst *lst, char *word);
int					set_pos_lght(t_lst *lst, char *word);
int					set_dir_lght(t_lst *lst, char *word);
int					set_ints_lght(t_lst *lst, char *word);
void				move_light(t_lst *lst, SDL_Point p);
void				move_fig(t_lst *lst, SDL_Point p);
void				multy_x(t_lst *lst, int tmp);
void				multy_y(t_lst *lst, int tmp);
void				multy_z(t_lst *lst, int tmp);
void				ret(t_lst *lst);

#endif
