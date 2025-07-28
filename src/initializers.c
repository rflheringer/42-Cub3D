/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:41:18 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/07/28 13:02:53 by rheringe         ###   ########.fr       */
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
}

static void	set_direction(t_game *game, int i, int j)
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
	game->player->move_speed = 0.05;
	game->player->rotation_speed = 0.03;
	
	int i;
	i = -1;
	while (i++, game->map->map[i])
	{
		int j;
		j = -1;
		while(j++, game->map->map[i][j])
		{
			if (ft_strchr("NSEW", game->map->map[i][j]))
			{
				game->player->pos_x = i;
				game->player->pos_y = j;
				set_direction(game, i, j);
				game->player->pos = &game->map->map[i][j];
				return;
			}
		}
	}
}

int32_t	init_cub3d(t_game *game)
{
	game->mlx = mlx_init(WIDTH, HEIGHT, W_NAME, true);
	init_map_images(game);
	init_player_direction(game);
	load_screen(game);
	mlx_loop(game->mlx);
	return (0);
}
