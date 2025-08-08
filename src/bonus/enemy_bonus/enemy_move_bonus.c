/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_move_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:37:18 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/08 11:43:34 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../../include/cub3d_bonus.h"

static void	enemy_move(t_game *game, t_enemy_list *enemy, double dx, double dy)
{
	double	inv_len;
	double	dirx;
	double	diry;
	double	next_x;
	double	next_y;

	inv_len = 1.0 / enemy->distance;
	dirx = dx * inv_len;
	diry = dy * inv_len;
	next_x = enemy->pos_x + dirx * game->enemy->move_speed;
	next_y = enemy->pos_y + diry * game->enemy->move_speed;
	if (!can_move_to(game->map->map, next_x, next_y))
		return ;
	enemy->pos_x = next_x;
	enemy->pos_y = next_y;
}

static void	calculate_distance_to_player(t_game *game, t_enemy_list *enemy)
{
	double	px;
	double	py;
	double	dx;
	double	dy;

	enemy->move_delay += game->delta_time;
	px = game->player->pos_x;
	py = game->player->pos_y;
	dx = px - enemy->pos_x;
	dy = py - enemy->pos_y;
	enemy->distance = sqrt(dx * dx + dy * dy);
	if (enemy->distance > 0.6 && enemy->move_delay > 2.0)
		enemy_move(game, enemy, dx, dy);
}

static void	calculate_sprite_change(t_game *game, t_enemy_list *enemy)
{
	enemy->frame_delay += game->delta_time;
	if (enemy->frame_delay > 0.3)
	{
		enemy->cur_sprite = (enemy->cur_sprite + 1) % 3;
		enemy->frame_delay = 0;
	}
}

void	manage_enemies(t_game *game)
{
	t_enemy_list	*nav;

	if (game->enemy->list == NULL)
		return ;
	nav = game->enemy->list;
	while (nav)
	{
		calculate_distance_to_player(game, nav);
		calculate_sprite_change(game, nav);
		calculate_enemie_position(game, nav);
		nav = nav->next;
	}
}
