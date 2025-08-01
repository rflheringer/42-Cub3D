/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ray.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 13:26:15 by rheringe          #+#    #+#             */
/*   Updated: 2025/08/01 13:34:53 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	calculate_ray_dir(t_game *game, int x)
{
	double	camera_x;

	camera_x = 2.0 * x / (double)WIDTH - 1;
	game->ray->ray_dir_x = game->player->player_dir_x
		+ game->player->camera_dir_x * camera_x;
	game->ray->ray_dir_y = game->player->player_dir_y
		+ game->player->camera_dir_y * camera_x;
	game->ray->map_x = (int)game->player->pos_x;
	game->ray->map_y = (int)game->player->pos_y;
	if (game->ray->ray_dir_x == 0)
		game->ray->delta_dist_x = 1e30;
	else
		game->ray->delta_dist_x = fabs(1 / game->ray->ray_dir_x);
	if (game->ray->ray_dir_y == 0)
		game->ray->delta_dist_y = 1e30;
	else
		game->ray->delta_dist_y = fabs(1 / game->ray->ray_dir_y);
	game->ray->hit = 0;
}

static void	calculate_step_x(t_game *game)
{
	if (game->ray->ray_dir_x < 0)
	{
		game->ray->step_x = -1;
		game->ray->side_dist_x = (game->player->pos_x
				- game->ray->map_x) * game->ray->delta_dist_x;
	}
	else
	{
		game->ray->step_x = 1;
		game->ray->side_dist_x = (game->ray->map_x + 1.0
				- game->player->pos_x) * game->ray->delta_dist_x;
	}
}

static void	calculate_step_y(t_game *game)
{
	if (game->ray->ray_dir_y < 0)
	{
		game->ray->step_y = -1;
		game->ray->side_dist_y = (game->player->pos_y
				- game->ray->map_y) * game->ray->delta_dist_y;
	}
	else
	{
		game->ray->step_y = 1;
		game->ray->side_dist_y = (game->ray->map_y + 1.0
				- game->player->pos_y) * game->ray->delta_dist_y;
	}
}

void	init_ray(t_game *game, int x)
{
	calculate_ray_dir(game, x);
	calculate_step_x(game);
	calculate_step_y(game);
}
