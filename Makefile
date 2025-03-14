# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/28 11:07:24 by guigonza          #+#    #+#              #
#    Updated: 2024/11/19 12:43:15 by guigonza         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	pipex

NAME_BONUS	=	pipex_bonus

CC			=	cc

CFLAGS		=	-Wall -Werror -Wextra 
LIBFT_DIR	=	./libft

LIBFT		=	$(LIBFT_DIR)/libft.a

SRC			=	pipex.c pipex_utils.c

SRC_BONUS	=	pipex_bonus.c pipex_utils_bonus.c pipex_path_bonus.c\
				./libft/get_next_line.c ./libft/get_next_line_utils.c

OBJ			=	$(SRC:.c=.o)

OBJ_BONUS	=	$(SRC_BONUS:.c=.o)

INCLUDES	=	-I $(LIBFT_DIR)

all: $(LIBFT) $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(OBJ_BONUS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ_BONUS) $(LIBFT) -o $(NAME_BONUS)

clean:
	@rm -f $(OBJ)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME) $(OBJ_BONUS) $(NAME_BONUS)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re bonus
