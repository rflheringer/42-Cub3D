/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:37:30 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/29 15:00:08 by rdel-fra         ###   ########.fr       */
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
			shutdown_program(game, 11);
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
	len = ft_count_map(game, content, *i);
	game->map->map = ft_calloc(len + 1, sizeof(char *));
	while (content[*i])
	{
		game->map->map[j] = content[*i];
		(*i)++;
		j++;
	}
	game->map->heigth = ft_ptrlen(game->map->map);
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
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (map[i][j] == '0')
				if (!check_valid_zero(map, i, j))
					shutdown_program(game, 11);
			if (ft_strchr("NSWE", map[i][j]))
				get_player_position(game, map, i, j);
			j++;
		}
		i++;
	}
	check_player(game);
}
