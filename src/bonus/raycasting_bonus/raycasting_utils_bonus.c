/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_utils_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 11:53:54 by rheringe          #+#    #+#             */
/*   Updated: 2025/08/08 18:48:16 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static void	calculate_wall_dimensions(t_game *game)
{
	double	dist_proj_plane;

	if (game->ray->perp_wall_dist < 0.01)
		game->ray->perp_wall_dist = 0.01;
	dist_proj_plane = (WIDTH / 2.0) / tan(FOV / 2.0);
	game->wall->height = (int)((1.0 / game->ray->perp_wall_dist)
			* dist_proj_plane);
	game->wall->draw_start = HEIGHT / 2 - game->wall->height / 2;
	if (game->wall->draw_start < 0)
		game->wall->draw_start = 0;
	game->wall->draw_end = HEIGHT / 2 + game->wall->height / 2;
	if (game->wall->draw_end >= HEIGHT)
		game->wall->draw_end = HEIGHT - 1;
}

static void	select_wall_texture(t_game *game)
{
	if (game->ray->hit == 2)
		game->wall->s_texture = game->texture->closed_door;
	else if (game->ray->side == 0)
	{
		if (game->ray->ray_dir_x > 0)
			game->wall->s_texture = game->texture->east_wall;
		else
			game->wall->s_texture = game->texture->west_wall;
	}
	else
	{
		if (game->ray->ray_dir_y > 0)
			game->wall->s_texture = game->texture->south_wall;
		else
			game->wall->s_texture = game->texture->north_wall;
	}
}

static void	calculate_texture_x(t_game *game)
{
	double	wall_x;

	if (game->ray->side == 0)
		wall_x = game->player->pos_y + game->ray->perp_wall_dist
			* game->ray->ray_dir_y;
	else
		wall_x = game->player->pos_x + game->ray->perp_wall_dist
			* game->ray->ray_dir_x;
	wall_x -= floor(wall_x);
	game->wall->text_x = (int)(wall_x
			* game->wall->s_texture->width);
	if ((game->ray->side == 0 && game->ray->ray_dir_x < 0)
		|| (game->ray->side == 1 && game->ray->ray_dir_y < 0))
		game->wall->text_x = game->wall->s_texture->width
			- game->wall->text_x - 1;
}

void	calculate_wall(t_game *game)
{
	calculate_wall_dimensions(game);
	select_wall_texture(game);
	calculate_texture_x(game);
}
