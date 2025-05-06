/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natalieyan <natalieyan@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:28:26 by natalieyan        #+#    #+#             */
/*   Updated: 2025/05/05 21:25:50 by natalieyan       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	free_split(char **arr)
{
	int	i;

	i = 0;
	if (!arr || !*arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

int	open_infile(char *filename)
{
	if (access(filename, F_OK) != 0)
	{
		ft_putendl_fd("No such file or directory", 26);
		return (-1);
	}
	if (access(filename, R_OK) != 0)
	{
		ft_putendl_fd("Permission denied", 18);
		ft_putendl_fd(filename, 2);
		return (-1);
	}
	return (open(filename, O_RDONLY));
}

int	open_outfile(char *filename)
{
	if (access(filename, F_OK) != 0)
		return (open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0664));
	if (access(filename, W_OK | R_OK) != 0)
	{
		ft_putendl_fd("Permission denied", 2);
		ft_putendl_fd(filename, 2);
		return (-1);
	}
	return (open(filename, O_RDONLY | O_WRONLY | O_TRUNC, 0664));
}

char	*get_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	char	*full;
	int		i;

	if (access(cmd, F_OK | X_OK) == 0)
		return (ft_strdup(cmd));
	while (*envp && ft_strncmp(*envp, "PATH=", 5))
		envp++;
	if (!*envp)
		return (NULL);
	paths = ft_split(*envp + 5, ':');
	i = -1;
	while (paths[++i])
	{
		path = ft_strjoin(paths[i], "/");
		full = ft_strjoin(path, cmd);
		free(path);
		if (access(full, F_OK | X_OK) == 0)
			return (free_split(paths), full);
		free(full);
	}
	return (free_split(paths), NULL);
}

void	error_exit(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}
