/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_fireball_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 20:44:10 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/14 20:53:46 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static bool	hit_boss(t_game *game, double x, double y)
{
	double	dx;
	double	dy;
	double	dist;

	dx = x - game->boss->pos_x;
	dy = y - game->boss->pos_y;
	dist = sqrt(dx * dx + dy * dy);
	if ((game->boss->state == ALERT || game->boss->state == ATTACK)
		&& dist < 0.3)
		return (true);
	return (false);
}

static bool	hit_enemie(t_game *game, double x, double y)
{
	t_enemy_list	*tmp;
	double			dx;
	double			dy;
	double			dist;

	tmp = game->enemy->list;
	while (tmp)
	{
		dx = x - tmp->pos_x;
		dy = y - tmp->pos_y;
		dist = sqrt(dx * dx + dy * dy);
		if ((tmp->state == ALERT || tmp->state == ATTACK) && dist < 0.3)
		{
			tmp->state = HITED;
			return (true);
		}
		tmp = tmp->next;
	}
	return (false);
}

static int	fireball_move(t_game *game, double x, double y)
{
	if (game->map->map[(int)floor(y + 0.1)][(int)floor(x + 0.1)] == '1'
		|| game->map->map[(int)floor(y + 0.1)][(int)floor(x + 0.1)] == 'D')
		return (1);
	if (game->map->map[(int)floor(y + 0.1)][(int)floor(x - 0.1)] == '1'
		|| game->map->map[(int)floor(y + 0.1)][(int)floor(x - 0.1)] == 'D')
		return (1);
	if (game->map->map[(int)floor(y - 0.1)][(int)floor(x + 0.1)] == '1'
		|| game->map->map[(int)floor(y - 0.1)][(int)floor(x + 0.1)] == 'D')
		return (1);
	if (game->map->map[(int)floor(y - 0.1)][(int)floor(x - 0.1)] == '1'
		|| game->map->map[(int)floor(y - 0.1)][(int)floor(x - 0.1)] == 'D')
		return (1);
	if (hit_enemie(game, x, y))
		return (2);
	if (hit_boss(game, x, y))
		return (3);
	return (0);
}

void	update_fireballs(t_game *game)
{
	double		next_x;
	double		next_y;
	t_attack	*nav;

	nav = game->fireballs;
	while (nav)
	{
		nav->move_delay += game->delta_time;
		if (nav->state == MOVING && nav->move_delay > 0.2)
		{
			next_x = nav->pos_x + nav->dir_x * 0.5;
			next_y = nav->pos_y + nav->dir_y * 0.5;
			nav->next_tile = fireball_move(game, next_x, next_y);
			if (nav->next_tile > 0)
				nav->state = HITED;
			else
			{
				nav->pos_x = next_x;
				nav->pos_y = next_y;
			}
		}
		render_fireballs(game, nav);
		nav = nav->next;
	}
}
