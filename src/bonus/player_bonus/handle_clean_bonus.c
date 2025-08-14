/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_clean_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 19:46:24 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/14 19:51:21 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

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

static void	clean_dead_bullets(t_game *game)
{
	t_attack	*current;
	t_attack	*next;
	t_attack	*prev;

	current = game->bullets;
	prev = NULL;
	while (current)
	{
		next = current->next;
		if (current->state == DEAD)
		{
			if (prev)
				prev->next = next;
			else
				game->bullets = next;
			free(current);
		}
		else
			prev = current;
		current = next;
	}
}

static void	clean_dead_boss(t_game *game)
{
	t_boss	*temp;

	if (game->boss->state != DEAD)
		return ;
	temp = game->boss;
	game->boss = NULL;
	free(temp);
}

void	call_clean_and_draw_functions(t_game *game)
{
	clean_dead_fireballs(game);
	clean_dead_bullets(game);
	clean_dead_enemies(game);
	clean_dead_boss(game);
	draw_life(game);
	draw_hand(game);
	draw_keys(game);
	if (game->player->hp <= 0)
		game->game_over = 1;
	if (!game->boss)
		game->game_win = 1;
}
