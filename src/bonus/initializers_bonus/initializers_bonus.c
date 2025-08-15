/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:41:18 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/08/15 16:20:18 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static uint32_t	get_color_with_alpha(int r, int g, int b, int a)
{
	return ((r & 0xFF) << 24 | (g & 0xFF) << 16 | (b & 0xFF) << 8 | (a & 0xFF));
}

static uint32_t	convert_rgb_string(t_game *game, char *rgb_str)
{
	char	**rgb_value;
	int		r;
	int		g;
	int		b;

	rgb_value = ft_split(rgb_str, ',');
	if (!rgb_value)
		shutdown_program(game, EXIT_INVALID_RGB_COLOR);
	r = ft_atoi(rgb_value[0]);
	g = ft_atoi(rgb_value[1]);
	b = ft_atoi(rgb_value[2]);
	ft_free_matrix(rgb_value);
	return (get_color_with_alpha(r, g, b, 255));
}

static void	init_colors(t_game *game)
{
	if (game->texture->ceiling_color)
		game->texture->ceiling_color_hex
			= convert_rgb_string(game, game->texture->ceiling_color);
	if (game->texture->floor_color)
		game->texture->floor_color_hex
			= convert_rgb_string(game, game->texture->floor_color);
	game->texture->closed_door = mlx_load_png("assets/map/door_2.png");
	game->texture->potion = mlx_load_png("assets/collectables/pot.png");
	game->texture->key = mlx_load_png("assets/collectables/key.png");
}

static void	init_map_images(t_game *game)
{
	game->texture->south_wall = mlx_load_png(game->texture->south_path);
	if (!game->texture->south_wall)
		shutdown_program(game, EXIT_INVALID_TEXTURE_PATH);
	game->image->south_wall = mlx_texture_to_image(game->mlx,
			game->texture->south_wall);
	game->texture->north_wall = mlx_load_png(game->texture->north_path);
	if (!game->texture->north_wall)
		shutdown_program(game, EXIT_INVALID_TEXTURE_PATH);
	game->image->north_wall = mlx_texture_to_image(game->mlx,
			game->texture->north_wall);
	game->texture->east_wall = mlx_load_png(game->texture->east_path);
	if (!game->texture->east_wall)
		shutdown_program(game, EXIT_INVALID_TEXTURE_PATH);
	game->image->east_wall = mlx_texture_to_image(game->mlx,
			game->texture->east_wall);
	game->texture->west_wall = mlx_load_png(game->texture->west_path);
	if (!game->texture->west_wall)
		shutdown_program(game, EXIT_INVALID_TEXTURE_PATH);
	game->image->west_wall = mlx_texture_to_image(game->mlx,
			game->texture->west_wall);
	if (!game->image->north_wall || !game->image->south_wall
		|| !game->image->east_wall || !game->image->west_wall)
		shutdown_program(game, EXIT_INVALID_TEXTURE_PATH);
	init_colors(game);
}

int32_t	init_cub3d(t_game *game)
{
	game->mlx = mlx_init(WIDTH, HEIGHT, W_NAME, true);
	game->image = ft_calloc(1, sizeof(t_image));
	game->ui = ft_calloc(1, sizeof(t_life_ui));
	init_map_images(game);
	init_lightning(game);
	init_bonus_images(game);
	game->ray = ft_calloc(1, sizeof(t_ray));
	game->wall = ft_calloc(1, sizeof(t_wall));
	game->raycasting = ft_calloc(1, sizeof(t_raycasting));
	game->raycasting->buffer = ft_calloc(WIDTH, sizeof(double));
	mlx_key_hook(game->mlx, &keypress, game);
	mlx_cursor_hook(game->mlx, &rotate_player_mouse, game);
	mlx_loop_hook(game->mlx, &handle_movement, game);
	mlx_loop(game->mlx);
	shutdown_program(game, EXIT_SUCCESS);
	return (0);
}
