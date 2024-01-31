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

/* The loop is iterating through the `envp` array, the loop continues until it 
either finds an element that contains "PATH" or reaches the end of the 
`envp` array.
The line `every_path = ft_split(envp[i] + 5, ':');` is splitting the string 
`envp[i] + 5` using the delimiter `':'`.
The `while (every_path[++i])` loop is iterating through the `every_path` array,
which contains different directories in the `PATH` environment variable. 
`if (access(path, F_OK | X_OK) == 0)` is checking if the file specified by 
`path` exists and is executable, if so return(path), if the loop ends and if
no existing or executable path return (NULL); */

char	*find_path(char **envp, char *cmd)
{
	int		i;
	char	**every_path;
	char	*path;

	i = 0;
	while (envp[i] != NULL && ft_strnstr(envp[i], "PATH", 4) == NULL)
		i++;
	every_path = ft_split(envp[i] + 5, ':');
	i = -1;
	while (every_path[++i])
	{
		if (cmd[0] == '/')
			path = cmd;
		else
			path = ft_strjoin((ft_strjoin(every_path[i], "/")), cmd);
		free(every_path[i]);
		if (access(path, F_OK | X_OK) == 0)
			return (path);
		free(path);
	}
	return (NULL);
}

/* `dup2(infile, STDIN_FILENO);` duplicates the fd `infile` and assign it 
to the fd `STDIN_FILENO`, which is the fd for standard input. So any input 
read from standard input will now be read from the `infile` file instead. */
/* The `close(end[0]);` is closing the read end of the pipe. This indicates 
that the child process has finished reading from the pipe. */
/* `dup2(end[1], STDOUT_FILENO);` duplicates the fd `end[1]` (which is 
the write end of the pipe) and assign it to the fd `STDOUT_FILENO` 
(which is the file descriptor for standard output). So any output written 
to standard output will now be written to the write end of the pipe 
instead. */
/*`if (execve(path, cmd1, envp) == -1)` is using the `execve` system call 
to execute the command specified by `path` with the arguments `cmd1` and the 
environment variables `envp`. */

void	child_1(int infile, char **argv, char **envp, int *end)
{
	char	**cmd1;
	char	*path;

	dup2(infile, STDIN_FILENO);
	close(end[0]);
	dup2(end[1], STDOUT_FILENO);
	cmd1 = ft_split(argv[2], ' ');
	path = find_path(envp, cmd1[0]);
	if (!path)
		exit_error("Command Error");
	if (execve(path, cmd1, envp) == -1)
		exit_error("Execve Error");
}

/* outfile is the stdout, any output written to stdout will be written to the 
`outfile` file instead.*/
/* The `close(end[1]);` statement is closing the write end of the pipe. This 
indicates that the parent process has finished writing to the pipe. */
/* end[0] is the stdin */
void	child_2(int outfile, char **argv, char **envp, int *end)
{
	char	*path;
	char	**cmd2;

	dup2(outfile, STDOUT_FILENO);
	close(end[1]);
	dup2(end[0], STDIN_FILENO);
	cmd2 = ft_split(argv[3], ' ');
	path = find_path(envp, cmd2[0]);
	if (!path)
		exit_error("Command Error");
	if (execve(path, cmd2, envp) == -1)
		exit_error("Execve Error");
}

/* The `pipe(end);` function is creating a pipe, which is a mechanism for 
interprocess communication. It creates a pair of file descriptors, `end[0]` 
and `end[1]`, where `end[0]` is the file descriptor for reading from the pipe 
and `end[1]` is the file descriptor for writing to the pipe. */
/* The `waitpid` function is used to wait for a specific child process to 
terminate. In this case, `waitpid(pid[0], NULL, 0)` is waiting for child_1 
to terminate, and `waitpid(pid[1], NULL, 0)` is waiting for child_2 to terminate.
The `NULL` argument is used to discard the exit status of the child process, 
and the `0` argument is used to indicate that the function should block until 
the specified child process terminates. */

void	start_process(int infile, int outfile, char **argv, char **envp)
{
	int		end[2];
	pid_t	pid[2];

	if (pipe(end) < 0)
		exit_error("Pipe Error");
	pid[0] = fork();
	if (pid[0] < 0)
		exit_error("Fork Error");
	else if (pid[0] == 0)
		child_1(infile, argv, envp, end);
	pid[1] = fork();
	if (pid[1] < 0)
		exit_error("Fork Error");
	else if (pid[1] == 0)
		child_2(outfile, argv, envp, end);
	close(end[1]);
	close(end[0]);
	waitpid(pid[0], NULL, 0);
	waitpid(pid[1], NULL, 0);
}

/*void a()
{
	system("leaks pipex");
}
//atexit(a);
*/

/* The `open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);` statement is opening
a file specified by `argv[4]` for read and write access. `O_TRUNC` is a flag 
used in the `open` function to truncate the file if it already exists. 
If the file exists and `O_TRUNC` is specified, the file is truncated to zero 
length. If the file does not exist, it is created. */
/* The `close(infile)` and `close(outfile)` statements are closing 
the file descriptors for the input file (`infile`) and output file 
(`outfile`) respectively. This is done to free up system resources
and ensure that the files are properly closed after they are no longer 
needed. */

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
		close(infile);
		close(outfile);
	}
	else
		ft_putstr_fd("Args Error: use ./pipex infile cmd1 cmd2 outfile\n", 1);
	return (0);
}
