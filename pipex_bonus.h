/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:39:54 by guigonza          #+#    #+#             */
/*   Updated: 2024/11/13 13:35:50 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "../libft/libft.h"
# include "get_next_line.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_pipex
{
	int		fd_in;
	int		fd_out;
	int		**pipes;
	int		cmd_count;
	int		here_doc;
	char	*delimiter;
}			t_pipex;

void	handle_error(const char *msg);
void	setup_fd(int argc, char **argv, t_pipex *pipex);
void	setup_pipes(t_pipex *pipex);
void	close_pipes(t_pipex *pipex);
void	free_pipes(t_pipex *pipex);
void	here_doc(t_pipex *pipex);
void	exec_commands(t_pipex *pipex, char **argv, char **envp, int cmd_index);
char	*get_path(char *cmd, char **envp);
void	free_paths(char **paths);

#endif
