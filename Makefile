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

include mk/colors.mk

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
	@$(call run,$(AR) $(NAME) $(OBJS))

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJS)
	@$(call run,$(AR) $(BONUS_NAME) $(BONUS_OBJS))

$(OBJS): get_next_line.h $(STAMP)
$(BONUS_OBJS): get_next_line_bonus.h $(STAMP)

$(STAMP):
	@$(RM) .buffer_size_*
	@touch $@

%.o: %.c
	@$(call run,$(CC) $(CFLAGS) -c $< -o $@)

clean:
	@$(call remove,$(OBJS),Removing the objects of get_next_line,objects of get_next_line)
	@$(call remove,$(BONUS_OBJS),Removing the bonus objects of get_next_line,bonus objects of get_next_line)

fclean: clean
	@$(call remove,$(NAME),Removing the library $(NAME))
	@$(call remove,$(BONUS_NAME),Removing the library $(BONUS_NAME))
	@$(call remove,.buffer_size_*,Removing the stamp of the last BUFFER_SIZE,stamp of the last BUFFER_SIZE)

re: fclean all

.PHONY: all bonus clean fclean re
