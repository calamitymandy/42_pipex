# 42_pipex

This project is about handling pipes<br>

Pipex reproduces the behaviour of the shell pipe | command in c it launches as
`.pipex infile cmd1 cmd2 outfile` and behaves as this line does in the shell `<infile cmd1 | cmd2 > outfile`.

## Tutos
### https://csnotes.medium.com/pipex-tutorial-42-project-4469f5dd5901
### https://web.archive.org/web/20220508111904/https://www.codequoi.com/en/pipex-reproducing-the-pipe-operator-in-c/#understanding_the_pipe_operator_for_pipex

## Usage
To compile, use ```make``` or  ```make all```.

Pipex can handle 2. To execute:

```
$ ./pipex input_file cmd1 cmd2 output_file
```
And it replicates the shell command:
```
$ < input_file cmd1 | cmd2 > output_file
```

Each command can be given arguments and options if surrounded by quotation marks. For example:
```
$ ./pipex inputfile.txt "cat" "grep x" outputfile.txt
```

## MAIN

The general idea: we read from infile, execute cmd1 with infile as input, send the output to cmd2, which will write to outfile.


```
int	main(int argc, char **argv, char **envp)
{
	int		infile;
	int		outfile;

	if (argc == 5)
	{
		infile = open(argv[1], O_RDONLY); // read infile
		outfile = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644); // create outfile with read & write access, & truncate the file if it already exists. 
		if (infile < 0)
			print error & exit;
		if (outfile < 0)
			print error & exit;
		start_process(infile, outfile, argv, envp); // to start parent process
	}
	else
		print error; //error nb of args
	return (0);
}
```
## PARENT & 2 CHILDREN PROCESS

### PARENT

pipe() is creating a pipe, which is a mechanism for interprocess communication. 
It creates a pair of file descriptors, `end[0]` and `end[1]`, where `end[0]` is the fd 
for reading from the pipe and `end[1]` is the fd for writing to the pipe.

fork() is a system call in Unix-like operating systems that creates a new process by
duplicating the existing process. The new process, called the child process, is an exact
copy of the existing process, called the parent process, except for a few values that are
different. After the `fork()` call, both the parent and child processes continue executing
from the point immediately after the `fork()` call, but they have different process IDs.
The child process receives a process ID (pid) of 0, while the parent process receives the
pid of the child process.

The `waitpid` function is used to wait for a specific child process to 
terminate. In this case, `waitpid(pid[0], NULL, 0)` is waiting for child_1 
to terminate, and `waitpid(pid[1], NULL, 0)` is waiting for child_2 to terminate.

```
void	start_process(int infile, int outfile, char **argv, char **envp)
{
	int		end[2];
	pid_t	pid[2];

	if (pipe(end) < 0)
		print error & exit;
	pid[0] = fork();
	if (pid[0] < 0)
		print error & exit;
	else if (pid[0] == 0)
		child_1(infile, argv, envp, end);
	pid[1] = fork();
	if (pid[1] < 0)
        print error & exit;
	else if (pid[1] == 0)
		child_2(outfile, argv, envp, end);
	close(end[1]);
	close(end[0]);
	waitpid(pid[0], NULL, 0);
	waitpid(pid[1], NULL, 0);
}
```
### 2 CHILDREN PROCESS

dup2() swaps our files with stdin and stdout. It duplicates the fd from first param 
and assign it to the fd of 2nd param, which is the fd for standard input or standard output. 
So any input read from standard input will now be read from the 1st param instead.
And any output written in standard output will now be written into the 2nd param instead.

Child_1 reads from the infile & write the result of the execution of 1st command (argv[2]) 
into the pipe:

```
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
```
Child_2 reads from the the pipe the result of the execution of 1st command, then execute the 
2nd command (argv[3]) and write the result in the outfile:

```
same as child_1 except with 2nd command and:

	dup2(outfile, STDOUT_FILENO);
	close(end[1]);
	dup2(end[0], STDIN_FILENO);
```

## PATH
// parsing (somewhere in your code)
char *PATH_from_envp;  
char **mypaths;  
char **mycmdargs;

// retrieve the line PATH from envp  
PATH_from_envp = ft_substr(envp ....);  
mypaths = ft_split(PATH_from_envp, ":"); 
mycmdargs = ft_split(ag[2], " ");
// in your child or parent processint  i;  
char *cmd;

i = -1;  
while (mypaths[++i])  
{  
    cmd = ft_join(mypaths[i], ag[2]); // protect your ft_join  
    execve(cmd, mycmdargs, envp); // if execve succeeds, it exits  
    // perror("Error"); <- add perror to debug  
    free(cmd) // if execve fails, we free and we try a new path  
}  
return (EXIT_FAILURE);

```
char	*find_path(char **envp, char *cmd)
{
	int		i;
	char	**every_path;
	char	*path;

	i = 0;
	while (envp[i] != NULL && ft_strnstr(envp[i], "PATH", 4) == NULL)
		i++; // retrieve the line PATH from `envp` array, the loop continues until it 
				either finds "PATH" or reaches the end of the `envp` array.
	every_path = ft_split(envp[i] + 5, ':'); // split all paths
	i = -1;
	while (every_path[++i])
	{
		path = ft_strjoin((ft_strjoin(every_path[i], "/")), cmd); // join path with "/" and cmd
		free(every_path[i]); //free all paths
		if (access(path, F_OK | X_OK) == 0)
			return (path); // if the path + cmd exists and is executable return it
		free(path);
	}
	return (NULL);
}
```

```
int execve(const char *path, char *const argv[], char *envp[]);
* path: the path to our command  
        type `which ls` and `which wc` in your terminal  
        you'll see the exact path to the commands' binaries
* argv[]: the args the command needs, for ex. `ls -la`  
          you can use your ft_split to obtain a char **  
          like this { "ls", "-la", NULL }  
          it must be null terminated
* envp: the environmental variable  
        you can simply retrieve it in your main (see below)  
        and pass it onto execve, no need to do anything here  
        in envp you'll see a line PATH which contains all possible  
        paths to the commands' binaries
```
To see what is inside `envp`, type `env` in your terminal.  
There is a line `PATH` , which contains all possible paths to the command binaries.  
split using `:` as a delimiter, and retrieve them. Add a `/` at the end for the path to work correctly.
The execve function will try every possible path with the cmd to find the correct one. 
If the command does not exist, execve will do nothing and return -1; else, it will execute the cmd, delete all ongoing processes and exit. 

```
