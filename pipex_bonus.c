/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:45:24 by guigonza          #+#    #+#             */
/*   Updated: 2024/11/13 13:35:08 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	handle_error(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void	setup_fd(int argc, char **argv, t_pipex *pipex)
{
	if (argc < 5)
		handle_error("Error! Usage: file1 cmd1 ... cmdN file2");
	if (pipex->here_doc)
	{
		pipex->fd_in = open("here_doc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (pipex->fd_in < 0)
			handle_error("Error opening here_doc_tmp");
		here_doc(pipex);
		close(pipex->fd_in);
		pipex->fd_in = open("here_doc_tmp", O_RDONLY);
		pipex->fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND,
				0644);
	}
	else
	{
		pipex->fd_in = open(argv[1], O_RDONLY);
		pipex->fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC,
				0644);
	}
	if (pipex->fd_in < 0 || pipex->fd_out < 0)
		handle_error("Error opening input/output files");
}

void	setup_pipes(t_pipex *pipex)
{
	int	i;

	pipex->pipes = malloc(sizeof(int *) * (pipex->cmd_count - 1));
	if (!pipex->pipes)
		handle_error("Error allocating memory for pipes");
	i = 0;
	while (i < pipex->cmd_count - 1)
	{
		pipex->pipes[i] = malloc(sizeof(int) * 2);
		if (!pipex->pipes[i] || pipe(pipex->pipes[i]) == -1)
		{
			while (--i >= 0)
				free(pipex->pipes[i]);
			free(pipex->pipes);
			handle_error("Error creating pipes");
		}
		i++;
	}
}

void	create_process(t_pipex *pipex, char **argv, char **envp)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (i < pipex->cmd_count)
	{
		pid = fork();
		if (pid == -1)
			handle_error("Error forking process");
		if (pid == 0)
			exec_commands(pipex, argv, envp, i);
		i++;
	}
	close_pipes(pipex);
	while (i-- > 0)
		wait(NULL);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	pipex.here_doc = (ft_strncmp(argv[1], "here_doc", 8) == 0);
	pipex.cmd_count = argc - 3 - pipex.here_doc;
	if (pipex.here_doc)
		pipex.delimiter = argv[2];
	setup_fd(argc, argv, &pipex);
	setup_pipes(&pipex);
	create_process(&pipex, argv, envp);
	close(pipex.fd_in);
	close(pipex.fd_out);
	free_pipes(&pipex);
	if (pipex.here_doc)
		unlink("here_doc_tmp");
	return (EXIT_SUCCESS);
}
