/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:41:18 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/07/30 18:44:38 by rdel-fra         ###   ########.fr       */
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
	game->texture->south_wall = mlx_load_png(game->texture->south_path);
	if (!game->texture->south_wall)
		shutdown_program(game, EXIT_FAILED_TO_LOAD_TEXTURE);
	game->image->south_wall = mlx_texture_to_image(game->mlx,
			game->texture->south_wall);
	game->texture->north_wall = mlx_load_png(game->texture->north_path);
	if (!game->texture->north_wall)
		shutdown_program(game, EXIT_FAILED_TO_LOAD_TEXTURE);
	game->image->north_wall = mlx_texture_to_image(game->mlx,
			game->texture->north_wall);
	game->texture->east_wall = mlx_load_png(game->texture->east_path);
	if (!game->texture->east_wall)
		shutdown_program(game, EXIT_FAILED_TO_LOAD_TEXTURE);
	game->image->east_wall = mlx_texture_to_image(game->mlx,
			game->texture->east_wall);
	game->texture->west_wall = mlx_load_png(game->texture->west_path);
	if (!game->texture->west_wall)
		shutdown_program(game, EXIT_FAILED_TO_LOAD_TEXTURE);
	game->image->west_wall = mlx_texture_to_image(game->mlx,
			game->texture->west_wall);
	if (!game->image->north_wall || !game->image->south_wall
		|| !game->image->east_wall || !game->image->west_wall)
		shutdown_program(game, EXIT_FAILED_TO_LOAD_IMAGE);
	init_colors(game);
}

int32_t	init_cub3d(t_game *game)
{
	game->mlx = mlx_init(WIDTH, HEIGHT, W_NAME, true);
	game->image = ft_calloc(1, sizeof(t_image));
	init_map_images(game);
	game->ray = ft_calloc(1, sizeof(t_ray));
	game->wall = ft_calloc(1, sizeof(t_wall));
	game->raycasting = ft_calloc(1, sizeof(t_raycasting));
	perform_raycasting(game);
	mlx_key_hook(game->mlx, &keypress, game);
	mlx_loop_hook(game->mlx, &handle_movement, game);
	mlx_loop(game->mlx);
	shutdown_program(game, EXIT_SUCCESS);
	return (0);
}
