/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 17:41:21 by amdemuyn          #+#    #+#             */
/*   Updated: 2023/07/04 17:46:00 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>

int		main(int argc, char **argv, char **envp);
char	*find_path(char **envp);

/*SPLIT*/
char	**ft_split(char const *s, char c);

/*UTILS*/
size_t	ft_strlen(const char *s);
void    ft_putstr_fd(const char *str, int fd);
void	exit_error(char *error);

#endif
