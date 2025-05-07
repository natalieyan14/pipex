/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 02:43:47 by natalieyan        #+#    #+#             */
/*   Updated: 2025/05/07 15:56:18 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include "../libft/libft.h"
# include <fcntl.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_exec_info
{
	char		**argv;
	char		**envp;
	int			*pipefd;
	int			input_fd;
	int			output_fd;
}				t_exec_info;

typedef struct s_main_data
{
	int			pipefd[2];
	pid_t		pid1;
	pid_t		pid2;
	int			infile;
	int			outfile;
	t_exec_info	child_info;
	t_exec_info	parent_info;
}				t_main_data;

void			free_split(char **arr);
char			*get_path(char *cmd, char **envp);
char			**parse_cmd(char *cmd);
int				open_infile(char *filename);
int				open_outfile(char *filename);
void			error_exit(char *msg);

#endif
