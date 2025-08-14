/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 13:17:09 by rheringe          #+#    #+#             */
/*   Updated: 2025/08/14 15:10:26 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static void	key_release(mlx_key_data_t keydata, t_game *game)
{
	if (keydata.key == MLX_KEY_W)
		game->player->up = false;
	else if (keydata.key == MLX_KEY_S)
		game->player->down = false;
	else if (keydata.key == MLX_KEY_D)
		game->player->right = false;
	else if (keydata.key == MLX_KEY_A)
		game->player->left = false;
	else if (keydata.key == MLX_KEY_LEFT)
		game->player->rot_right = false;
	else if (keydata.key == MLX_KEY_RIGHT)
		game->player->rot_left = false;
}

static void	key_press(mlx_key_data_t keydata, t_game *game)
{
	if (keydata.key == MLX_KEY_W)
		game->player->up = true;
	else if (keydata.key == MLX_KEY_S)
		game->player->down = true;
	else if (keydata.key == MLX_KEY_D)
		game->player->right = true;
	else if (keydata.key == MLX_KEY_A)
		game->player->left = true;
	else if (keydata.key == MLX_KEY_LEFT)
		game->player->rot_right = true;
	else if (keydata.key == MLX_KEY_RIGHT)
		game->player->rot_left = true;
}

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

void	create_door(t_game *game, int i, int j)
{
	t_door	*door;

	door = ft_calloc(1, sizeof(t_door));
	door->need_key = true;
	door->pos_x = j;
	door->pos_y = i;
	add_door_list(game, door);
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

static void	open_close_door(t_game *game)
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
	if (game->map->map[chc_y][chc_x] == 'D'
			&& door->need_key && game->player->keys)
	{
		if (game->player->keys)
			game->player->keys -= 1;
		door->need_key = false;
		game->map->map[chc_y][chc_x] = 'O';
	}
	else if (game->map->map[chc_y][chc_x] == 'D' && !door->need_key)
		game->map->map[chc_y][chc_x] = 'O';
	else if (game->map->map[chc_y][chc_x] == 'O')
		game->map->map[chc_y][chc_x] = 'D';
}

void	rotate_player_mouse(double xpos, double ypos, void *param)
{
	t_game	*game;
	double	original_speed;

	(void)ypos;
	game = (t_game *)param;
	if (game->game_over || game->game_win)
		return ;
	original_speed = game->player->rotation_speed;
	if (xpos < WIDTH * 0.3)
	{
		game->player->rotation_speed = original_speed * 0.4;
		rot_right(game);
		game->player->moved = true;
	}
	else if (xpos > WIDTH * 0.7)
	{
		game->player->rotation_speed = original_speed * 0.4;
		rot_left(game);
		game->player->moved = true;
	}
	game->player->rotation_speed = original_speed;
}

void	keypress(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (keydata.key == MLX_KEY_ESCAPE)
		mlx_close_window(game->mlx);
	if (game->game_over || game->game_win)
		return ;
	if (keydata.action == MLX_PRESS && keydata.key == MLX_KEY_F)
		open_close_door(game);
	else if (keydata.action == MLX_PRESS && keydata.key == MLX_KEY_SPACE
		&& game->player->attack_delay > 0.4)
	{
		create_fireball(game);
		game->player->attack_delay = 0;
	}
	else if (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT)
		key_press(keydata, game);
	else if (keydata.action == MLX_RELEASE)
		key_release(keydata, game);
}
