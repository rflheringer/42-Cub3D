/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_utils_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:37:21 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/12 12:13:06 by rdel-fra         ###   ########.fr       */
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
	if (can_move_to(game->map->map, new_x, new_y, game->enemy->list))
	{
		if (game->map->map[(int)new_y][(int)new_x] == 'P')
		{
			game->player->hp += 3;
			if (game->player->hp > 10)
				game->player->hp = 10;
			game->map->map[(int)new_y][(int)new_x] = '0';
		}
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
	if (can_move_to(game->map->map, new_x, new_y, game->enemy->list))
	{
		if (game->map->map[(int)new_y][(int)new_x] == 'P')
		{
			game->player->hp += 3;
			if (game->player->hp > 10)
				game->player->hp = 10;
			game->map->map[(int)new_y][(int)new_x] = '0';
		}
		game->player->old_x = game->player->pos_x;
		game->player->old_y = game->player->pos_y;
		game->player->pos_x = new_x;
		game->player->pos_y = new_y;
		return (true);
	}
	return (false);
}

bool	can_move_to(char **map, double x, double y, t_enemy_list *enemy_list)
{
	t_enemy_list	*tmp;

	if (map[(int)floor(y + R)][(int)floor(x + R)] == '1'
		|| map[(int)floor(y + R)][(int)floor(x + R)] == 'D')
		return (false);
	if (map[(int)floor(y + R)][(int)floor(x - R)] == '1'
		|| map[(int)floor(y + R)][(int)floor(x - R)] == 'D')
		return (false);
	if (map[(int)floor(y - R)][(int)floor(x + R)] == '1'
		|| map[(int)floor(y - R)][(int)floor(x + R)] == 'D')
		return (false);
	if (map[(int)floor(y - R)][(int)floor(x - R)] == '1'
		|| map[(int)floor(y - R)][(int)floor(x - R)] == 'D')
		return (false);
	tmp = enemy_list;
	while (tmp)
	{
		if ((tmp->state == ALERT || tmp->state == ATTACK) && !calculate_dist_to_enemy(tmp, x, y))
			return (false);
		tmp = tmp->next;
	}
	return (true);
}

static void	clean_dead_enemies(t_game *game)
{
	t_enemy_list	*current;
	t_enemy_list	*next;
	t_enemy_list	*prev;

	current = game->enemy->list;
	prev = NULL;
	while (current)
	{
		next = current->next;
		if (current->state == DEAD)
		{
			if (prev)
				prev->next = next;
			else
				game->enemy->list = next;
			free(current);
		}
		else
			prev = current;
		current = next;
	}
}

static void	clean_dead_fireballs(t_game *game)
{
	t_attack	*current;
	t_attack	*next;
	t_attack	*prev;

	current = game->fireballs;
	prev = NULL;
	while (current)
	{
		next = current->next;
		if (current->state == DEAD)
		{
			if (prev)
				prev->next = next;
			else
				game->fireballs = next;
			free(current);
		}
		else
			prev = current;
		current = next;
	}
}

static void	render_potion(t_game *game)
{
	int	i;
	int j;

	i = 0;
	while (game->map->map[i])
	{
		j = 0;
		while (game->map->map[i][j])
		{
			if (game->map->map[i][j] == 'P')
				calculate_enemie_position(game, (double)j + 0.5, (double)i + 0.5, game->texture->potion);
			if (game->map->map[i][j] == 'K')
				calculate_enemie_position(game, (double)j + 0.5, (double)i + 0.5, game->texture->key);
			j++;
		}
		i++;
	}
}

void	handle_movement(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	game->delta_time = get_delta_time();
	update_game_state(game);
	if (game->game_over >= 1 || game->game_win >= 1)
		return ;
	get_move(game);
	game->player->attack_delay += game->delta_time;
	perform_raycasting(game);
	render_potion(game);
	manage_enemies(game);
	update_fireballs(game);
	clean_dead_fireballs(game);
	clean_dead_enemies(game);
	update_minimap(game);
	draw_life(game);
	if (game->player->hp <= 0)
		game->game_over = 1;
	if (!game->enemy->list)
		game->game_win = 1;
}
