# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    sources.mk                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/11/07 00:00:00 by rperez-t          #+#    #+#              #
#    Updated: 2026/11/07 00:00:00 by rperez-t         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# The same files are compiled for both libraries: make bonus adds
# -D BONUS_MODE=1 (BONUS_FLAGS), see include/core/get_next_line_defines.h.
SRCS = get_next_line.c get_next_line_utils.c get_next_line_bonus.c

OBJS = $(addprefix $(OBJ_DIR)/,$(SRCS:.c=.o))
BONUS_OBJS = $(addprefix $(OBJ_BONUS_DIR)/,$(SRCS:.c=.o))

DEP_FILES = $(OBJS:.o=.d) $(BONUS_OBJS:.o=.d)
