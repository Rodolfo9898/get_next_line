# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    colors.mk                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/11/06 23:00:00 by rperez-t          #+#    #+#              #
#    Updated: 2026/11/06 23:00:00 by rperez-t         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# The recipes run in bash: 'set -o pipefail' keeps the exit status of a command
# whose output goes through COLORIZE (make 3.81, the one of macOS, has no
# .SHELLFLAGS, so run sets it itself).
SHELL := /bin/bash

RED		= \033[0;31m
GREEN	= \033[0;32m
YELLOW	= \033[0;33m
BLUE	= \033[0;34m
RESET	= \033[0m

# What a compiler, a linker or a library's Makefile prints: errors in red,
# warnings in yellow, compile and archive commands in green, the rest as it is.
# It matches the diagnostics (': error:'), not the word: error.c stays green.
COLORIZE = awk '\
	/^(fatal )?error:|: (fatal )?error:|undefined reference|Undefined symbols|\*\*\* .*Error [0-9]/ \
		{ printf "$(RED)%s$(RESET)\n", $$0; next } \
	/^warning:|: warning:/ { printf "$(YELLOW)%s$(RESET)\n", $$0; next } \
	/^(cc|gcc|clang|ar) / { printf "$(GREEN)%s$(RESET)\n", $$0; next } \
	{ print }'

# $(call explain,text): what is going to happen, in blue (no comma in the text).
explain = printf '$(BLUE)%s$(RESET)\n' '$(1)'

# $(call warn,text): a warning, in yellow (no comma in the text).
warn = printf '$(YELLOW)warning: %s$(RESET)\n' '$(1)'

# $(call run,command): shows the command in green, then runs it. What it prints
# goes through COLORIZE, and the rule fails when the command fails.
run = set -o pipefail; printf '$(GREEN)%s$(RESET)\n' '$(1)'; \
	$(1) 2>&1 | $(COLORIZE)

# $(call exists,paths): true when at least one of the paths (or patterns) exists.
exists = [ -n "$$(ls -d $(1) 2>/dev/null)" ]

# $(call remove,paths,why[,name]): when one of the paths exists, says why in
# blue, then shows the rm in green and runs it. Otherwise, a warning: nothing
# to remove (named after name when given: handy for a long list of objects).
remove = if $(call exists,$(1)); then \
		$(call explain,$(2)); $(call run,rm -rf $(1)); \
	else \
		$(call warn,$(if $(3),no $(3) to remove,nothing to remove: $(1) does not exist)); \
	fi
