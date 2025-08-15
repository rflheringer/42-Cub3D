/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_boss_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:27:59 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/14 20:30:35 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static void	calculate_distance_to_player(t_game *game)
{
	double	px;
	double	py;
	double	dx;
	double	dy;

	game->boss->move_delay += game->delta_time;
	game->boss->attack_delay += game->delta_time;
	px = game->player->pos_x;
	py = game->player->pos_y;
	dx = px - game->boss->pos_x;
	dy = py - game->boss->pos_y;
	game->boss->distance = sqrt(dx * dx + dy * dy);
	if (game->boss->state == DYING)
		return ;
	if (game->boss->distance <= 16.0 && game->boss->attack_delay > 0.5)
		game->boss->state = ATTACK;
	else if (game->boss->distance <= 18.0)
		game->boss->state = ALERT;
	if (game->boss->state == ALERT && game->boss->distance > 8.0
		&& game->boss->move_delay > 0.1)
		boss_move(game, dx, dy);
	else if (game->boss->state == ALERT && game->boss->distance <= 16.0
		&& game->boss->attack_delay > 0.5)
		game->boss->state = ATTACK;
}

static void	boss_attack(t_game *game)
{
	game->boss->attack_sprite++;
	game->boss->frame_delay = 0;
	if (game->boss->attack_sprite > 6)
	{
		fire_projectile(game);
		game->boss->state = IDLE;
		game->boss->attack_delay = 1;
		game->boss->attack_sprite = 3;
	}
}

static void	calculate_sprite_change(t_game *game)
{
	if (game->boss->state == ALERT || game->boss->state == ATTACK)
		game->boss->frame_delay += game->delta_time;
	else if (game->boss->state == DYING)
		game->boss->death_delay += game->delta_time;
	if (game->boss->frame_delay > 0.3 && game->boss->state == ALERT)
	{
		game->boss->cur_sprite = (game->boss->cur_sprite + 1) % 3;
		game->boss->frame_delay = 0;
	}
	else if (game->boss->frame_delay > 0.3 && game->boss->state == ATTACK)
		boss_attack(game);
	else if (game->boss->death_delay > 0.4 && game->boss->state == DYING)
	{
		game->boss->dying_sprite++;
		game->boss->death_delay = 0;
		if (game->boss->dying_sprite > 9)
			game->boss->state = DEAD;
	}
}

void	manage_boss(t_game *game)
{
	if (game->boss == NULL || !game->boss->boss_text[0])
		return ;
	calculate_distance_to_player(game);
	if (game->boss->state != DEAD)
		calculate_sprite_change(game);
	if (game->boss->state == ALERT)
		calculate_boss_position(game, game->boss->pos_x, game->boss->pos_y,
			game->boss->boss_text[game->boss->cur_sprite]);
	else if (game->boss->state == ATTACK)
		calculate_boss_position(game, game->boss->pos_x, game->boss->pos_y,
			game->boss->boss_text[game->boss->attack_sprite]);
	else if (game->boss->state == DYING)
		calculate_boss_position(game, game->boss->pos_x, game->boss->pos_y,
			game->boss->boss_text[game->boss->dying_sprite]);
}

void	create_boss(t_game *game, int i, int j)
{
	if (!game->boss || game->boss->b == 1)
		shutdown_program(game, EXIT_TOO_MANY_BOSSES);
	game->boss->b = 1;
	game->boss->hp = 60;
	game->boss->pos_x = j + 0.5;
	game->boss->pos_y = i + 0.5;
	game->boss->state = IDLE;
	game->boss->attack_sprite = 3;
	game->boss->dying_sprite = 7;
	game->boss->move_speed = 0.2;
	game->boss->hit_player = false;
}
