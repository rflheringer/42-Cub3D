/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:37:30 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/31 13:52:37 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static int	ft_count_map(t_game *game, char **content, int i)
{
	int	len;

	len = 0;
	while (content[i])
	{
		if (has_control_char(content[i]))
			shutdown_program(game, EXIT_CHAR_CONTROL);
		len++;
		i++;
	}
	return (len);
}

void	get_map(t_game *game, char **content, int *i)
{
	int		first;
	int		len;
	int		j;

	j = 0;
	first = 1;
	len = ft_count_map(game, content, *i);
	game->map->map = ft_calloc(len + 1, sizeof(char *));
	while (content[*i])
	{
		if (ft_findchar(content[*i], "01NSWE"))
			first = 0;
		if (first == 0)
			if (only_spaces(content[*i]))
				first = 2;
		if (first == 2 && ft_findchar(content[*i], "01NSWE"))
			shutdown_program(game, EXIT_INVALID_MAP);
		game->map->map[j++] = ft_strdup(content[*i]);
		(*i)++;
	}
	game->map->height = ft_ptrlen(game->map->map);
}

static bool	check_valid_zero(char **map, int i, int j)
{
	if (i == 0)
		return (false);
	if (!map[i + 1])
		return (false);
	if (j > 0 && (map[i][j - 1] == ' ' || map[i][j - 1] == '\0'))
		return (false);
	if (i > 0 && (map[i - 1][j] == ' ' || map[i - 1][j] == '\0'))
		return (false);
	if (map[i][j + 1] == '\0' || map[i][j + 1] == ' ')
		return (false);
	if (map[i + 1] && (map[i + 1][j] == ' ' || map[i + 1][j] == '\0'))
		return (false);
	return (true);
}

void	validate_map(t_game *game, char **map)
{
	int	i;
	int	j;

	i = 0;
	if (!map || !*map)
		shutdown_program(game, EXIT_INVALID_MAP);
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (!ft_strchr(" 01NSWE", map[i][j]))
				shutdown_program(game, EXIT_UNEXPECTED_CHAR);
			if (map[i][j] == '0')
				if (!check_valid_zero(map, i, j))
					shutdown_program(game, EXIT_MAP_NOT_CLOSED);
			if (ft_strchr("NSWE", map[i][j]))
				get_player_position(game, map, i, j);
			j++;
		}
		i++;
	}
	check_player(game);
}
