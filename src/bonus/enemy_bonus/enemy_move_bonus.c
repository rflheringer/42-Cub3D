/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_move_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 18:45:12 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/15 18:40:05 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static bool	check_enemies(t_game *game, double x, double y, t_enemy_list *cur)
{
	double			dx;
	double			dy;
	double			distance;
	t_enemy_list	*tmp;

	tmp = game->enemy->list;
	while (tmp)
	{
		if (tmp != cur && tmp->state != DEAD)
		{
			dx = x - tmp->pos_x;
			dy = y - tmp->pos_y;
			distance = sqrt(dx * dx + dy * dy);
			if (distance < 0.6)
				return (false);
		}
		tmp = tmp->next;
	}
	return (true);
}

static bool	can_enemy_move(t_game *game, double x, double y, t_enemy_list *cur)
{
	if (game->map->map[(int)floor(y + 0.2)][(int)floor(x + 0.2)] == '1'
		|| game->map->map[(int)floor(y + 0.2)][(int)floor(x + 0.2)] == 'D')
		return (false);
	if (game->map->map[(int)floor(y + 0.2)][(int)floor(x - 0.2)] == '1'
		|| game->map->map[(int)floor(y + 0.2)][(int)floor(x - 0.2)] == 'D')
		return (false);
	if (game->map->map[(int)floor(y - 0.2)][(int)floor(x + 0.2)] == '1'
		|| game->map->map[(int)floor(y - 0.2)][(int)floor(x + 0.2)] == 'D')
		return (false);
	if (game->map->map[(int)floor(y - 0.2)][(int)floor(x - 0.2)] == '1'
		|| game->map->map[(int)floor(y - 0.2)][(int)floor(x - 0.2)] == 'D')
		return (false);
	if (!check_enemies(game, x, y, cur))
		return (false);
	return (true);
}

static void	att_enemy_pos(double next_x, double next_y, t_enemy_list *enemy)
{
	enemy->pos_x = next_x;
	enemy->pos_y = next_y;
	enemy->move_delay = 0;
}

static void	try_move_again(t_game *game, double dirx, double diry,
	t_enemy_list *enemy)
{
	double	alt_x;
	double	alt_y;

	alt_x = enemy->pos_x + diry * game->enemy->move_speed;
	alt_y = enemy->pos_y - dirx * game->enemy->move_speed;
	if (can_enemy_move(game, alt_x, alt_y, enemy))
	{
		att_enemy_pos(alt_x, alt_y, enemy);
		return ;
	}
	alt_x = enemy->pos_x - diry * game->enemy->move_speed;
	alt_y = enemy->pos_y + dirx * game->enemy->move_speed;
	if (can_enemy_move(game, alt_x, alt_y, enemy))
	{
		att_enemy_pos(alt_x, alt_y, enemy);
		return ;
	}
}

void	enemy_move(t_game *game, t_enemy_list *enemy, double dx, double dy)
{
	double	inv_len;
	double	dirx;
	double	diry;
	double	next_x;
	double	next_y;

	if (enemy->state == DEAD)
		return ;
	inv_len = 1.0 / enemy->distance;
	dirx = dx * inv_len;
	diry = dy * inv_len;
	next_x = enemy->pos_x + dirx * game->enemy->move_speed;
	next_y = enemy->pos_y + diry * game->enemy->move_speed;
	if (can_enemy_move(game, next_x, next_y, enemy))
		att_enemy_pos(next_x, next_y, enemy);
	else
		try_move_again(game, dirx, diry, enemy);
}
