/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_utils_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:37:21 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/05 18:52:01 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

bool	right_move(t_game *game)
{
	double	new_x;
	double	new_y;

	new_x = game->player->pos_x + game->player->player_dir_y
		* game->player->move_speed;
	new_y = game->player->pos_y - game->player->player_dir_x
		* game->player->move_speed;
	if (game->map->map[(int)new_y][(int)new_x] == '1')
		return (false);
	if (can_move_to(game->map->map, new_x, new_y))
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
	if (game->map->map[(int)new_y][(int)new_x] == '1')
		return (false);
	if (can_move_to(game->map->map, new_x, new_y))
	{
		game->player->old_x = game->player->pos_x;
		game->player->old_y = game->player->pos_y;
		game->player->pos_x = new_x;
		game->player->pos_y = new_y;
		return (true);
	}
	return (false);
}

bool	can_move_to(char **map, double x, double y)
{
	int	ix;
	int	iy;

	ix = (int)floor(x + R);
	iy = (int)floor(y + R);
	if (map[iy][ix] == '1')
		return (false);
	ix = (int)floor(x - R);
	iy = (int)floor(y + R);
	if (map[iy][ix] == '1')
		return (false);
	ix = (int)floor(x + R);
	iy = (int)floor(y - R);
	if (map[iy][ix] == '1')
		return (false);
	ix = (int)floor(x - R);
	iy = (int)floor(y - R);
	if (map[iy][ix] == '1')
		return (false);
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
