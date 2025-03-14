/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 11:34:54 by guigonza          #+#    #+#             */
/*   Updated: 2024/11/19 13:01:29 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	handle_error(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void	setup_fd(int argc, char **argv, t_pipex *pipex)
{
	if (argc != 5)
	{
		ft_putstr_fd("ERROR! Usage: file1 cmd1 cmd2 file2\n", 2);
		exit(EXIT_FAILURE);
	}
	pipex->fd_in = open(argv[1], O_RDONLY);
	if (pipex->fd_in < 0)
		handle_error("Error trying to open file1");
	pipex->fd_out = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->fd_out < 0)
		handle_error("Error trying to open file2");
}

void	commands(char **argv, char **envp, t_pipex *pipex)
{
	if (argv[2][0] == '\0')
	{
		ft_putstr_fd("Error: Command 1 is empty\n", 2);
		exit(EXIT_FAILURE);
	}
	if (argv[3][0] == '\0')
	{
		ft_putstr_fd("Error: Command 2 is empty\n", 2);
		exit(EXIT_FAILURE);
	}
	setup_commands(pipex, argv[2], envp, 0);
	setup_commands(pipex, argv[3], envp, 1);
}

void	create_child_proccess(t_pipex *pipex, char **envp)
{
	pipex->pid1 = fork();
	if (pipex->pid1 < 0)
		handle_error("Error forking first child");
	if (pipex->pid1 == 0)
		exe_cmd(pipex, envp, 1);
	pipex->pid2 = fork();
	if (pipex->pid2 < 0)
		handle_error("Error forking second child");
	if (pipex->pid2 == 0)
		exe_cmd(pipex, envp, 2);
	close(pipex->fd_pipe[0]);
	close(pipex->fd_pipe[1]);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	setup_fd(argc, argv, &pipex);
	if (pipe(pipex.fd_pipe) < 0)
	{
		handle_error("Error trying to create the pipe");
		free_paths(pipex.cmd1_args);
	}
	commands(argv, envp, &pipex);
	create_child_proccess(&pipex, envp);
	close(pipex.fd_in);
	close(pipex.fd_out);
	waitpid(pipex.pid1, NULL, 0);
	waitpid(pipex.pid2, NULL, 0);
	free_paths(pipex.cmd1_args);
	free_paths(pipex.cmd2_args);
	free(pipex.cmd1);
	free(pipex.cmd2);
	return (EXIT_SUCCESS);
}
