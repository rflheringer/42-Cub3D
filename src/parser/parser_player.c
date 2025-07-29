/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_player.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:05:29 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/29 13:15:42 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	set_north(t_game *game)
{
	game->player->player_dir_x = 0;
	game->player->player_dir_y = -1;
	game->player->camera_dir_x = FOV;
	game->player->camera_dir_y = 0;
}

static void	set_field_of_vision(t_game *game, int i, int j)
{
	if (game->map->map[i][j] == 'N')
		set_north(game);
	else if (game->map->map[i][j] == 'S')
	{
		game->player->player_dir_x = 0;
		game->player->player_dir_y = 1;
		game->player->camera_dir_x = -FOV;
		game->player->camera_dir_y = 0;
	}
	else if (game->map->map[i][j] == 'E')
	{
		game->player->player_dir_x = 1;
		game->player->player_dir_y = 0;
		game->player->camera_dir_x = 0;
		game->player->camera_dir_y = FOV;
	}
	else if (game->map->map[i][j] == 'W')
	{
		game->player->player_dir_x = -1;
		game->player->player_dir_y = 0;
		game->player->camera_dir_x = 0;
		game->player->camera_dir_y = -FOV;
	}
}

void	get_player_position(t_game *game, char **map, int i, int j)
{
	game->player->start_dir = map[i][j];
	game->player->pos_x = j + 0.5;
	game->player->pos_y = i + 0.5;
	game->player->p++;
	set_field_of_vision(game, i, j);
}

void	check_player(t_game *game)
{
	if (game->player->p > 1)
		shutdown_program(game, 13);
	else if (game->player->p < 1)
		shutdown_program(game, 14);
}
