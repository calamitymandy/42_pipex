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

void	ft_putstr_fd(const char *str, int fd)
{
	if (!*str)
		return ;
	write (fd, str, ft_strlen(str));
}

void	exit_error(char *error)
{
	perror(error);
	exit(EXIT_FAILURE);
}

void	free_two_stars(char **two_stars)
{
	int	i;

	i = 0;
	while (two_stars[i])
	{
		free(two_stars[i]);
		i++;
	}
	free(two_stars);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	char	*dst;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	dst = (char *)malloc(sizeof(*s1) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!dst)
		return (NULL);
	while (s1[i] != '\0')
	{
		dst[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		dst[i + j] = s2[j];
		j++;
	}
	dst[i + j] = '\0';
	return (dst);
}
