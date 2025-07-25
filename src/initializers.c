/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:41:18 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/07/25 17:43:19 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	static init_map_images(t_game *game)
{
	game->texture->south_wall = mlx_load_png("assets/map/dungeon_wall_1.png");
	if (!game->texture->south_wall)
		error_messages(EXIT_FAILED_TO_LOAD_TEXTURE);
	game->image->south_wall = mlx_texture_to_image(game->mlx,
			game->texture->south_wall);
	if (!game->image->south_wall)
		error_messages(EXIT_FAILED_TO_LOAD_IMAGE);
}
int32_t	init_cub3d(t_game *game)
{
	game->mlx = mlx_init(800, 600, "Cub3D", true);
	init_map_images(game);
	mlx_loop(game->mlx);
	return (0);
}

