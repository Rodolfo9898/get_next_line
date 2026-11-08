/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 09:32:03 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/03 22:58:06 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include "core/get_next_line_structs.h"

/*FROM get_next_line.c*/
char	*get_next_line(int fd);

/*FROM get_next_line_utils.c*/
void	gnl_stash_clear(t_stash *stash);
char	*gnl_extract_line(t_stash *stash, size_t line_len);
ssize_t	gnl_read_until_newline(int fd, t_stash *stash, size_t read_size);

/*
** BONUS ENTRY POINT: declared in both builds because get_next_line calls it
** under 'if (BONUS_MODE)', but only defined with -D BONUS_MODE=1.
*/
/*FROM get_next_line_bonus.c*/
char	*gnl_bonus_get_next_line(int fd, t_stash **list);

#endif
