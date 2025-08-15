/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_bullet_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:25:21 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/15 17:06:19 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

void	calculate_bullet_position(t_game *game, t_attack *bullet,
	mlx_texture_t *texture)
{
	t_render	*rend;
	double		inv_det;

	rend = ft_calloc(1, sizeof(t_render));
	rend->less_height = 2000;
	rend->sprite_x = bullet->pos_x - game->player->pos_x;
	rend->sprite_y = bullet->pos_y - game->player->pos_y;
	inv_det = 1.0 / (game->player->camera_dir_x * game->player->player_dir_y
			- game->player->player_dir_x * game->player->camera_dir_y);
	rend->transform_x = inv_det * (game->player->player_dir_y * rend->sprite_x
			- game->player->player_dir_x * rend->sprite_y);
	rend->transform_y = inv_det * (-game->player->camera_dir_y * rend->sprite_x
			+ game->player->camera_dir_x * rend->sprite_y);
	if (rend->transform_y <= 0)
	{
		free(rend);
		return ;
	}
	rend->sprite_height = (int)fabs(HEIGHT / rend->transform_y);
	rend->sprite_screen_x = (int)((WIDTH / 2) * (1 + rend->transform_x
				/ rend->transform_y));
	calculate_sprite(rend);
	put_x_on_screen(game, rend, texture);
	free(rend);
}
