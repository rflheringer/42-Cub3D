/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:41:18 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/07/29 16:27:30 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	init_map_images(t_game *game)
{
    // Carregando a textura sul
    game->texture->south_wall = mlx_load_png("assets/map/dungeon_wall_1.png");
    if (!game->texture->south_wall)
        error_messages(EXIT_FAILED_TO_LOAD_TEXTURE);
    game->image->south_wall = mlx_texture_to_image(game->mlx,
            game->texture->south_wall);
    if (!game->image->south_wall)
        error_messages(EXIT_FAILED_TO_LOAD_IMAGE);
    
    // Carregando a textura norte
    game->texture->north_wall = mlx_load_png("assets/map/dungeon_wall_2.png");
    if (!game->texture->north_wall)
        error_messages(EXIT_FAILED_TO_LOAD_TEXTURE);
    game->image->north_wall = mlx_texture_to_image(game->mlx,
            game->texture->north_wall);
    if (!game->image->north_wall)
        error_messages(EXIT_FAILED_TO_LOAD_IMAGE);

    // Carregando a textura leste
    game->texture->east_wall = mlx_load_png("assets/map/dungeon_wall_2.png");
    if (!game->texture->east_wall)
        error_messages(EXIT_FAILED_TO_LOAD_TEXTURE);
    game->image->east_wall = mlx_texture_to_image(game->mlx,
            game->texture->east_wall);
    if (!game->image->east_wall)
        error_messages(EXIT_FAILED_TO_LOAD_IMAGE);

    // Carregando a textura oeste
    game->texture->west_wall = mlx_load_png("assets/map/dungeon_wall_1.png");
    if (!game->texture->west_wall)
        error_messages(EXIT_FAILED_TO_LOAD_TEXTURE);
    game->image->west_wall = mlx_texture_to_image(game->mlx,
            game->texture->west_wall);
    if (!game->image->west_wall)
        error_messages(EXIT_FAILED_TO_LOAD_IMAGE);
        
    // Converter cores RGB para hexadecimal
    if (game->texture->floor_color)
        game->texture->floor_color_hex = convert_rgb_string(game->texture->floor_color);
    if (game->texture->ceiling_color)
        game->texture->ceiling_color_hex = convert_rgb_string(game->texture->ceiling_color);
}

static void	set_field_of_vision(t_game *game, int i, int j)
{
	if (game->map->map[i][j] == 'N')
	{
		game->player->player_dir_x = 0;
		game->player->player_dir_y = -1;
		game->player->camera_dir_x = FOV;
		game->player->camera_dir_y = 0;
	}
	else if (game->map->map[i][j] == 'S')
	{
		game->player->player_dir_x = 0;
		game->player->player_dir_y = 1;
		game->player->camera_dir_x = -FOV;
		game->player->camera_dir_y = 0;
	}
	if (game->map->map[i][j] == 'E')
	{
		game->player->player_dir_x = 1;
		game->player->player_dir_y = 0;
		game->player->camera_dir_x = 0;
		game->player->camera_dir_y = FOV;
	}
	if (game->map->map[i][j] == 'W')
	{
		game->player->player_dir_x = -1;
		game->player->player_dir_y = 0;
		game->player->camera_dir_x = 0;
		game->player->camera_dir_y = -FOV;
	}
}

static void	init_player_direction(t_game *game)
{
	int	i;
	int	j;

	game->player->move_speed = 0.05;
	game->player->rotation_speed = 0.03;
	i = -1;
	while (i++, game->map->map[i])
	{
		j = -1;
		while (j++, game->map->map[i][j])
		{
			if (ft_strchr("NSEW", game->map->map[i][j]))
			{
				game->player->pos_x = j + 0.5;
				game->player->pos_y = i + 0.5;
				set_field_of_vision(game, i, j);
				game->player->pos = &game->map->map[i][j];
				return ;
			}
		}
	}
}

int32_t	init_cub3d(t_game *game)
{
	game->mlx = mlx_init(WIDTH, HEIGHT, W_NAME, true);
	init_map_images(game);
	init_player_direction(game);
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
