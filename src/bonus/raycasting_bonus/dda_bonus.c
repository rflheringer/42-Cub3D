/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 13:18:05 by rheringe          #+#    #+#             */
/*   Updated: 2025/08/08 19:40:59 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static void	calculate_perp_dist(t_game *game, int x)
{
	if (game->ray->side == 0 && game->ray->ray_dir_x != 0)
		game->ray->perp_wall_dist = fabs((game->ray->map_x
					- game->player->pos_x + (1 - game->ray->step_x) / 2)
				/ game->ray->ray_dir_x);
	else if (game->ray->ray_dir_y != 0)
		game->ray->perp_wall_dist = fabs((game->ray->map_y
					- game->player->pos_y + (1 - game->ray->step_y) / 2)
				/ game->ray->ray_dir_y);
	game->raycasting->buffer[x] = game->ray->perp_wall_dist;
}

static int	check_ray_hit(t_game *game)
{
	int	map_height;
	int	map_width;

	if (game->ray->map_y < 0 || game->ray->map_x < 0)
		return (1);
	map_height = ft_ptrlen(game->map->map);
	if (game->ray->map_y >= map_height)
		return (1);
	map_width = ft_strlen(game->map->map[game->ray->map_y]);
	if (game->ray->map_x >= map_width)
		return (1);
	if (game->map->map[game->ray->map_y][game->ray->map_x] == 'D')
		return (2);
	if (game->map->map[game->ray->map_y][game->ray->map_x] == '1')
		return (1);
	return (0);
}

void	perform_dda(t_game *game, int x)
{
	int	max_iterations;
	int	iterations;

	max_iterations = 100;
	iterations = 0;
	while (game->ray->hit == 0 && iterations < max_iterations)
	{
		if (game->ray->side_dist_x < game->ray->side_dist_y)
		{
			game->ray->side_dist_x += game->ray->delta_dist_x;
			game->ray->map_x += game->ray->step_x;
			game->ray->side = 0;
		}
		else
		{
			game->ray->side_dist_y += game->ray->delta_dist_y;
			game->ray->map_y += game->ray->step_y;
			game->ray->side = 1;
		}
		iterations++;
		game->ray->hit = check_ray_hit(game);
	}
	calculate_perp_dist(game, x);
}
