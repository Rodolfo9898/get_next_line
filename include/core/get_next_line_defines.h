/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_defines.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/08 13:22:24 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/08 13:22:24 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_DEFINES_H
# define GET_NEXT_LINE_DEFINES_H

/*BONUS BUILD: make bonus compiles every file with -D BONUS_MODE=1*/
# ifndef BONUS_MODE
#  define BONUS_MODE 0
# endif

/*BYTES ASKED FROM read() EACH TIME: make BUFFER_SIZE=n overrides it*/
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

#endif
