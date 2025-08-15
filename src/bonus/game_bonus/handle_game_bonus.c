/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_game_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 19:45:07 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/14 19:50:39 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static void	render_potion_and_key(t_game *game)
{
	int	i;
	int	j;

	i = 0;
	while (game->map->map[i])
	{
		j = 0;
		while (game->map->map[i][j])
		{
			if (game->map->map[i][j] == 'P')
				calculate_enemie_position(game, (double)j + 0.5,
					(double)i + 0.5, game->texture->potion, 14500);
			if (game->map->map[i][j] == 'K')
				calculate_enemie_position(game, (double)j + 0.5,
					(double)i + 0.5, game->texture->key, 10000);
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
	render_potion_and_key(game);
	manage_boss(game);
	manage_enemies(game);
	update_fireballs(game);
	update_bullets(game);
	update_minimap(game);
	if (game->boss->hit_player)
	{
		game->player->hp -= 2;
		game->boss->hit_player = false;
	}
	call_clean_and_draw_functions(game);
}
