/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_textures.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafaelheringer <rafaelheringer@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:26:22 by marvin            #+#    #+#             */
/*   Updated: 2025/08/01 22:24:36 by rafaelherin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static void	count_textures(t_game *game, char *content)
{
	if (startswith(content, "NO"))
		game->texture->north++;
	else if (startswith(content, "SO"))
		game->texture->south++;
	else if (startswith(content, "WE"))
		game->texture->west++;
	else if (startswith(content, "EA"))
		game->texture->east++;
	else if (startswith(content, "F"))
		game->texture->floor++;
	else if (startswith(content, "C"))
		game->texture->ceiling++;
	else
	{
		if (!is_valid_line(content))
			shutdown_program(game, EXIT_UNEXPECTED_CHAR);
	}
}

static char	*get_path(char *content, char *start)
{
	char	*trimmed;
	char	*path;
	int		len;
	int		i;

	len = ft_strlen(start);
	trimmed = ft_strtrim(content, " ");
	i = len;
	while (trimmed[i])
	{
		if (trimmed[i] != ' ')
		{
			path = ft_substr(trimmed, i, ft_strlen(trimmed) - i);
			free(trimmed);
			return (path);
		}
		i++;
	}
	free(trimmed);
	return (NULL);
}

static bool	find_textures(t_game *game, char *content)
{
	count_textures(game, content);
	if (game->texture->north > 1 || game->texture->south > 1
		|| game->texture->west > 1 || game->texture->east > 1)
		shutdown_program(game, EXIT_DUPLICATE_TEXTURE);
	if (game->texture->floor > 1 || game->texture->ceiling > 1)
		shutdown_program(game, EXIT_DUPLICATE_COLOR);
	if (startswith(content, "NO"))
		game->texture->north_path = get_path(content, "NO");
	else if (startswith(content, "SO"))
		game->texture->south_path = get_path(content, "SO");
	else if (startswith(content, "WE"))
		game->texture->west_path = get_path(content, "WE");
	else if (startswith(content, "EA"))
		game->texture->east_path = get_path(content, "EA");
	else if (startswith(content, "F"))
		game->texture->floor_color = get_path(content, "F");
	else if (startswith(content, "C"))
		game->texture->ceiling_color = get_path(content, "C");
	else
	{
		if (!is_valid_line(content))
			shutdown_program(game, EXIT_UNEXPECTED_CHAR);
		return (false);
	}
	return (true);
}

void	get_text_color_and_map(t_game *game, char **content)
{
	int		i;

	i = 0;
	game->texture = ft_calloc(1, sizeof(t_texture));
	while (content[i])
	{
		if (has_control_char(content[i]))
			shutdown_program(game, 10);
		if (!only_spaces(content[i]))
		{
			if (!find_textures(game, content[i]))
			{
				get_map(game, content, &i);
				break ;
			}
		}
		i++;
	}
	if (!game->texture->north_path || !game->texture->south_path
		|| !game->texture->west_path || !game->texture->east_path)
		shutdown_program(game, EXIT_INVALID_TEXTURE_PATH);
	if (!game->texture->floor_color || !game->texture->ceiling_color)
		shutdown_program(game, EXIT_INVALID_RGB_COLOR);
}

void	validate_textures(t_game *game, char *path)
{
	char	*extension;
	int		fd;

	extension = ft_strrchr(path, '.');
	if (!extension)
		shutdown_program(game, EXIT_INVALID_TEXTURE_PATH);
	if (ft_strcmp(extension, ".png") != 0)
		shutdown_program(game, EXIT_INVALID_TEXTURE_PATH);
	fd = open(path, O_RDONLY);
	if (fd == -1)
		shutdown_program(game, EXIT_INVALID_TEXTURE_PATH);
	close(fd);
}
