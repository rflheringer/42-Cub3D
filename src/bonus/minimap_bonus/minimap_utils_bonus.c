/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_utils_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 12:53:26 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/15 18:39:28 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static void	draw_enemies_on_screen(t_game *game, t_enemy_list *enemy)
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
			x = 5 + ((int)enemy->pos_x * game->raycasting->scale) + sx;
			y = 5 + ((int)enemy->pos_y * game->raycasting->scale) + sy;
			if (x < WIDTH && y < HEIGHT)
				mlx_put_pixel(game->raycasting->image, x, y, 0xFF0000FF);
			sx++;
		}
		sy++;
	}
}

void	draw_enemies(t_game *game)
{
	t_enemy_list	*enemy;

	enemy = game->enemy->list;
	if (!enemy)
		return ;
	while (enemy)
	{
		if (enemy->state != DEAD)
			draw_enemies_on_screen(game, enemy);
		enemy = enemy->next;
	}
}
