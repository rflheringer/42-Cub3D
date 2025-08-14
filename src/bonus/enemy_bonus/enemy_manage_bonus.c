/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_move_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:37:18 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/14 18:25:14 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../../include/cub3d_bonus.h"

static void	change_enemy_attack(t_game *game, t_enemy_list *enemy)
{
	enemy->attack_sprite++;
	enemy->frame_delay = 0;
	if (enemy->attack_sprite > 6)
	{
		game->player->hp -= 1;
		enemy->state = ALERT;
		enemy->attack_sprite = 0;
	}
}

static void	calculate_sprite_change(t_game *game, t_enemy_list *enemy)
{
	if (enemy->state == ALERT || enemy->state == ATTACK)
		enemy->frame_delay += game->delta_time;
	else if (enemy->state == DYING)
		enemy->death_delay += game->delta_time;
	if (enemy->frame_delay > 0.3 && enemy->state == ALERT)
	{
		enemy->cur_sprite = (enemy->cur_sprite + 1) % 3;
		enemy->frame_delay = 0;
	}
	else if (enemy->frame_delay > 0.2 && enemy->state == ATTACK)
		change_enemy_attack(game, enemy);
	else if (enemy->death_delay > 0.5 && enemy->state == DYING)
	{
		enemy->dying_sprite++;
		enemy->death_delay = 0;
		if (enemy->dying_sprite > 9)
			enemy->state = DEAD;
	}
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
	if (enemy->state == ALERT && enemy->distance > 0.7
		&& enemy->move_delay > 0.1)
		enemy_move(game, enemy, dx, dy);
	else if (enemy->state == ALERT && enemy->distance <= 0.7)
		enemy->state = ATTACK;
}

static void	calculate_distance(t_game *game)
{
	t_enemy_list	*nav;

	nav = game->enemy->list;
	while (nav)
	{	
		calculate_distance_to_player(game, nav);
		nav = nav->next;
	}
}

void	manage_enemies(t_game *game)
{
	t_enemy_list	*nav;

	if (game->enemy->list == NULL)
		return ;
	calculate_distance(game);
	sort_enemies(game, game->enemy->list);
	nav = game->enemy->list;
	while (nav)
	{
		if (nav->state != DEAD)
			calculate_sprite_change(game, nav);
		if (nav->state == ALERT)
			calculate_enemie_position(game, nav->pos_x, nav->pos_y,
				game->enemy->enemy_texture[nav->cur_sprite], 6000);
		else if (nav->state == ATTACK)
			calculate_enemie_position(game, nav->pos_x, nav->pos_y,
				game->enemy->enemy_texture[nav->attack_sprite], 6000);
		else if (nav->state == DYING)
			calculate_enemie_position(game, nav->pos_x, nav->pos_y,
				game->enemy->enemy_texture[nav->dying_sprite], 6000);
		nav = nav->next;
	}
}
