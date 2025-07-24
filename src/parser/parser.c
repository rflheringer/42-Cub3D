/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:26:22 by marvin            #+#    #+#             */
/*   Updated: 2025/07/24 12:15:10 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	read_file(t_game *game, int fd)
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
	int	fd;

	if (!ft_strnstr(file, ".cub", ft_strlen(file)))
	{
		(void)game;
		exit(1);
	}
	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		(void)game;
		exit(1);
	}
	read_file(game, fd);
}

bool	only_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isspace(str[i]))
			return (false);
		i++;
	}
	return (true);
}

bool	find_textures(t_game *game, char *content)
{
	if (startswith(content, "NO"))
		game->texture->north_path = get_path(content);
	else if (startswith(content, "SO"))
		game->texture->south_path = get_path(content);
	else if (startswith(content, "WE"))
		game->texture->west_path = get_path(content);
	else if (startswith(content, "EA"))
		game->texture->east_path = get_path(content);
	else if (startswith(content, "F"))
		game->texture->floor_color = get_color(content);
	else if (startswoth(content, "C"))
		game->texture->ceiling_color = get_color(content);
	else
	{
		if (!is_valid_line(content))
		{
			ft_printf("Error\nInvalid line\n");
			exit (1);
		}
		return (false);
	}
	return (true);
}

bool	has_control_char(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] >= 9 && str[i] <= 13)
			return (true);
		i++;
	}
	return (false);
}

void	get_text_color_and_map(t_game *game, char **content)
{
	int		i;

	i = 0;
	while (content[i])
	{
		if (has_control_char(content[i]))
		{
			ft_printf("Error\nControl character found when expected space\n");
			exit (1);
		}
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
}
