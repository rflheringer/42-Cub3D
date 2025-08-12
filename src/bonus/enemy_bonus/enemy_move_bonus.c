/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_move_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:37:18 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/12 18:57:42 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../../include/cub3d_bonus.h"

static bool	can_enemy_move_to(t_game *game, double x, double y, t_enemy_list *current_enemy)
{
	t_enemy_list	*tmp;

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
	while (tmp)
	{
		if (tmp != current_enemy)
		{
			double dx = x - tmp->pos_x;
			double dy = y - tmp->pos_y;
			double distance = sqrt(dx * dx + dy * dy);
			if (distance < 0.6)
				return (false);
		}
		tmp = tmp->next;
	}
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
	
	// Tentar movimento direto primeiro
	if (can_enemy_move_to(game, next_x, next_y, enemy))
	{
		enemy->pos_x = next_x;
		enemy->pos_y = next_y;
		enemy->move_delay = 0;  // Reset delay quando consegue se mover
		return ;
	}

	// Se não conseguir ir direto, tentar movimento perpendicular (contorno)
	double alt_x1 = enemy->pos_x + diry * game->enemy->move_speed;
	double alt_y1 = enemy->pos_y - dirx * game->enemy->move_speed;
	if (can_enemy_move_to(game, alt_x1, alt_y1, enemy))
	{
		enemy->pos_x = alt_x1;
		enemy->pos_y = alt_y1;
		enemy->move_delay = 0;  // Reset delay quando consegue se mover
		return ;
	}
	
	// Tentar movimento perpendicular no sentido oposto
	double alt_x2 = enemy->pos_x - diry * game->enemy->move_speed;
	double alt_y2 = enemy->pos_y + dirx * game->enemy->move_speed;
	if (can_enemy_move_to(game, alt_x2, alt_y2, enemy))
	{
		enemy->pos_x = alt_x2;
		enemy->pos_y = alt_y2;
		enemy->move_delay = 0;  // Reset delay quando consegue se mover
	}
	// Se chegou aqui, não conseguiu se mover em nenhuma direção
	// O move_delay continua acumulando, evitando tentativas muito frequentes
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
	if (enemy->state == ALERT && enemy->distance > 0.7 && enemy->move_delay > 0.1)
		enemy_move(game, enemy, dx, dy);
	else if (enemy->state == ALERT && enemy->distance <= 0.7)
		enemy->state = ATTACK;
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
	else if (enemy->death_delay > 0.5 && enemy->state == DYING)
	{
		enemy->dying_sprite++;
		enemy->death_delay = 0;
		if (enemy->dying_sprite > 9)
			enemy->state = DEAD;
	}
}

static bool	ft_swap(t_enemy_list **head, t_enemy_list *cur, t_enemy_list *next)
{
	if (!head || !cur || !next || cur == next)
		return (false);
	if (cur->prev)
		cur->prev->next = next;
	else
		*head = next;
	if (next->next)
		next->next->prev = cur;
	next->prev = cur->prev;
	cur->prev = next;
	cur->next = next->next;
	next->next = cur;
	return (true);
}

static bool	ft_is_sorted(t_game *game)
{
	t_enemy_list	*nav;

	nav = game->enemy->list;
	while (nav->next)
	{
		if (nav->distance < nav->next->distance)
			return (false);
		nav = nav->next;
	}
	return (true);
}

static void	sort_enemies(t_game *game, t_enemy_list *nav)
{
	if (!nav->next)
		return ;
	while (!ft_is_sorted(game))
	{
		nav = game->enemy->list;
		while (nav->next)
		{
			if (nav->distance < nav->next->distance)
				if(ft_swap(&game->enemy->list, nav, nav->next))
					break ;
			nav = nav->next;
		}
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
		nav = nav->next;
	}
	sort_enemies(game, game->enemy->list);
	nav = game->enemy->list;
	while (nav)
	{
		if (nav->state != DEAD)
			calculate_sprite_change(game, nav);
		if (nav->state == ALERT)
			calculate_enemie_position(game, nav->pos_x, nav->pos_y, game->enemy->enemy_texture[nav->cur_sprite], 6000);
		else if (nav->state == ATTACK)
			calculate_enemie_position(game, nav->pos_x, nav->pos_y, game->enemy->enemy_texture[nav->attack_sprite], 6000);
		else if (nav->state == DYING)
			calculate_enemie_position(game, nav->pos_x, nav->pos_y, game->enemy->enemy_texture[nav->dying_sprite], 6000);
		nav = nav->next;
	}
}
