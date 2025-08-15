/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_boss_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 20:23:26 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/14 20:23:42 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static void	calculate_boss_sprite(t_render *rend)
{
	double	dist_proj_plane;
	int		floor_height;
	int		floor_draw_end;
	double	scale_factor;

	dist_proj_plane = (WIDTH / 2.0) / tan(FOV / 2.0);
	floor_height = (int)((1.0 / rend->transform_y) * dist_proj_plane);
	floor_draw_end = (HEIGHT / 2 + floor_height / 2);
	scale_factor = 1.4;
	rend->sprite_height = (int)(rend->sprite_height * scale_factor);
	rend->draw_end_y = floor_draw_end;
	rend->draw_start_y = (-rend->sprite_height / 2 + HEIGHT / 2);
	if (rend->draw_start_y < 0)
		rend->draw_start_y = 0;
	if (rend->draw_end_y >= HEIGHT)
		rend->draw_end_y = HEIGHT - 1;
	rend->sprite_width = (int)fabs(HEIGHT / rend->transform_y);
	rend->sprite_width = (int)(rend->sprite_width * scale_factor);
	rend->draw_start_x = -rend->sprite_width / 2 + rend->sprite_screen_x;
	rend->original_start_x = rend->draw_start_x;
	if (rend->draw_start_x < 0)
		rend->draw_start_x = 0;
}

void	calculate_boss_position(t_game *game, double pos_x, double pos_y,
	mlx_texture_t *texture)
{
	t_render	*rend;
	double		inv_det;

	rend = ft_calloc(1, sizeof(t_render));
	rend->less_height = 2500;
	rend->sprite_x = pos_x - game->player->pos_x;
	rend->sprite_y = pos_y - game->player->pos_y;
	inv_det = 1.0 / (game->player->camera_dir_x * game->player->player_dir_y
			- game->player->player_dir_x * game->player->camera_dir_y);
	rend->transform_x = inv_det * (game->player->player_dir_y * rend->sprite_x
			- game->player->player_dir_x * rend->sprite_y);
	rend->transform_y = inv_det * (-game->player->camera_dir_y * rend->sprite_x
			+ game->player->camera_dir_x * rend->sprite_y);
	if (rend->transform_y <= 0)
		return ;
	rend->sprite_height = (int)fabs(HEIGHT / rend->transform_y);
	rend->sprite_screen_x = (int)((WIDTH / 2) * (1 + rend->transform_x
				/ rend->transform_y));
	calculate_boss_sprite(rend);
	put_x_on_screen(game, rend, texture);
	free(rend);
}
