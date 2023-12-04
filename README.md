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

## PATH
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
int	main(int argc, char **argv, char **envp)
{
	int		infile;
	int		outfile;

	if (argc == 5)
	{
		infile = open(argv[1], O_RDONLY); // read infile
		outfile = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644); // create outfile with read & write access, & truncate the file if it already exists. 
		if (infile < 0)
			print error;
                        exit;
		if (outfile < 0)
			print error;
                        exit;
		start_process(infile, outfile, argv, envp); // to start parent process
	}
	else
		print error; //error nb of args
	return (0);
}
```
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
