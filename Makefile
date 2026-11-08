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

# The headers each object uses come from its .d file (DEPFLAGS).
$(OBJS) $(BONUS_OBJS): $(STAMP)

$(STAMP):
	@$(RM) .buffer_size_*
	@touch $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(call run,$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@)

$(OBJ_BONUS_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(call run,$(CC) $(CFLAGS) $(BONUS_FLAGS) $(DEPFLAGS) -c $< -o $@)

clean:
	@$(call remove,$(OBJ_DIR),Removing the objects of get_next_line,objects of get_next_line)
	@$(call remove,$(OBJ_BONUS_DIR),Removing the bonus objects of get_next_line,bonus objects of get_next_line)

fclean: clean
	@$(call remove,$(NAME),Removing the library $(NAME))
	@$(call remove,$(BONUS_NAME),Removing the library $(BONUS_NAME))
	@$(call remove,.buffer_size_*,Removing the stamp of the last BUFFER_SIZE,stamp of the last BUFFER_SIZE)

re: fclean all

.PHONY: all bonus clean fclean re

-include $(DEP_FILES)
