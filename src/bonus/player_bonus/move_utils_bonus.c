/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_utils_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:37:21 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/11 10:18:09 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static bool	calculate_dist_to_enemy(t_enemy_list *enemy, double x, double y)
{
	double	distance;
	double	dx;
	double	dy;

	dx = x - enemy->pos_x;
	dy = y - enemy->pos_y;
	distance = sqrt(dx * dx + dy * dy);
	if (distance > 0.3)
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
		if (!calculate_dist_to_enemy(tmp, x, y))
			return (false);
		tmp = tmp->next;
	}
	return (true);
}

void	handle_movement(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	game->delta_time = get_delta_time();
	get_move(game);
	if (game->raycasting->image)
		mlx_delete_image(game->mlx, game->raycasting->image);
	perform_raycasting(game);
	manage_enemies(game);
	update_minimap(game);
}
