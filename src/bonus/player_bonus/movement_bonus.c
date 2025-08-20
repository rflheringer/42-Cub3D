/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:45:53 by rheringe          #+#    #+#             */
/*   Updated: 2025/08/20 12:11:30 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static bool	up_move(t_game *game)
{
	double	new_x;
	double	new_y;
	double	speed;

	if (game->player->sprint)
		speed = game->player->move_speed * 2.0;
	else
		speed = game->player->move_speed;
	new_x = game->player->pos_x + (game->player->player_dir_x * speed);
	new_y = game->player->pos_y + (game->player->player_dir_y * speed);
	if (can_move_to(game, new_x, new_y, game->enemy->list))
	{
		check_key_and_potion(game, new_x, new_y);
		game->player->old_x = game->player->pos_x;
		game->player->old_y = game->player->pos_y;
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
	double	speed;

	if (game->player->sprint)
		speed = game->player->move_speed * 2.0;
	else
		speed = game->player->move_speed;
	new_x = game->player->pos_x - game->player->player_dir_x * speed;
	new_y = game->player->pos_y - game->player->player_dir_y * speed;
	if (can_move_to(game, new_x, new_y, game->enemy->list))
	{
		check_key_and_potion(game, new_x, new_y);
		game->player->old_x = game->player->pos_x;
		game->player->old_y = game->player->pos_y;
		game->player->pos_x = new_x;
		game->player->pos_y = new_y;
		return (true);
	}
	return (false);
}

bool	rot_left(t_game *game)
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

bool	rot_right(t_game *game)
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

void	get_move(t_game *game)
{
	if (game->player->up)
		if (up_move(game))
			game->player->moved = true;
	if (game->player->down)
		if (down_move(game))
			game->player->moved = true;
	if (game->player->right)
		if (right_move(game))
			game->player->moved = true;
	if (game->player->left)
		if (left_move(game))
			game->player->moved = true;
	if (game->player->rot_left)
		if (rot_left(game))
			game->player->moved = true;
	if (game->player->rot_right)
		if (rot_right(game))
			game->player->moved = true;
}
