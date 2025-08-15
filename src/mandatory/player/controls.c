/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 13:17:09 by rheringe          #+#    #+#             */
/*   Updated: 2025/08/15 14:14:32 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d.h"

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
	if (keydata.key == MLX_KEY_ESCAPE)
		mlx_close_window(game->mlx);
	else if (keydata.key == MLX_KEY_W)
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

void	keypress(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT)
		key_press(keydata, game);
	else if (keydata.action == MLX_RELEASE)
		key_release(keydata, game);
}
