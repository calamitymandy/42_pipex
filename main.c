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

/* The `waitpid(pid, NULL, 0);` function is used to wait for a specific child process to terminate. In
this case, it is waiting for the child process with the process ID `pid` to terminate. The `NULL`
parameter is used to discard the exit status of the child process, and the `0` parameter is used to
specify that the function should block until the child process terminates. */
void	start_process(int infile, int outfile, char **argv, char **envp)
{
	int end[2];
	pid_t	pid;
	char	**path;
	char	**cmd1;
	char	**cmd2;
	(void)infile;
	(void)outfile;

	path = ft_split(find_path(envp), ':');
	if (!path)
		exit_error("Path Error");
		//printf("%s\n", *path);
	pipe(end);
	pid = fork();
	cmd1 = ft_split(argv[2], ' ');
	cmd2 = ft_split(argv[3], ' ');
	printf("pid :%d\n", pid);

	if (pid < 0)
		exit_error("Fork Error");
	if (!pid)
		ft_putstr_fd("child process\n", 1);
		//child_process(infile, cmd1, envp, end);
	else
		ft_putstr_fd("parent process\n", 1);
		//parent_process(outfile, cmd2, envp, end);
	waitpid(pid, NULL, 0);
	waitpid(pid, NULL, 0);
}

int	main(int argc, char **argv, char **envp)
{
	int		infile;
	int		outfile;

	if (argc == 5)
	{
		infile = open(argv[1], O_RDONLY);
		outfile = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (infile < 0)
			exit_error("infile");
		if (outfile < 0)
			exit_error("outfile");
		start_process(infile, outfile, argv, envp);
		//printf("%s\n", *envp);
	}
	else
		ft_putstr_fd("Error: Incorrect number of args\n", 1);
	return (0);
}
