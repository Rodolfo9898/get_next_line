# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/11/03 14:34:52 by rperez-t          #+#    #+#              #
#    Updated: 2026/11/03 22:58:06 by rperez-t         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= get_next_line.a
BONUS_NAME	= get_next_line_bonus.a

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
AR			= ar rcs
RM			= rm -f

# make BUFFER_SIZE=42 overrides the default from the header.
# The stamp file name holds the value, so changing it rebuilds the objects.
ifdef BUFFER_SIZE
CFLAGS		+= -D BUFFER_SIZE=$(BUFFER_SIZE)
endif
STAMP		= .buffer_size_$(or $(BUFFER_SIZE),default)

SRCS		= get_next_line.c get_next_line_utils.c
BONUS_SRCS	= get_next_line_bonus.c get_next_line_utils_bonus.c

OBJS		= $(SRCS:.c=.o)
BONUS_OBJS	= $(BONUS_SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJS)
	$(AR) $(BONUS_NAME) $(BONUS_OBJS)

$(OBJS): get_next_line.h $(STAMP)
$(BONUS_OBJS): get_next_line_bonus.h $(STAMP)

$(STAMP):
	@$(RM) .buffer_size_*
	@touch $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(BONUS_OBJS)

fclean: clean
	$(RM) $(NAME) $(BONUS_NAME) .buffer_size_*

re: fclean all

.PHONY: all bonus clean fclean re
