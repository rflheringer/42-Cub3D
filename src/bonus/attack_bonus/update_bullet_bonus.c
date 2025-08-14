/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_bullet_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 20:36:40 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/14 20:36:49 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static int	bullet_move(t_game *game, double x, double y)
{
	double	dx;
	double	dy;
	double	distance;

	dx = x - game->player->pos_x;
	dy = y - game->player->pos_y;
	distance = sqrt(dx * dx + dy * dy);
	if (distance < 0.3)
	{
		game->boss->hit_player = true;
		return (2);
	}
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
	return (0);
}

static void	render_wall_hit(t_game *game, t_attack *bullet)
{
	bullet->frame_delay += game->delta_time;
	if (bullet->frame_delay > 0.1)
	{
		bullet->current_frame = (bullet->current_frame + 1) % 4;
		bullet->frame_delay = 0;
		if (bullet->current_frame == 0)
		{
			bullet->state = DEAD;
			return ;
		}
	}
	calculate_bullet_position(game, bullet);
}

static void	render_player_hit(t_game *game, t_attack *bullet)
{
	bullet->frame_delay += game->delta_time;
	if (bullet->frame_delay > 0.05)
	{
		bullet->current_frame = (bullet->current_frame + 1) % 4;
		bullet->frame_delay = 0;
		if (bullet->current_frame == 0)
		{
			bullet->state = DEAD;
			return ;
		}
	}
	calculate_bullet_position(game, bullet);
}

static void	render_bullets(t_game *game, t_attack *bullet)
{
	if (bullet->state == DEAD)
		return ;
	if (bullet->next_tile == 1)
		render_wall_hit(game, bullet);
	else if (bullet->next_tile == 2)
		render_player_hit(game, bullet);
	else if (bullet->state == MOVING)
		calculate_bullet_position(game, bullet);
}

void	update_bullets(t_game *game)
{
	double		next_x;
	double		next_y;
	t_attack	*nav;

	nav = game->bullets;
	while (nav)
	{
		nav->move_delay += game->delta_time;
		if (nav->state == MOVING && nav->move_delay > 0.2)
		{
			next_x = nav->pos_x + nav->dir_x * 0.5;
			next_y = nav->pos_y + nav->dir_y * 0.5;
			nav->next_tile = bullet_move(game, next_x, next_y);
			if (nav->next_tile > 0)
				nav->state = HITED;
			else
			{
				nav->pos_x = next_x;
				nav->pos_y = next_y;
			}
		}
		render_bullets(game, nav);
		nav = nav->next;
	}
}
