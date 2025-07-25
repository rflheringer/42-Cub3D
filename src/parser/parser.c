/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:26:22 by marvin            #+#    #+#             */
/*   Updated: 2025/07/25 14:08:49 by rdel-fra         ###   ########.fr       */
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
	validate_map(game);
}
