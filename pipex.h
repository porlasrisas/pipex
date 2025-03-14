/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 11:30:57 by guigonza          #+#    #+#             */
/*   Updated: 2024/11/06 13:11:49 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_pipex
{
	int		fd_in;
	int		fd_out;
	char	*cmd1;
	char	*cmd2;
	char	**cmd1_args;
	char	**cmd2_args;
	pid_t	pid1;
	pid_t	pid2;
	char	**envp;
	int		fd_pipe[2];
}			t_pipex;

void		free_paths(char **paths);
char		*get_path(char *cmd, char **envp);
void		setup_fd(int argc, char **argv, t_pipex *pipex);
void		commands(char **argv, char **envp, t_pipex *pipex);
void		handle_error(const char *msg);
void		create_child_proccess(t_pipex *pipex, char **envp);
void		dup_exec(char *cmd, char **cmd_args, char **envp);
void		setup_commands(t_pipex *pipex, char *arg, char **envp, int cmd_num);
void		exe_cmd(t_pipex *pipex, char **envp, int cmd_num);

#endif