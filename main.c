/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 16:16:21 by amdemuyn          #+#    #+#             */
/*   Updated: 2023/07/04 18:01:34 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**ft_split(char const *s, char c);
/**
 * The function "find_path" searches through an array of strings (envp) to find 
 * the string that starts with "PATH" and returns a pointer to the characters 
 * after the fifth character of that string (after "PATH=").
 * @param envp The parameter `envp` is a pointer to an array of strings. Each 
 * string represents an environment variable in the form "NAME=VALUE". In this 
 * case, the function is searching for the environment variable "PATH" and 
 * returning a pointer to the value part of the variable.
 * @return a pointer to the string that represents the value of the "PATH" 
 * environment variable.
 */

char	*find_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (envp[i][0] == 'P' && envp[i][1] == 'A' && envp[i][2] == 'T'
			&& envp[i][3] == 'H')
			return (&envp[i][5]);
		i++;
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	char	**path;
	int		infile;
	int		outfile;
	char	**cmd1;
	char	**cmd2;

	if (argc == 5)
	{
		infile = open(argv[1], O_RDONLY);
		outfile = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (infile < 0)
		{
			perror("infile");
			exit(EXIT_FAILURE);
		}
		if (outfile < 0)
		{
			perror("outfile");
			exit(EXIT_FAILURE);
		}
		path = ft_split(find_path(envp), ':');
		if (!path)
		{
			ft_putstr_fd("Error: path not found\n", 1);
			exit(EXIT_FAILURE);
		}
		cmd1 = ft_split(argv[2], ' ');
		cmd2 = ft_split(argv[3], ' ');
		printf("%s\n", *envp);
	}
	else
		ft_putstr_fd("Error: Incorrect number of args\n", 1);
	return (0);
}
