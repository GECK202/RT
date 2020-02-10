/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabraham <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/08 23:22:12 by vabraham          #+#    #+#             */
/*   Updated: 2020/02/08 23:22:13 by vabraham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

char	*get_inbr_to_string(int min, int num, char *s)
{
	char *c;
	char *str;

	c = malloc(sizeof(char) * 2);
	if (num < 0)
	{
		min *= -1;
		num *= -1;
	}
	*c = num % 10 + 48;
	c[1] = '\0';
	num /= 10;
	str = ft_strjoin(c, s);
	free(c);
	if (num == 0)
	{
		if (min == -1)
			return (ft_strjoin("-", str));
		return (str);
	}
	return (get_inbr_to_string(min, num, str));
}

char	*get_fnbr_to_string(int min, float num, char *s)
{
	float tmp;

	tmp = num - (float)(int)num;
	if (tmp < 0)
		tmp *= -1;
	while (tmp != (float)(int)tmp)
		tmp *= 10;
	return (ft_strjoin(get_inbr_to_string(1, (int)num, "."),
		get_inbr_to_string(1, (tmp), "")));
}

int		get_file_scene(void)
{
	int		fd;
	int		num_scene;
	char	*str;
	char	*str_scene;

	str = "scenes/scene";
	num_scene = 0;
	str_scene = NULL;
	while (1)
	{
		if (str_scene)
			free(str_scene);
		str_scene = ft_strjoin(str, get_inbr_to_string(1, num_scene, ""));
		if ((fd = open(str_scene, O_WRONLY)) == -1)
			break ;
		close(fd);
		num_scene++;
	}
	fd = open(str_scene, O_WRONLY | O_CREAT);
	ft_putstr(str_scene);
	ft_putchar('\n');
	return (fd);
}

char	*get_thre_int(int x, int y, int z)
{
	char *s;
	char *tmp;

	s = "";
	tmp = ft_strjoin(s, get_inbr_to_string(1, x, ""));
	s = tmp;
	tmp = ft_strjoin(s, " ");
	free(s);
	s = tmp;
	tmp = ft_strjoin(s, get_inbr_to_string(1, y, ""));
	free(s);
	s = tmp;
	tmp = ft_strjoin(s, " ");
	free(s);
	s = tmp;
	tmp = ft_strjoin(s, get_inbr_to_string(1, z, ""));
	free(s);
	return (tmp);
}

char	*get_thre_float(float x, float y, float z)
{
	char *s;
	char *tmp;

	s = "";
	tmp = ft_strjoin(s, get_fnbr_to_string(1, x, ""));
	s = tmp;
	tmp = ft_strjoin(s, " ");
	free(s);
	s = tmp;
	tmp = ft_strjoin(s, get_fnbr_to_string(1, y, ""));
	free(s);
	s = tmp;
	tmp = ft_strjoin(s, " ");
	free(s);
	s = tmp;
	tmp = ft_strjoin(s, get_fnbr_to_string(1, z, ""));
	free(s);
	return (tmp);
}
