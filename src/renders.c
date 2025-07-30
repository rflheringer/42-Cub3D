/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renders.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:44:12 by rheringe          #+#    #+#             */
/*   Updated: 2025/07/30 13:22:21 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	render_walls(t_game *game, int i, int j)
{
	int	tile_size;

	tile_size = 64;
	if (game->map->map[i][j] == '1')
		mlx_image_to_window(game->mlx, game->image->south_wall,
			j * tile_size, i * tile_size);
}

void	load_screen(t_game *game)
{
	int	i;
	int	j;

	i = -1;
	while (i++, game->map->map[i])
	{
		j = -1;
		while (j++, game->map->map[i][j])
		{
			if (game->map->map[i][j] == '1')
				render_walls(game, i, j);
		}
	}
}
