/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_bullet_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 20:35:47 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/14 20:35:59 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static void	add_bullet_list(t_game *game, t_attack *bullet)
{
	t_attack	*last;

	if (game->bullets == NULL)
		game->bullets = bullet;
	else
	{
		last = game->bullets;
		while (last->next)
			last = last->next;
		last->next = bullet;
	}
}

void	fire_projectile(t_game *game)
{
	t_attack	*bullet;
	double		inv_len;
	double		dx;
	double		dy;

	bullet = ft_calloc(1, sizeof(t_attack));
	dx = game->player->pos_x - game->boss->pos_x;
	dy = game->player->pos_y - game->boss->pos_y;
	inv_len = 1.0 / game->boss->distance;
	bullet->pos_x = game->boss->pos_x;
	bullet->pos_y = game->boss->pos_y;
	bullet->dir_x = dx * inv_len;
	bullet->dir_y = dy * inv_len;
	bullet->state = MOVING;
	bullet->move_delay = 0.11;
	bullet->frame_delay = 0.0;
	bullet->current_frame = 0;
	add_bullet_list(game, bullet);
}
