/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 16:27:51 by amdemuyn          #+#    #+#             */
/*   Updated: 2023/07/04 16:32:01 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"


size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != 0)
		i++;
	return (i);
}

void ft_putstr_fd(const char *str, int fd)
{
	if (!*str)
		return;
	write (fd, str, ft_strlen(str));
}

void	exit_error(char *error)
{
	perror(error);
	exit(EXIT_FAILURE);
}