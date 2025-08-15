/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fireball_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:25:21 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/14 20:50:44 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static void	calculate_min_distance(t_game *game, t_attack *fireball,
	t_render *rend)
{
	double	distance;
	double	visual_offset;

	rend->sprite_x = fireball->pos_x - game->player->pos_x;
	rend->sprite_y = fireball->pos_y - game->player->pos_y;
	distance = sqrt(rend->sprite_x * rend->sprite_x + rend->sprite_y
			* rend->sprite_y);
	if (distance < 0.4)
	{
		visual_offset = 0.4 - distance;
		rend->sprite_x += fireball->dir_x * visual_offset;
		rend->sprite_y += fireball->dir_y * visual_offset;
	}
}

void	calculate_fireball_position(t_game *game, t_attack *fireball,
	mlx_texture_t *texture)
{
	t_render	*rend;
	double		inv_det;

	rend = ft_calloc(1, sizeof(t_render));
	rend->less_height = 6000;
	calculate_min_distance(game, fireball, rend);
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
	calculate_sprite(rend);
	put_x_on_screen(game, rend, texture);
	free(rend);
}

static void	add_fireball_list(t_game *game, t_attack *fireball)
{
	t_attack	*last;

	if (game->fireballs == NULL)
		game->fireballs = fireball;
	else
	{
		last = game->fireballs;
		while (last->next)
			last = last->next;
		last->next = fireball;
	}
}

void	create_fireball(t_game *game)
{
	t_attack	*fireball;

	fireball = ft_calloc(1, sizeof(t_attack));
	fireball->pos_x = game->player->pos_x;
	fireball->pos_y = game->player->pos_y;
	fireball->dir_x = game->player->player_dir_x;
	fireball->dir_y = game->player->player_dir_y;
	fireball->state = MOVING;
	fireball->move_delay = 0.21;
	fireball->frame_delay = 0.0;
	fireball->current_frame = 0;
	add_fireball_list(game, fireball);
}
