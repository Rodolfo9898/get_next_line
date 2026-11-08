# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    config.mk                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/11/07 00:00:00 by rperez-t          #+#    #+#              #
#    Updated: 2026/11/07 00:00:00 by rperez-t         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = get_next_line.a
BONUS_NAME = get_next_line_bonus.a

CC = cc
CFLAGS = -Wall -Wextra -Werror
BONUS_FLAGS = -D BONUS_MODE=1
DEPFLAGS = -MMD -MP
AR = ar rcs

SRC_DIR = ./sources
OBJ_DIR = ./objects
OBJ_BONUS_DIR = ./objects_bonus

# make BUFFER_SIZE=42 overrides the default of include/core/defines.h.
# The stamp file name holds the value, so changing it rebuilds the objects.
ifdef BUFFER_SIZE
CFLAGS += -D BUFFER_SIZE=$(BUFFER_SIZE)
endif
STAMP = .buffer_size_$(or $(BUFFER_SIZE),default)
