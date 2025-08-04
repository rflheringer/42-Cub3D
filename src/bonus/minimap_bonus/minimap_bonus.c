/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:26:34 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/04 16:40:48 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static void	draw_minimap_on_screen(t_game *game, int i, int j, int scale)
{
	int	x;
	int	y;
	int	sx;
	int	sy;

	sy = 0;
	while (sy < scale)
	{
		sx = 0;
		while (sx < scale)
		{
			x = 10 + (j * scale) + sx;
			y = 10 + (i * scale) + sy;
			if (x < WIDTH && y < HEIGHT)
			{
				if (game->map->map[i][j] != ' ')
					mlx_put_pixel(game->raycasting->image, x, y, 0x000000FF);
				if (game->map->map[i][j] == '1')
					mlx_put_pixel(game->raycasting->image, x, y, 0x00000000);
			}
			sx++;
		}
		sy++;
	}
}

static void	draw_player_on_screen(t_game *game, int scale)
{
	int	x;
	int	y;
	int	sx;
	int	sy;

	sy = 0;
	while (sy < scale)
	{
		sx = 0;
		while (sx < scale)
		{
			x = 10 + ((int)game->player->pos_x * scale) + sx;
			y = 10 + ((int)game->player->pos_y * scale) + sy;
			if (x < WIDTH && y < HEIGHT)
				mlx_put_pixel(game->raycasting->image, x, y, 0xFF0000FF);
			sx++;
		}
		sy++;
	}
}

void	update_minimap(t_game *game)
{
	int	scale;
	int	i;
	int	j;

	scale = 6;
	i = 0;
	while (game->map->map[i])
	{
		j = 0;
		while (game->map->map[i][j])
		{
			draw_minimap_on_screen(game, i, j, scale);
			j++;
		}
		i++;
	}
	draw_player_on_screen(game, scale);
}
