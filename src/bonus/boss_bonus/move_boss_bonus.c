/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_boss_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 20:17:05 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/14 20:22:34 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static bool	can_boss_move_utils(t_enemy_list *tmp, int x, int y)
{
	double			dx;
	double			dy;
	double			distance;

	while (tmp)
	{
		dx = x - tmp->pos_x;
		dy = y - tmp->pos_y;
		distance = sqrt(dx * dx + dy * dy);
		if (distance < 1)
			return (false);
		tmp = tmp->next;
	}
	return (true);
}

static bool	can_boss_move_to(t_game *game, double x, double y)
{
	t_enemy_list	*tmp;
	double			x_r;
	double			y_r;

	x_r = x;
	y_r = y;
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
	tmp = game->enemy->list;
	can_boss_move_utils(tmp, x_r, y_r);
	return (true);
}

static void	att_boss_position(t_game *game, double next_x, double next_y)
{
	game->boss->pos_x = next_x;
	game->boss->pos_y = next_y;
	game->boss->move_delay = 0;
}

static void	try_move_again(t_game *game, double dirx, double diry)
{
	double	alt_x;
	double	alt_y;

	alt_x = game->boss->pos_x + diry * game->boss->move_speed;
	alt_y = game->boss->pos_y - dirx * game->boss->move_speed;
	if (can_boss_move_to(game, alt_x, alt_y))
	{
		att_boss_position(game, alt_x, alt_y);
		return ;
	}
	alt_x = game->boss->pos_x - diry * game->boss->move_speed;
	alt_y = game->boss->pos_y + dirx * game->boss->move_speed;
	if (can_boss_move_to(game, alt_x, alt_y))
	{
		att_boss_position(game, alt_x, alt_y);
		return ;
	}
}

void	boss_move(t_game *game, double dx, double dy)
{
	double	inv_len;
	double	dirx;
	double	diry;
	double	next_x;
	double	next_y;

	inv_len = 1.0 / game->boss->distance;
	dirx = dx * inv_len;
	diry = dy * inv_len;
	next_x = game->boss->pos_x + dirx * game->boss->move_speed;
	next_y = game->boss->pos_y + diry * game->boss->move_speed;
	if (can_boss_move_to(game, next_x, next_y))
	{
		att_boss_position(game, next_x, next_y);
		return ;
	}
	else
		try_move_again(game, dirx, diry);
}
