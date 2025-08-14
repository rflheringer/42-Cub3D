/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 19:54:09 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/14 20:03:35 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static void	add_door_list(t_game *game, t_door *door)
{
	t_door	*last;

	if (!game->doors)
		game->doors = door;
	else
	{
		last = game->doors;
		while (last->next)
			last = last->next;
		last->next = door;
	}
}

static t_door	*find_the_door(t_game *game, double check_x, double check_y)
{
	t_door	*nav;

	nav = game->doors;
	while (nav)
	{
		if (nav->pos_x == (int)check_x && nav->pos_y == (int)check_y)
			return (nav);
		nav = nav->next;
	}
	return (NULL);
}

static void	open_or_close(t_game *game, int x, int y, t_door *door)
{
	if (game->map->map[y][x] == 'D'
			&& door->need_key && game->player->keys)
	{
		if (game->player->keys)
			game->player->keys -= 1;
		door->need_key = false;
		game->map->map[y][x] = 'O';
	}
	else if (game->map->map[y][x] == 'D' && !door->need_key)
		game->map->map[y][x] = 'O';
	else if (game->map->map[y][x] == 'O')
		game->map->map[y][x] = 'D';
}

void	can_open_or_close_door(t_game *game)
{
	t_door	*door;
	int		chc_x;
	int		chc_y;
	double	dist;

	dist = 1.0;
	if (!game->doors)
		return ;
	chc_x = (int)floor(game->player->pos_x
			+ game->player->player_dir_x * dist);
	chc_y = (int)floor(game->player->pos_y
			+ game->player->player_dir_y * dist);
	if (chc_x < 0 || chc_y < 0 || chc_y >= game->map->height)
		return ;
	if (!game->map->map[chc_y]
		|| chc_x >= (int)ft_strlen(game->map->map[chc_y]))
		return ;
	door = find_the_door(game, chc_x, chc_y);
	if (!door)
		return ;
	open_or_close(game, chc_x, chc_y, door);
}

void	create_door(t_game *game, int i, int j)
{
	t_door	*door;

	door = ft_calloc(1, sizeof(t_door));
	door->need_key = true;
	door->pos_x = j;
	door->pos_y = i;
	add_door_list(game, door);
}
