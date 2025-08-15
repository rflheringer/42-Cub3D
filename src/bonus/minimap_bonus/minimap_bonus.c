/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:26:34 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/15 15:44:13 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static void	draw_minimap(t_game *game, int i, int j, int sy)
{
	int	x;
	int	y;
	int	sx;

	sx = 0;
	while (sx < game->raycasting->scale)
	{
		x = 5 + (j * game->raycasting->scale) + sx;
		y = 5 + (i * game->raycasting->scale) + sy;
		if (x < WIDTH && y < HEIGHT)
		{
			if (game->map->map[i][j] != ' ')
				mlx_put_pixel(game->raycasting->image, x, y, 0x000000FF);
			if (game->map->map[i][j] == '1')
				mlx_put_pixel(game->raycasting->image, x, y, 0x00000000);
			if (game->map->map[i][j] == 'D' || game->map->map[i][j] == 'O')
				mlx_put_pixel(game->raycasting->image, x, y, 0xFFFF00FF);
			if (game->map->map[i][j] == 'P')
				mlx_put_pixel(game->raycasting->image, x, y, 0xFFC0CBFF);
			if (game->map->map[i][j] == 'K')
				mlx_put_pixel(game->raycasting->image, x, y, 0x00FFFFFF);
		}
		sx++;
	}
}

static void	draw_minimap_on_screen(t_game *game, int i, int j)
{
	int	sy;

	sy = 0;
	while (sy < game->raycasting->scale)
	{
		draw_minimap(game, i, j, sy);
		sy++;
	}
}

static void	draw_player_on_screen(t_game *game)
{
	int	x;
	int	y;
	int	sx;
	int	sy;

	sy = 0;
	while (sy < game->raycasting->scale)
	{
		sx = 0;
		while (sx < game->raycasting->scale)
		{
			x = 5 + ((int)game->player->pos_x * game->raycasting->scale) + sx;
			y = 5 + ((int)game->player->pos_y * game->raycasting->scale) + sy;
			if (x < WIDTH && y < HEIGHT)
				mlx_put_pixel(game->raycasting->image, x, y, 0x00FF00FF);
			sx++;
		}
		sy++;
	}
}

static void	draw_boss_on_screen(t_game *game)
{
	int	x;
	int	y;
	int	sx;
	int	sy;

	if (game->boss == NULL)
		return ;
	sy = 0;
	while (sy < game->raycasting->scale)
	{
		sx = 0;
		while (sx < game->raycasting->scale)
		{
			x = 5 + ((int)game->boss->pos_x * game->raycasting->scale) + sx;
			y = 5 + ((int)game->boss->pos_y * game->raycasting->scale) + sy;
			if (x < WIDTH && y < HEIGHT)
				mlx_put_pixel(game->raycasting->image, x, y, 0xA020F0FF);
			sx++;
		}
		sy++;
	}
}

void	update_minimap(t_game *game)
{
	int	i;
	int	j;

	game->raycasting->scale = game->map->height * 0.10;
	if (game->raycasting->scale > 5)
		game->raycasting->scale = 2;
	else
		game->raycasting->scale = 5;
	i = 0;
	while (game->map->map[i])
	{
		j = 0;
		while (game->map->map[i][j])
		{
			draw_minimap_on_screen(game, i, j);
			j++;
		}
		i++;
	}
	draw_enemies(game);
	draw_boss_on_screen(game);
	draw_player_on_screen(game);
	calculate_player_fov(game, game->raycasting->scale);
}
