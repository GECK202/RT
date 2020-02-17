/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/08 23:22:12 by vabraham          #+#    #+#             */
/*   Updated: 2020/02/18 00:19:22 by vkaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

char	*get_inbr_to_string(int min, int num, char *s)
{
	//////////here go kostyl
	min = 1;
	/////////kostl over
	char *b = ft_itoa(num);
	char *c = ft_strjoin(b, s);
	printf("%s     %s\n", b, c);
	free(b);
	return(c);	
}

char	*get_fnbr_to_string(float num)
{
	float tmp;
	char *result;
	char *buf1;
	char *buf2;

	tmp = num - (float)(int)num;
	if (tmp < 0)
		tmp *= -1;
	while (tmp != (float)(int)tmp)
		tmp *= 10;
	buf1 = get_inbr_to_string(1, (int)num, ".");
	buf2 = get_inbr_to_string(1, (tmp), "");
	if (!(result = ft_strjoin(buf1, buf2)))
		ft_exit("STRJOIN FAILED IN FNBR");
	free(buf1);
	free(buf2);
	return (result);
}

int		get_file_scene(void)
{
	int		fd;
	int		num_scene;
	char	*str;
	char	*str_scene;
	char	*buf;

	str = "scenes/scene";
	num_scene = 0;
	str_scene = NULL;
	while (1)
	{
		if (str_scene)
			free(str_scene);
		buf = get_inbr_to_string(1, num_scene, "");
		str_scene = ft_strjoin(str, buf);
		free(buf);
		if ((fd = open(str_scene, O_WRONLY)) == -1)
			break ;
		close(fd);
		num_scene++;
	}
	fd = open(str_scene, O_WRONLY | O_CREAT);
	ft_putstr(str_scene);
	free(str_scene);
	ft_putchar('\n');
	return (fd);
}

char	*get_thre_int(int x, int y, int z)
{
	char *s;
	char *tmp;
	char *buf;

	//s = "";
	tmp = get_inbr_to_string(1, x, "");
	//tmp = ft_strjoin(s, buf);
	//free(buf);
	s = tmp;
	tmp = ft_strjoin(s, " ");
	free(s);
	s = tmp;
	buf = get_inbr_to_string(1, y, "");
	tmp = ft_strjoin(s, buf);
	free(buf);
	free(s);
	s = tmp;
	tmp = ft_strjoin(s, " ");
	free(s);
	s = tmp;
	buf = get_inbr_to_string(1, z, "");
	tmp = ft_strjoin(s, buf);
	free(buf);
	free(s);
	return (tmp);
}

char	*get_thre_float(float x, float y, float z)
{
	char *s;
	char *tmp;
	char *buf;

	//s = "";
	//tmp = ft_strjoin(s, get_fnbr_to_string(x));
	tmp = get_fnbr_to_string(x);
	s = tmp;
	tmp = ft_strjoin(s, " ");
	free(s);
	s = tmp;
	buf = get_fnbr_to_string(y);
	tmp = ft_strjoin(s, buf);
	free(buf);
	free(s);
	s = tmp;
	tmp = ft_strjoin(s, " ");
	free(s);
	s = tmp;
	buf = get_fnbr_to_string(z);
	tmp = ft_strjoin(s, buf);
	free(buf);
	free(s);
	return (tmp);
}
