/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:44:48 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/30 17:18:06 by rdel-fra         ###   ########.fr       */
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

static void	verify_map_separation(t_game *game, char *line, int i)
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
				if (line[i + 1] && (line[i + 1] == '\n' || line[i + 1] == ' '))
				{
					while (line[i] && (line[i] == '\n' || line[i] == ' '))
						i++;
					if (line[i] != '\0')
						shutdown_program(game, EXIT_INVALID_MAP);
				}
			}
		}
		if (line[i])
			i++;
	}
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
	verify_map_separation(game, line, i);
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
}
