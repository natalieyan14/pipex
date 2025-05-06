/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 02:44:23 by natalieyan        #+#    #+#             */
/*   Updated: 2025/05/05 21:25:32 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

char	**parse_cmd(char *cmd)
{
	char	**args;

	args = ft_split(cmd, ' ');
	if (!args || !args[0])
	{
		ft_putendl_fd("Command not found", 2);
		exit(127);
	}
	return (args);
}

void	child_process(char **argv, char **envp, int *pipefd, int infile)
{
	char	**cmd_args;
	char	*cmd_path;

	if (dup2(infile, STDIN_FILENO) < 0 || dup2(pipefd[1], STDOUT_FILENO) < 0)
		error_exit("dup2 (child)");
	close(pipefd[0]);
	cmd_args = parse_cmd(argv[2]);
	cmd_path = get_path(cmd_args[0], envp);
	if (!cmd_path)
	{
		free_split(cmd_args);
		error_exit("Command not found");
	}
	execve(cmd_path, cmd_args, envp);
	free_split(cmd_args);
	free(cmd_path);
	error_exit("execve (child)");
}

void	parent_process(char **argv, char **envp, int *pipefd, int outfile)
{
	char	**cmd_args;
	char	*cmd_path;

	if (dup2(pipefd[0], STDIN_FILENO) < 0 || dup2(outfile, STDOUT_FILENO) < 0)
		error_exit("dup2 (parent)");
	close(pipefd[1]);
	cmd_args = parse_cmd(argv[3]);
	cmd_path = get_path(cmd_args[0], envp);
	if (!cmd_path)
	{
		free_split(cmd_args);
		error_exit("Command not found");
	}
	execve(cmd_path, cmd_args, envp);
	free_split(cmd_args);
	free(cmd_path);
	error_exit("execve (parent)");
}

pid_t	fork_process(void (*func)(char **, char **, int *, int), char **argv,
		char **envp, int *pipefd, int fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		error_exit("Fork error");
	if (pid == 0)
		func(argv, envp, pipefd, fd);
	return (pid);
}

int	main(int argc, char **argv, char **envp)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		infile;
	int		outfile;

	if (argc != 5)
		error_exit("Usage: ./pipex file1 cmd1 cmd2 file2");
	infile = open_infile(argv[1]);
	outfile = open_outfile(argv[4]);
	if (infile < 0 || outfile < 0)
		error_exit("File error");
	if (pipe(pipefd) == -1)
		error_exit("Pipe error");
	pid1 = fork_process(child_process, argv, envp, pipefd, infile);
	pid2 = fork_process(parent_process, argv, envp, pipefd, outfile);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}
