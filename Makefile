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
include mk/config.mk
include mk/sources.mk

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
