/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 12:13:15 by guigonza          #+#    #+#             */
/*   Updated: 2024/11/19 13:02:50 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_paths(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
}

char	*get_path(char *cmd, char **envp)
{
	int		i;
	char	*full_path;
	char	**paths;
	char	*tmp;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full_path, X_OK) == 0)
			return (free_paths(paths), full_path);
		free(full_path);
		i++;
	}
	free_paths(paths);
	return (NULL);
}

void	dup_exec(char *cmd, char **cmd_args, char **envp)
{
	execve(cmd, cmd_args, envp);
	handle_error("Error trying to exec cmd");
}

void	exe_cmd(t_pipex *pipex, char **envp, int cmd_num)
{
	if (cmd_num == 1)
	{
		dup2(pipex->fd_in, STDIN_FILENO);
		dup2(pipex->fd_pipe[1], STDOUT_FILENO);
		close(pipex->fd_pipe[0]);
	}
	else
	{
		dup2(pipex->fd_pipe[0], STDIN_FILENO);
		dup2(pipex->fd_out, STDOUT_FILENO);
		close(pipex->fd_pipe[1]);
	}
	close(pipex->fd_in);
	close(pipex->fd_out);
	close(pipex->fd_pipe[0]);
	close(pipex->fd_pipe[1]);
	if (cmd_num == 1)
		dup_exec(pipex->cmd1, pipex->cmd1_args, envp);
	else
		dup_exec(pipex->cmd2, pipex->cmd2_args, envp);
}

void	setup_commands(t_pipex *pipex, char *arg, char **envp, int cmd_num)
{
	char	**cmd_args;
	char	*cmd_path;

	cmd_args = ft_split(arg, ' ');
	if ((access(cmd_args[0], X_OK) == 0) && (ft_strnstr(cmd_args[0], "./", 2)
			|| ft_strchr(cmd_args[0], '/')))
		cmd_path = ft_strdup(cmd_args[0]);
	else if (access(cmd_args[0], X_OK) != 0 && ft_strchr(cmd_args[0], '/'))
		handle_error("Error: command not found");
	else
	{
		cmd_path = get_path(cmd_args[0], envp);
		if (!cmd_path)
			ft_putstr_fd("Error: trying to get the path of cmd", 2);
	}
	if (cmd_num == 0)
	{
		pipex->cmd1 = cmd_path;
		pipex->cmd1_args = cmd_args;
	}
	else
	{
		pipex->cmd2 = cmd_path;
		pipex->cmd2_args = cmd_args;
	}
}
