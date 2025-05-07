/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 02:44:23 by natalieyan        #+#    #+#             */
/*   Updated: 2025/05/07 15:56:21 by natalieyan       ###   ########.fr       */
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

void	child_process(t_exec_info *info)
{
	char	**cmd_args;
	char	*cmd_path;

	if (dup2(info->input_fd, STDIN_FILENO) < 0)
		error_exit("dup2 (child - input)");
	if (dup2(info->pipefd[1], STDOUT_FILENO) < 0)
		error_exit("dup2 (child - pipe)");
	close(info->pipefd[0]);
	cmd_args = parse_cmd(info->argv[2]);
	cmd_path = get_path(cmd_args[0], info->envp);
	if (!cmd_path)
	{
		free_split(cmd_args);
		error_exit("Command not found");
	}
	execve(cmd_path, cmd_args, info->envp);
	free_split(cmd_args);
	free(cmd_path);
	error_exit("execve (child)");
}

void	parent_process(t_exec_info *info)
{
	char	**cmd_args;
	char	*cmd_path;

	if (dup2(info->pipefd[0], STDIN_FILENO) < 0)
		error_exit("dup2 (parent - pipe)");
	if (dup2(info->output_fd, STDOUT_FILENO) < 0)
		error_exit("dup2 (parent - output)");
	close(info->pipefd[1]);
	cmd_args = parse_cmd(info->argv[3]);
	cmd_path = get_path(cmd_args[0], info->envp);
	if (!cmd_path)
	{
		free_split(cmd_args);
		error_exit("Command not found");
	}
	execve(cmd_path, cmd_args, info->envp);
	free_split(cmd_args);
	free(cmd_path);
	error_exit("execve (parent)");
}

pid_t	fork_process(void (*func)(t_exec_info *), t_exec_info *info)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		error_exit("Fork error");
	if (pid == 0)
		func(info);
	return (pid);
}

int	main(int argc, char **argv, char **envp)
{
	t_main_data	data;

	if (argc != 5)
		error_exit("Usage: ./pipex file1 cmd1 cmd2 file2");
	data.infile = open_infile(argv[1]);
	data.outfile = open_outfile(argv[4]);
	if (data.infile < 0 || data.outfile < 0)
		error_exit("File error");
	if (pipe(data.pipefd) == -1)
		error_exit("Pipe error");
	data.child_info = (t_exec_info){argv, envp, data.pipefd, data.infile, -1};
	data.parent_info = (t_exec_info){argv, envp, data.pipefd, -1, data.outfile};
	data.pid1 = fork_process(child_process, &data.child_info);
	data.pid2 = fork_process(parent_process, &data.parent_info);
	close(data.pipefd[0]);
	close(data.pipefd[1]);
	waitpid(data.pid1, NULL, 0);
	waitpid(data.pid2, NULL, 0);
	return (0);
}
