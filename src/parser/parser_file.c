/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:44:48 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/29 14:49:03 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	read_file(t_game *game, int fd)
{
	char	*line;
	char	*temp;
	char	*aux;

	line = ft_strdup("");
	while (1)
	{
		temp = get_next_line(fd);
		if (!temp)
			break ;
		aux = ft_strjoin(line, temp);
		free(line);
		free(temp);
		line = aux;
	}
	game->map->file_content = ft_split(line, '\n');
	free(line);
}

void	validate_file(t_game *game, char *file)
{
	char	*extension;
	int		fd;

	extension = ft_strrchr(file, '.');
	if (ft_strcmp(extension, ".cub") != 0)
		shutdown_program(game, 6);
	fd = open(file, O_RDONLY);
	if (fd < 0)
		shutdown_program(game, 7);
	read_file(game, fd);
}
