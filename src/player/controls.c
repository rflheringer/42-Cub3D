/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 13:17:09 by rheringe          #+#    #+#             */
/*   Updated: 2025/07/30 15:21:05 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	keypress(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT)
	{
		if (keydata.key == MLX_KEY_W)
		game->player->up = true;
		else if (keydata.key == MLX_KEY_S)
		game->player->down = true;
		else if (keydata.key == MLX_KEY_D)
		game->player->rot_left = true;
		else if (keydata.key == MLX_KEY_A)
		game->player->rot_right = true;
	}
	else if (keydata.action == MLX_RELEASE)
	{
		if (keydata.key == MLX_KEY_W)
		game->player->up = false;
		else if (keydata.key == MLX_KEY_S)
		game->player->down = false;
		else if (keydata.key == MLX_KEY_D)
		game->player->rot_left = false;
		else if (keydata.key == MLX_KEY_A)
		game->player->rot_right = false;
	}
