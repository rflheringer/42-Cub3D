/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:26:34 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/04 16:06:17 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

void	update_minimap(t_game *game)
{
	int	i;
	int	j;

	i = 0;
	while (game->map->map[i])
	{
		j = 0;
		while (game->map->map[i][j])
		{
			mlx_put_pixel(game->raycasting->image, j, i, 0x000000FF);
			if (game->map->map[i][j] == '1')
				mlx_put_pixel(game->raycasting->image, j, i, 0xFF00F0FF);
			j++;
		}
		i++;
	}
	mlx_put_pixel(game->raycasting->image, (int)game->player->pos_x, (int)game->player->pos_y, 0xFF0000FF);
}

void	init_minimap(t_game *game)
{
	int	i;
	int	j;

	i = 0;
	while (game->map->map[i])
	{
		j = 0;
		while (game->map->map[i][j])
		{
			mlx_put_pixel(game->raycasting->image, j, i, 0x000000FF);
			if (game->map->map[i][j] == '1')
				mlx_put_pixel(game->raycasting->image, j, i, 0xFF00F0FF);
			if (ft_strchr("NSWE", game->map->map[i][j]))
				mlx_put_pixel(game->raycasting->image, (int)game->player->pos_x, (int)game->player->pos_y, 0xFF0000FF);
			j++;
		}
		i++;
	}

}
