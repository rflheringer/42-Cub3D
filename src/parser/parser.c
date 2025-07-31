/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:44:48 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/31 12:05:21 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	is_map_line(char *line, int *i)
{
	if (line[*i] == '\n')
		return (false);
	while (line[*i] && line[*i] != '\n')
	{
		if (!ft_strchr("01NSWE", line[*i]))
		{
			while (line[*i] && line[*i] != '\n')
				(*i)++;
			return (false);
		}
		(*i)++;
	}
	return (true);
}

static bool	verify_map_separation(char *line, int i)
{
	while (line[i])
	{
		if (is_map_line(line, &i))
		{
			while (line[i] && line[i] != '\n')
			{
				if (!ft_strchr(" 01NSWE", line[i]))
					break ;
				i++;
			}
			if (line[i] == '\n')
			{
				if (line[i + 1] && (line[i + 1] == '\n'))
				{
					while (line[i] && (line[i] == '\n' || line[i] == ' '))
						i++;
					if (line[i] != '\0')
						return (false);
				}
			}
		}
		if (line[i])
			i++;
	}
	return (true);
}

static void	read_file(t_game *game, int fd)
{
	char	*line;
	char	*temp;
	char	*aux;
	int		i;

	i = 0;
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
	close(fd);
	if (!verify_map_separation(line, i))
	{
		free(line);
		shutdown_program(game, EXIT_INVALID_MAP);
	}
	game->map->file_content = ft_split(line, '\n');
	free(line);
}

static void	validate_file(t_game *game, char *file)
{
	char	*extension;
	int		fd;

	extension = ft_strrchr(file, '.');
	if (!extension)
		shutdown_program(game, EXIT_INVALID_EXTENSION);
	if (ft_strcmp(extension, ".cub") != 0)
		shutdown_program(game, EXIT_INVALID_EXTENSION);
	fd = open(file, O_RDONLY);
	if (fd < 0)
		shutdown_program(game, EXIT_INVALID_FILE);
	read_file(game, fd);
}

void	parser(t_game *game, char *file)
{
	validate_file(game, file);
	get_text_color_and_map(game, game->map->file_content);
	validate_map(game, game->map->map);
	validate_textures(game, game->texture->north_path);
	validate_textures(game, game->texture->south_path);
	validate_textures(game, game->texture->west_path);
	validate_textures(game, game->texture->east_path);
	validate_colors(game);
}
