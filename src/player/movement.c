/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:45:53 by rheringe          #+#    #+#             */
/*   Updated: 2025/07/30 14:15:07 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	up_move(t_game *game)
{
	double	new_x;
	double	new_y;

	new_x = game->player->pos_x + (game->player->player_dir_x
			* game->player->move_speed);
	new_y = game->player->pos_y + (game->player->player_dir_y
			* game->player->move_speed);
	if (game->map->map[(int)new_y][(int)new_x] != '1')
	{
		game->player->pos_x = new_x;
		game->player->pos_y = new_y;
		return (true);
	}
	return (false);
}

static bool	down_move(t_game *game)
{
	double	new_x;
	double	new_y;

	new_x = game->player->pos_x - game->player->player_dir_x
		* game->player->move_speed;
	new_y = game->player->pos_y - game->player->player_dir_y
		* game->player->move_speed;
	if (game->map->map[(int)new_y][(int)new_x] != '1')
	{
		game->player->pos_x = new_x;
		game->player->pos_y = new_y;
		return (true);
	}
	return (false);
}

static bool	rot_left(t_game *game)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = game->player->player_dir_x;
	game->player->player_dir_x = game->player->player_dir_x
		* cos(-game->player->rotation_speed)
		- game->player->player_dir_y * sin(-game->player->rotation_speed);
	game->player->player_dir_y = old_dir_x
		* sin(-game->player->rotation_speed)
		+ game->player->player_dir_y * cos(-game->player->rotation_speed);
	old_plane_x = game->player->camera_dir_x;
	game->player->camera_dir_x = game->player->camera_dir_x
		* cos(-game->player->rotation_speed)
		- game->player->camera_dir_y * sin(-game->player->rotation_speed);
	game->player->camera_dir_y = old_plane_x
		* sin(-game->player->rotation_speed)
		+ game->player->camera_dir_y * cos(-game->player->rotation_speed);
	return (true);
}

static bool	rot_right(t_game *game)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = game->player->player_dir_x;
	game->player->player_dir_x = game->player->player_dir_x
		* cos(game->player->rotation_speed)
		- game->player->player_dir_y * sin(game->player->rotation_speed);
	game->player->player_dir_y = old_dir_x
		* sin(game->player->rotation_speed)
		+ game->player->player_dir_y * cos(game->player->rotation_speed);
	old_plane_x = game->player->camera_dir_x;
	game->player->camera_dir_x = game->player->camera_dir_x
		* cos(game->player->rotation_speed)
		- game->player->camera_dir_y * sin(game->player->rotation_speed);
	game->player->camera_dir_y = old_plane_x
		* sin(game->player->rotation_speed)
		+ game->player->camera_dir_y * cos(game->player->rotation_speed);
	return (true);
}

void	handle_movement(void *param)
{
	t_game	*game;
	bool	moved;

	game = (t_game *)param;
	moved = false;
	if (game->player->up)
		if (up_move(game))
			moved = true;
	if (game->player->down)
		if (down_move(game))
			moved = true;
	if (game->player->rot_left)
		if (rot_right(game))
			moved = true;
	if (game->player->rot_right)
		if (rot_left(game))
			moved = true;
	if (moved)
	{
		if (game->raycasting->image)
			mlx_delete_image(game->mlx, game->raycasting->image);
		perform_raycasting(game);
	}
}
