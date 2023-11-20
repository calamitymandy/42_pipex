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

void	leaks(void)
{
	system("leaks pipex");
}

char	*find_path(char **envp, char *cmd)
{
	int		i;
	char	**every_path;
	char	*res;

	i = -1;
	while (envp[++i])
	{
		if (envp[i][0] == 'P' && envp[i][1] == 'A' && envp[i][2] == 'T'
			&& envp[i][3] == 'H')
		{
			every_path = ft_split(envp[i] + 5, ':');
			i = -1;
			while (every_path[++i])
			{
				res = ft_strjoin((ft_strjoin(every_path[i], "/")), cmd);
				free(every_path[i]);
				if (access(res, F_OK) == 0)
					return (res);
				free(res);
			}
		}
	}
	return (0); // Si la variable de entorno PATH no está presente en envp
}

void	child_process(int infile, char **cmd1, char **envp, int *end)
{
	char	*path;

	dup2(infile, STDIN_FILENO);
	close(end[0]);
	dup2(end[1], STDOUT_FILENO);
	path = find_path(envp, cmd1[0]);
	if (execve(path, cmd1, envp) == -1)
		exit_error("Execve Error");
	free(cmd1);
}

/* The `waitpid(-1, NULL, 0);` function is used to wait for any child process to terminate. The first
parameter `-1` indicates that the function should wait for any child process. The second parameter
`NULL` is used to discard the exit status of the child process. The third parameter `0` is used to
specify that the function should block until a child process terminates. */
void	parent_process(int outfile, char **cmd2, char **envp, int *end)
{
	char	*path;

	//printf("waitpid :%d\n", waitpid(-1, NULL, 0));
	waitpid(-1, NULL, 0);
	dup2(outfile, STDOUT_FILENO); // outfile is the stdout any output written to stdout will be written to the `outfile` file instead.
	close(end[1]); //closing the write end of the pipe to indicate that `parent_process` has finished writing. This allows the child process to detect the end of the input and exit.
	dup2(end[0], STDIN_FILENO); // end[0] is the stdin
	path = find_path(envp, cmd2[0]);
	if (execve(path, cmd2, envp) == -1)
		exit_error("Execve Error");
	free(cmd2);
}

/* The `pipe(end);` function is creating a pipe, which is a mechanism for interprocess communication.
It creates a pair of file descriptors, `end[0]` and `end[1]`, where `end[0]` is the file descriptor
for reading from the pipe and `end[1]` is the file descriptor for writing to the pipe. */

void	start_process(int infile, int outfile, char **argv, char **envp)
{
	int		end[2];
	pid_t	pid;
	char	*path;
	char	**cmd1;
	char	**cmd2;

	pipe(end);
	pid = fork();
	cmd1 = ft_split(argv[2], ' ');
	cmd2 = ft_split(argv[3], ' ');
		path = find_path(envp, cmd1[0]);
	if (!path)
		exit_error("Path Error");
	if (pid < 0)
		exit_error("Fork Error");
	if (!pid)
	{
		ft_putstr_fd("child process\n", 1);
		child_process(infile, cmd1, envp, end);
	}
	else
	{
		ft_putstr_fd("parent process\n", 1);
		parent_process(outfile, cmd2, envp, end);
	}
	free_two_stars(cmd2);
	free_two_stars(cmd1);
}

int	main(int argc, char **argv, char **envp)
{
	int		infile;
	int		outfile;
	system("leaks pipex");
	if (argc == 5)
	{
		infile = open(argv[1], O_RDONLY);
		outfile = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (infile < 0)
			exit_error("infile");
		if (outfile < 0)
			exit_error("outfile");
		start_process(infile, outfile, argv, envp);
	}
	else
		ft_putstr_fd("Error: Incorrect number of args\n", 1);
	return (0);
}