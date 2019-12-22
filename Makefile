# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/09/23 17:12:18 by vkaron            #+#    #+#              #
#    Updated: 2019/12/22 18:49:04 by vkaron           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all, clean, fclean, re, libr

FLAGS = -Wall -Wextra -Werror -g

C_FILES =	key_press.c move_multy.c main.c light.c events.c scene.c trace.c\
			matrix.c init.c clear.c	read_fig1.c read_fig2.c read_light.c\
			read_scn.c tools.c intersection.c vec1.c vec2.c

O_FILES = $(C_FILES:.c=.o)

H_DIR = -Ilibft -Iusr/L -Iinclude

LIBS = -Llibft -lft -lmlx -framework OpenGL -framework AppKit -framework OpenCL

S_DIR = src

OBJ = $(addprefix $(S_DIR)/, $(O_FILES))

NAME = RT

all: libr $(NAME)

libr:
	make -C libft/ all

$(NAME): $(OBJ)
	gcc $(FLAGS) -o $@ $^ $(H_DIR) $(LIBS)

$(S_DIR)/%.o: $(S_DIR)/%.c include/rtv1.h
	gcc $(FLAGS) -c $< -o $@ $(H_DIR)

clean:
	make -C libft/ clean
	rm -f $(OBJ)

fclean : clean
	make -C libft/ fclean
	rm -f $(NAME)

re: fclean all
