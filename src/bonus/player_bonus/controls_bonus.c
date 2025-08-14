/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 13:17:09 by rheringe          #+#    #+#             */
/*   Updated: 2025/08/14 19:59:17 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static void	key_release(mlx_key_data_t keydata, t_game *game)
{
	if (keydata.key == MLX_KEY_W)
		game->player->up = false;
	else if (keydata.key == MLX_KEY_S)
		game->player->down = false;
	else if (keydata.key == MLX_KEY_D)
		game->player->right = false;
	else if (keydata.key == MLX_KEY_A)
		game->player->left = false;
	else if (keydata.key == MLX_KEY_LEFT)
		game->player->rot_right = false;
	else if (keydata.key == MLX_KEY_RIGHT)
		game->player->rot_left = false;
}

static void	key_press(mlx_key_data_t keydata, t_game *game)
{
	if (keydata.key == MLX_KEY_W)
		game->player->up = true;
	else if (keydata.key == MLX_KEY_S)
		game->player->down = true;
	else if (keydata.key == MLX_KEY_D)
		game->player->right = true;
	else if (keydata.key == MLX_KEY_A)
		game->player->left = true;
	else if (keydata.key == MLX_KEY_LEFT)
		game->player->rot_right = true;
	else if (keydata.key == MLX_KEY_RIGHT)
		game->player->rot_left = true;
}

void	rotate_player_mouse(double xpos, double ypos, void *param)
{
	t_game	*game;
	double	original_speed;

	(void)ypos;
	game = (t_game *)param;
	if (game->game_over || game->game_win)
		return ;
	original_speed = game->player->rotation_speed;
	if (xpos < WIDTH * 0.3)
	{
		game->player->rotation_speed = original_speed * 0.4;
		rot_right(game);
		game->player->moved = true;
	}
	else if (xpos > WIDTH * 0.7)
	{
		game->player->rotation_speed = original_speed * 0.4;
		rot_left(game);
		game->player->moved = true;
	}
	game->player->rotation_speed = original_speed;
}

void	keypress(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (keydata.key == MLX_KEY_ESCAPE)
		mlx_close_window(game->mlx);
	if (game->game_over || game->game_win)
		return ;
	if (keydata.action == MLX_PRESS && keydata.key == MLX_KEY_F)
		can_open_or_close_door(game);
	else if (keydata.action == MLX_PRESS && keydata.key == MLX_KEY_SPACE
		&& game->player->attack_delay > 0.4)
	{
		create_fireball(game);
		game->player->attack_delay = 0;
	}
	else if (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT)
		key_press(keydata, game);
	else if (keydata.action == MLX_RELEASE)
		key_release(keydata, game);
}
