/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/08 13:22:24 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/08 13:22:24 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "libs.h"
# include "defines.h"

/*
** Bytes read but not returned yet: the len bytes starting at data + start.
** cap is the size of the data allocation. fd is the descriptor those
** bytes were read from. next links the stashes of the other fds in the
** bonus (one per fd); the mandatory part keeps a single stash.
*/
typedef struct s_stash
{
	char			*data;
	size_t			start;
	size_t			len;
	size_t			cap;
	int				fd;
	struct s_stash	*next;
}	t_stash;

#endif
