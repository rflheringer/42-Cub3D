/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:26:34 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/08 20:11:57 by rdel-fra         ###   ########.fr       */
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
		sx = -1;
		while (sx++ < scale - 1)
		{
			x = 10 + (j * scale) + sx;
			y = 10 + (i * scale) + sy;
			if (x < WIDTH && y < HEIGHT)
			{
				if (game->map->map[i][j] != ' ')
					mlx_put_pixel(game->raycasting->image, x, y, 0x000000FF);
				if (game->map->map[i][j] == '1')
					mlx_put_pixel(game->raycasting->image, x, y, 0x00000000);
				if (game->map->map[i][j] == 'D' || game->map->map[i][j] == 'O')
					mlx_put_pixel(game->raycasting->image, x, y, 0xFFFF00FF);
			}
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
				mlx_put_pixel(game->raycasting->image, x, y, 0x00FF00FF);
			sx++;
		}
		sy++;
	}
}

static void	draw_enemies_on_screen(t_game *game, t_enemy_list *enemy, int scale)
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
			x = 10 + ((int)enemy->pos_x * scale) + sx;
			y = 10 + ((int)enemy->pos_y * scale) + sy;
			if (x < WIDTH && y < HEIGHT)
				mlx_put_pixel(game->raycasting->image, x, y, 0xFF0000FF);
			sx++;
		}
		sy++;
	}
}

static void	draw_enemies(t_game *game, int scale)
{
	t_enemy_list	*enemy;

	enemy = game->enemy->list;
	if (!enemy)
		return ;
	while (enemy)
	{
		draw_enemies_on_screen(game, enemy, scale);
		enemy = enemy->next;
	}
}

void	update_minimap(t_game *game)
{
	int	scale;
	int	i;
	int	j;

	scale = game->map->height * 0.10;
	if (scale > 5)
		scale = 2;
	else
		scale = 5;
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
	draw_enemies(game, scale);
	draw_player_on_screen(game, scale);
}
