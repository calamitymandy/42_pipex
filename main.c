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
/* The `waitpid(-1, NULL, 0);` function is used to wait for any child process to terminate. The first
parameter `-1` indicates that the function should wait for any child process. The second parameter
`NULL` is used to discard the exit status of the child process. The third parameter `0` is used to
specify that the function should block until a child process terminates. */
void	parent_process(int outfile, char **cmd2, char **envp, int *end)
{
	(void)cmd2;
	(void)envp;
	printf("waitpid :%d\n", waitpid(-1, NULL, 0));
	waitpid(-1, NULL, 0);
	dup2(outfile, STDOUT_FILENO); // outfile is the stdout any output written to stdout will be written to the `outfile` file instead.
	close(end[1]); //closing the write end of the pipe to indicate that `parent_process` has finished writing. This allows the child process to detect the end of the input and exit.
	dup2(end[0], STDIN_FILENO); // end[0] is the stdin
	close(end[0]); //closing the read end of the pipe to indicate that `parent_process` has finished reading. This allows the child process to detect the end of the output and exit.
	close(outfile); //close fd for the `outfile` file o free up system resources and indicate that the file is no longer needed.
}

/* The `pipe(end);` function is creating a pipe, which is a mechanism for interprocess communication.
It creates a pair of file descriptors, `end[0]` and `end[1]`, where `end[0]` is the file descriptor
for reading from the pipe and `end[1]` is the file descriptor for writing to the pipe. */

void	start_process(int infile, int outfile, char **argv, char **envp)
{
	int end[2];
	pid_t	pid;
	char	**path;
	char	**cmd1;
	char	**cmd2;
	(void)infile;

	path = ft_split(find_path(envp), ':');
	if (!path)
		exit_error("Path Error");
		printf("%s\n", *path);
	pipe(end);
	//printf("end[0] :%d\n", end[0]);
	//printf("end[1] :%d\n", end[1]);
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
		parent_process(outfile, cmd2, envp, end);
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
