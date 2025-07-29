/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:26:22 by marvin            #+#    #+#             */
/*   Updated: 2025/07/29 14:49:08 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	find_textures(t_game *game, char *content)
{
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
			shutdown_program(game, 12);
		return (false);
	}
	return (true);
}

static void	get_text_color_and_map(t_game *game, char **content)
{
	int		i;

	i = 0;
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
}

void	parser(t_game *game, char *file)
{
	validate_file(game, file);
	get_text_color_and_map(game, game->map->file_content);
	validate_map(game, game->map->map);
}
