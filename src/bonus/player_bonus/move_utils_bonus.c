/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_utils_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:37:21 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/14 20:02:17 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static bool	calculate_dist_to_enemy(t_enemy_list *enemy, double x, double y)
{
	double	distance;
	double	dx;
	double	dy;

	if (enemy->state == DEAD)
		return (true);
	dx = x - enemy->pos_x;
	dy = y - enemy->pos_y;
	distance = sqrt(dx * dx + dy * dy);
	if (distance > 0.6)
		return (true);
	return (false);
}

bool	right_move(t_game *game)
{
	double	new_x;
	double	new_y;

	new_x = game->player->pos_x + game->player->player_dir_y
		* game->player->move_speed;
	new_y = game->player->pos_y - game->player->player_dir_x
		* game->player->move_speed;
	if (can_move_to(game, new_x, new_y, game->enemy->list))
	{
		check_key_and_potion(game, new_x, new_y);
		game->player->old_x = game->player->pos_x;
		game->player->old_y = game->player->pos_y;
		game->player->pos_x = new_x;
		game->player->pos_y = new_y;
		return (true);
	}
	return (false);
}

bool	left_move(t_game *game)
{
	double	new_x;
	double	new_y;

	new_x = game->player->pos_x - game->player->player_dir_y
		* game->player->move_speed;
	new_y = game->player->pos_y + game->player->player_dir_x
		* game->player->move_speed;
	if (can_move_to(game, new_x, new_y, game->enemy->list))
	{
		check_key_and_potion(game, new_x, new_y);
		game->player->old_x = game->player->pos_x;
		game->player->old_y = game->player->pos_y;
		game->player->pos_x = new_x;
		game->player->pos_y = new_y;
		return (true);
	}
	return (false);
}

static bool	dist_to_boss(t_game *game, double x, double y)
{
	double	boss_dx;
	double	boss_dy;
	double	boss_dist;

	if (game->boss)
	{
		boss_dx = x - game->boss->pos_x;
		boss_dy = y - game->boss->pos_y;
		boss_dist = sqrt(boss_dx * boss_dx + boss_dy * boss_dy);
		if (boss_dist < 0.6)
			return (false);
	}
	return (true);
}

bool	can_move_to(t_game *game, double x, double y, t_enemy_list *enemy_list)
{
	t_enemy_list	*tmp;

	if (game->map->map[(int)floor(y + R)][(int)floor(x + R)] == '1'
		|| game->map->map[(int)floor(y + R)][(int)floor(x + R)] == 'D')
		return (false);
	if (game->map->map[(int)floor(y + R)][(int)floor(x - R)] == '1'
		|| game->map->map[(int)floor(y + R)][(int)floor(x - R)] == 'D')
		return (false);
	if (game->map->map[(int)floor(y - R)][(int)floor(x + R)] == '1'
		|| game->map->map[(int)floor(y - R)][(int)floor(x + R)] == 'D')
		return (false);
	if (game->map->map[(int)floor(y - R)][(int)floor(x - R)] == '1'
		|| game->map->map[(int)floor(y - R)][(int)floor(x - R)] == 'D')
		return (false);
	tmp = enemy_list;
	while (tmp)
	{
		if ((tmp->state == ALERT || tmp->state == ATTACK)
			&& !calculate_dist_to_enemy(tmp, x, y))
			return (false);
		tmp = tmp->next;
	}
	if (!dist_to_boss(game, x, y))
		return (false);
	return (true);
}
