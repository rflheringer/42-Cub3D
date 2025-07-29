/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:41:18 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/07/29 15:21:01 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	init_map_images(t_game *game)
{
	game->texture->south_wall = mlx_load_png("assets/map/dungeon_wall_1.png");
	if (!game->texture->south_wall)
		error_messages(EXIT_FAILED_TO_LOAD_TEXTURE);
	game->image->south_wall = mlx_texture_to_image(game->mlx,
			game->texture->south_wall);
	if (!game->image->south_wall)
		error_messages(EXIT_FAILED_TO_LOAD_IMAGE);
	game->texture->north_wall = mlx_load_png("assets/map/dungeon_wall_2.png");
	if (!game->texture->north_wall)
		error_messages(EXIT_FAILED_TO_LOAD_TEXTURE);
	game->image->north_wall = mlx_texture_to_image(game->mlx,
			game->texture->north_wall);
	if (!game->image->north_wall)
		error_messages(EXIT_FAILED_TO_LOAD_IMAGE);
}

int32_t	init_cub3d(t_game *game)
{
	game->mlx = mlx_init(WIDTH, HEIGHT, W_NAME, true);
	init_map_images(game);
	game->player->up = false;
	game->player->down = false;
	game->player->rot_left = false;
	game->player->rot_right = false;
	perform_raycasting(game);
	mlx_key_hook(game->mlx, &keypress, game);
	mlx_loop_hook(game->mlx, &handle_movement, game);
	mlx_loop(game->mlx);
	return (0);
}
