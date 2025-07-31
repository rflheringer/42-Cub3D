/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_player.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:05:29 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/31 16:56:10 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	set_north(t_game *game)
{
	game->player->player_dir_x = 0;
	game->player->player_dir_y = -1;
	game->player->camera_dir_x = 0;
	game->player->camera_dir_y = 0.66;
}

static void	set_field_of_vision(t_game *game, int i, int j)
{
	if (game->map->map[i][j] == 'N')
		set_north(game);
	else if (game->map->map[i][j] == 'S')
	{
		game->player->player_dir_x = 0;
		game->player->player_dir_y = 1;
		game->player->camera_dir_x = 0;
		game->player->camera_dir_y = 0.66;
	}
	else if (game->map->map[i][j] == 'E')
	{
		game->player->player_dir_x = 1;
		game->player->player_dir_y = 0;
		game->player->camera_dir_x = 0;
		game->player->camera_dir_y = 0.66;
	}
	else if (game->map->map[i][j] == 'W')
	{
		game->player->player_dir_x = -1;
		game->player->player_dir_y = 0;
		game->player->camera_dir_x = 0;
		game->player->camera_dir_y = 0.66;
	}
}

static bool	check_player_position(char **map, int i, int j)
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

void	get_player_position(t_game *game, char **map, int i, int j)
{
	if (!check_player_position(map, i, j))
		shutdown_program(game, EXIT_INVALID_START_POSITION);
	game->player->move_speed = 0.05;
	game->player->rotation_speed = 0.03;
	game->player->up = false;
	game->player->down = false;
	game->player->rot_left = false;
	game->player->rot_right = false;
	game->player->start_dir = map[i][j];
	game->player->pos_x = j + 0.5;
	game->player->pos_y = i + 0.5;
	game->player->p++;
	set_field_of_vision(game, i, j);
}

void	check_player(t_game *game)
{
	if (game->player->p > 1)
		shutdown_program(game, EXIT_TOO_MANY_START_POS);
	else if (game->player->p < 1)
		shutdown_program(game, EXIT_NO_START_POSITION);
}
