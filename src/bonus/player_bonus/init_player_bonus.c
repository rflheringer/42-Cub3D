/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_player_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:05:29 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/15 18:46:23 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static void	set_north(t_game *game)
{
	game->player->player_dir_x = 0;
	game->player->player_dir_y = -1;
	game->player->camera_dir_x = game->player->player_dir_y * tan(FOV / 2.0);
	game->player->camera_dir_y = -game->player->player_dir_x * tan(FOV / 2.0);
}

static void	set_south(t_game *game)
{
	game->player->player_dir_x = 0;
	game->player->player_dir_y = 1;
	game->player->camera_dir_x = game->player->player_dir_y * tan(FOV / 2.0);
	game->player->camera_dir_y = -game->player->player_dir_x * tan(FOV / 2.0);
}

static void	set_field_of_vision(t_game *game, int i, int j)
{
	if (game->map->map[i][j] == 'N')
		set_north(game);
	else if (game->map->map[i][j] == 'S')
		set_south(game);
	else if (game->map->map[i][j] == 'E')
	{
		game->player->player_dir_x = 1;
		game->player->player_dir_y = 0;
		game->player->camera_dir_x = game->player->player_dir_y
			* tan(FOV / 2.0);
		game->player->camera_dir_y = -game->player->player_dir_x
			* tan(FOV / 2.0);
	}
	else if (game->map->map[i][j] == 'W')
	{
		game->player->player_dir_x = -1;
		game->player->player_dir_y = 0;
		game->player->camera_dir_x = game->player->player_dir_y
			* tan(FOV / 2.0);
		game->player->camera_dir_y = -game->player->player_dir_x
			* tan(FOV / 2.0);
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
	game->player->move_speed = 0.06;
	game->player->rotation_speed = 0.03;
	game->player->start_dir = map[i][j];
	game->player->pos_x = j + 0.5;
	game->player->pos_y = i + 0.5;
	game->player->p++;
	game->player->hp = LIFE_MAX;
	set_field_of_vision(game, i, j);
}
