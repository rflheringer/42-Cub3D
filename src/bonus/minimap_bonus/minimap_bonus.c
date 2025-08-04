/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:26:34 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/04 15:32:08 by rdel-fra         ###   ########.fr       */
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
			mlx_put_pixel(game->raycasting->image, j, i, 0xFF0000);
			if (game->map->map[i][j] == '1')
				mlx_put_pixel(game->raycasting->image, j, i, 0xFF00F0);
			if (game->map->map[i][j] == '0')
				mlx_put_pixel(game->raycasting->image, j, i, 0x000000);
			if (ft_strchr("NSWE", game->map->map[i][j]))
				mlx_put_pixel(game->raycasting->image, j, i, 0xFFFF00);
			j++;
		}
		i++;
	}
}

void	init_minimap(t_game *game)
{
	int	i;
	int	j;

	i = 0;
	game->raycasting->minimap = mlx_new_image(game->mlx, 200, 200);
	while (game->map->map[i])
	{
		j = 0;
		while (game->map->map[i][j])
		{
			mlx_put_pixel(game->raycasting->image, j, i, 0xa8a8ff);
			if (game->map->map[i][j] == '1')
				mlx_put_pixel(game->raycasting->image, j, i, 0xFF00F0);
			if (game->map->map[i][j] == '0')
				mlx_put_pixel(game->raycasting->image, j, i, 0x545454ff);
			if (ft_strchr("NSWE", game->map->map[i][j]))
				mlx_put_pixel(game->raycasting->image, j, i, 0xFF0000);
			j++;
		}
		i++;
	}
	mlx_image_to_window(game->mlx, game->raycasting->minimap, 100, 100);
}
