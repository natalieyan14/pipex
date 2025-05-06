/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 02:43:47 by natalieyan        #+#    #+#             */
/*   Updated: 2025/05/05 20:11:03 by natalieyan       ###   ########.fr       */
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

void	free_split(char **arr);
char	*get_path(char *cmd, char **envp);
char	**parse_cmd(char *cmd);
int		open_infile(char *filename);
int		open_outfile(char *filename);
void	error_exit(char *msg);

#endif
