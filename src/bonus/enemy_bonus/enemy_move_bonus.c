/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_move_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:37:18 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/05 18:17:00 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../../include/cub3d_bonus.h"

static bool	validate_moves(t_game *game, t_enemy_list *enemy, double next_x,
		double next_y)
{
	if (game->map->map[(int)enemy->pos_y][(int)next_x] == '1')
		return (false);
	if (game->map->map[(int)next_y][(int)enemy->pos_x] == '1')
		return (false);
	return (true);
}

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
	if (!validate_moves(game, enemy, next_x, next_y))
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
	if (enemy->distance > 0.4 && enemy->move_delay > 0.4)
		enemy_move(game, enemy, dx, dy);
}

void	manage_enemies(t_game *game)
{
	t_enemy_list	*nav;

	if (!game->enemy->list)
		return ;
	nav = game->enemy->list;
	while (nav)
	{
		calculate_distance_to_player(game, nav);
		nav = nav->next;
	}
}
