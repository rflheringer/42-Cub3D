/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_enemy_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:27:01 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/14 18:14:57 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static void	add_enemy_list(t_game *game, t_enemy_list *enemy)
{
	t_enemy_list	*last;

	if (game->enemy->list == NULL)
		game->enemy->list = enemy;
	else
	{
		last = game->enemy->list;
		while (last->next)
			last = last->next;
		last->next = enemy;
		enemy->prev = last;
	}
}

static void	create_enemy(t_game *game, int i, int j)
{
	t_enemy_list	*enemy;

	enemy = ft_calloc(1, sizeof(t_enemy_list));
	enemy->pos_x = j + 0.5;
	enemy->pos_y = i + 0.5;
	enemy->state = ALERT;
	enemy->cur_sprite = 0;
	enemy->attack_sprite = 3;
	enemy->dying_sprite = 7;
	add_enemy_list(game, enemy);
}

void	set_enemy_and_door(t_game *game, int i, int j)
{
	if (game->map->map[i][j] == 'I')
		create_enemy(game, i, j);
	if (game->map->map[i][j] == 'D')
		create_door(game, i, j);
	if (game->map->map[i][j] == 'B')
		create_boss(game, i, j);
}
