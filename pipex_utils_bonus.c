/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 13:04:55 by guigonza          #+#    #+#             */
/*   Updated: 2024/11/19 13:02:36 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	close_pipes(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->cmd_count - 1)
	{
		close(pipex->pipes[i][0]);
		close(pipex->pipes[i][1]);
		i++;
	}
}

void	free_pipes(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->cmd_count - 1)
		free(pipex->pipes[i++]);
	free(pipex->pipes);
}

void	here_doc(t_pipex *pipex)
{
	char	*line;

	while (1)
	{
		ft_putstr_fd("here_doc> ", 1);
		line = get_next_line(0);
		if (!line || ft_strncmp(line, pipex->delimiter,
				ft_strlen(pipex->delimiter)) == 0)
		{
			free(line);
			break ;
		}
		write(pipex->fd_in, line, ft_strlen(line));
		free(line);
	}
}

void	exec_commands(t_pipex *pipex, char **argv, char **envp, int cmd_index)
{
	char	**cmd_args;
	char	*cmd_path;

	if (cmd_index == 0)
		dup2(pipex->fd_in, STDIN_FILENO);
	else
		dup2(pipex->pipes[cmd_index - 1][0], STDIN_FILENO);
	if (cmd_index == pipex->cmd_count - 1)
		dup2(pipex->fd_out, STDOUT_FILENO);
	else
		dup2(pipex->pipes[cmd_index][1], STDOUT_FILENO);
	close_pipes(pipex);
	cmd_args = ft_split(argv[cmd_index + 2 + pipex->here_doc], ' ');
	if (ft_strchr(cmd_args[0], '/') && access(cmd_args[0], X_OK) == 0)
		cmd_path = ft_strdup(cmd_args[0]);
	else
		cmd_path = get_path(cmd_args[0], envp);
	if (!cmd_path)
		handle_error("Error: Command not found");
	execve(cmd_path, cmd_args, envp);
	handle_error("Error executing command");
}
