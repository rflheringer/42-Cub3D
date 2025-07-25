/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:26:22 by marvin            #+#    #+#             */
/*   Updated: 2025/07/25 16:24:12 by rdel-fra         ###   ########.fr       */
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
	char	*extension;
	int		fd;

	extension = ft_strrchr(file, '.');
	if (ft_strcmp(extension, ".cub") != 0)
	{
		(void)game;
		ft_printf("Error\nInvalid Extension\n");
		exit(1);
	}
	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		(void)game;
		ft_printf("Error\nInvalid file\n");
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

bool	startswith(char *str, char *start)
{
	char	*trimmed;
	int		len;

	len = ft_strlen(start);
	trimmed = ft_strtrim(str, " ");
	if (ft_strnstr(trimmed, start, len) && trimmed[len] == 32)
	{
		ft_printf("is valid!\n");
		return (true);
	}
	return (false);
}

char	*get_path(char *content, char *start)
{
	char	*trimmed;
	int		len;
	int		i;

	len = ft_strlen(start);
	trimmed = ft_strtrim(content, " ");
	i = len;
	while (trimmed[i])
	{
		if (trimmed[i] != ' ')
			return (&trimmed[i]);
		i++;
	}
	return (NULL);
}

bool	is_valid_line(char *content)
{
	int	i;

	i = 0;
	while (content[i])
	{
		if (!ft_strchr(" 01NSWE", content[i]))
			return (false);
		i++;
	}
	return (true);
}

bool	find_textures(t_game *game, char *content)
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

int	ft_count_map(char **content, int i)
{
	int	len;

	len = 0;
	while (content[i])
	{
		if (has_control_char(content[i]))
		{
			ft_printf("Error\nControl character found when expected space\n");
			exit (1);
		}
		len++;
		i++;
	}
	return (len);
}

void	get_map(t_game *game, char **content, int *i)
{
	int		len;
	int		j;

	j = 0;
	len = ft_count_map(content, *i);
	game->map->map = ft_calloc(len + 1, sizeof(char *));
	while (content[*i])
	{
		game->map->map[j] = content[*i];
		(*i)++;
		j++;
	}
	game->map->heigth = ft_ptrlen(game->map->map);
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

bool	check_valid_zero(char **map, int i, int j)
{
	if (i == 0)
		return (false);
	if (!map[i + 1])
		return (false);
	if (j > 0 && map[i][j - 1] == ' ')
		return (false);
	if (i > 0 && map[i- 1][j] == ' ')
		return (false);
	if (map[i][j + 1] == '\0' || map[i][j + 1] == ' ')
		return (false);
	if (map[i + 1] && map[i + 1][j] == ' ')
		return (false);
	return (true);
}

void	validate_map(t_game *game, char **map)
{
	int	i;
	int	j;

	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (map[i][j] == '0')
			{
				if (!check_valid_zero(map, i, j))
				{
					(void)game;
					ft_printf("Error\nMap not closed\n");
					exit (1);
				}
			}
			j++;
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
