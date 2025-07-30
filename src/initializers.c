/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:41:18 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/07/30 17:00:14 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	init_colors(t_game *game)
{
	if (game->texture->ceiling_color)
		game->texture->ceiling_color_hex
			= convert_rgb_string(game->texture->ceiling_color);
	if (game->texture->floor_color)
		game->texture->floor_color_hex
			= convert_rgb_string(game->texture->floor_color);
}

static void	init_map_images(t_game *game)
{
	game->texture->south_wall = mlx_load_png("assets/map/dungeon_wall_1.png");
	if (!game->texture->south_wall)
		error_messages(EXIT_FAILED_TO_LOAD_TEXTURE);
	game->image->south_wall = mlx_texture_to_image(game->mlx,
			game->texture->south_wall);
	game->texture->north_wall = mlx_load_png("assets/map/dungeon_wall_1.png");
	if (!game->texture->north_wall)
		error_messages(EXIT_FAILED_TO_LOAD_TEXTURE);
	game->image->north_wall = mlx_texture_to_image(game->mlx,
			game->texture->north_wall);
	game->texture->east_wall = mlx_load_png("assets/map/dungeon_wall_2.png");
	if (!game->texture->east_wall)
		error_messages(EXIT_FAILED_TO_LOAD_TEXTURE);
	game->image->east_wall = mlx_texture_to_image(game->mlx,
			game->texture->east_wall);
	game->texture->west_wall = mlx_load_png("assets/map/dungeon_wall_2.png");
	if (!game->texture->west_wall)
		error_messages(EXIT_FAILED_TO_LOAD_TEXTURE);
	game->image->west_wall = mlx_texture_to_image(game->mlx,
			game->texture->west_wall);
	if (!game->image->north_wall || !game->image->south_wall
		|| !game->image->east_wall || !game->image->west_wall)
		error_messages(EXIT_FAILED_TO_LOAD_IMAGE);
	init_colors(game);
}

int32_t	init_cub3d(t_game *game)
{
	game->mlx = mlx_init(WIDTH, HEIGHT, W_NAME, true);
	init_map_images(game);
	perform_raycasting(game);
	mlx_key_hook(game->mlx, &keypress, game);
	mlx_loop_hook(game->mlx, &handle_movement, game);
	mlx_loop(game->mlx);
	shutdown_program(game, EXIT_SUCCESS);
	return (0);
}
