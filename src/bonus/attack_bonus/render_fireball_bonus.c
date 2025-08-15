/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_fireball_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 20:45:14 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/15 17:56:50 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static t_enemy_list	*find_enemy_hited(t_game *game)
{
	t_enemy_list	*nav;

	nav = game->enemy->list;
	while (nav)
	{
		if (nav->state == HITED)
			return (nav);
		nav = nav->next;
	}
	return (NULL);
}

static void	render_wall_hit(t_game *game, t_attack *fireball)
{
	fireball->frame_delay += game->delta_time;
	if (fireball->frame_delay > 0.1)
	{
		fireball->current_frame = (fireball->current_frame + 1) % 4;
		fireball->frame_delay = 0;
		if (fireball->current_frame == 0)
		{
			fireball->state = DEAD;
			return ;
		}
	}
	calculate_fireball_position(game, fireball,
		game->player->fireball_textures[fireball->current_frame]);
}

static void	render_enemy_hit(t_game *game, t_attack *fireball)
{
	t_enemy_list	*enemy;

	fireball->frame_delay += game->delta_time;
	enemy = find_enemy_hited(game);
	if (enemy)
		enemy->state = DYING;
	if (fireball->frame_delay > 0.1)
	{
		fireball->current_frame = (fireball->current_frame + 1) % 4;
		fireball->frame_delay = 0;
		if (fireball->current_frame == 0)
		{
			fireball->state = DEAD;
			return ;
		}
	}
	calculate_fireball_position(game, fireball,
		game->player->fireball_textures[fireball->current_frame]);
}

static void	render_boss_hit(t_game *game, t_attack *fireball)
{
	fireball->frame_delay += game->delta_time;
	if (fireball->frame_delay > 0.1)
	{
		fireball->current_frame = (fireball->current_frame + 1) % 4;
		fireball->frame_delay = 0;
		if (fireball->current_frame == 0)
		{
			fireball->state = DEAD;
			return ;
		}
	}
	calculate_fireball_position(game, fireball,
		game->player->fireball_textures[fireball->current_frame]);
}

void	render_fireballs(t_game *game, t_attack *fireball)
{
	if (fireball->state == DEAD)
		return ;
	if (fireball->next_tile == 1)
		render_wall_hit(game, fireball);
	else if (fireball->next_tile == 2)
		render_enemy_hit(game, fireball);
	else if (fireball->next_tile == 3)
	{
		render_boss_hit(game, fireball);
		if (fireball->state == HITED)
		{
			fireball->state = DAMAGE;
			game->boss->hp -= 10;
		}
		if (game->boss->hp <= 0 && game->boss->state != DYING)
		{
			game->boss->hp = 0;
			game->boss->state = DYING;
		}
	}
	else if (fireball->state == MOVING)
		calculate_fireball_position(game, fireball,
			game->player->fireball_textures[fireball->current_frame]);
}
