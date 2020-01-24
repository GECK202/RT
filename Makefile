# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vkaron <vkaron@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/09/23 17:12:18 by vkaron            #+#    #+#              #
#    Updated: 2020/01/11 22:54:27 by vkaron           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all, clean, fclean, re, libr

#FLAGS = -Wall -Wextra -Werror
FLAGS = -g -w

C_FILES =	key_press.c move_multy.c main.c light.c events.c scene.c trace.c\
			matrix.c init.c clear.c	read_fig1.c read_fig2.c read_light.c\
			read_scn.c tools.c intersection.c vec1.c vec2.c read_material1.c\
			read_material2.c

O_FILES =	$(C_FILES:.c=.o)

H_DIR =		-Ilibft -Iusr/L -Iinclude

LIBS =		-std=c99 -Llibft -lft -lSDL2 -lSDL2_image -lm -lpthread
# -lSDL2_ttf
#-dynamiclib -o lib/libSDL2.dylib -o lib/libSDL2_image.dylib

FRAME =		-F ./Frameworks/ -framework SDL2 -framework SDL2_image\
			-framework SDL2_ttf 

#LIBS = -Llibft -lft 
#-Llib 
#-lSDL2 -lSDL2_image -lSDL2_ttf
#-dynamiclib -o libSDL2.dylib -o libSDL2_image.dylib

S_DIR = src

OBJ = $(addprefix $(S_DIR)/, $(O_FILES))

NAME = RT

all: libr $(NAME)

libr:
	make -C libft/ all

$(NAME): $(OBJ)
	gcc -v $(FLAGS) -o $@ $^ $(H_DIR) $(LIBS)
#$(FRAME)

$(S_DIR)/%.o: $(S_DIR)/%.c include/rt.h
	gcc $(FLAGS) -c $< -o $@ $(H_DIR)

clean:
	make -C libft/ clean
	rm -f $(OBJ)

fclean : clean
	make -C libft/ fclean
	rm -f $(NAME)

re: fclean all
