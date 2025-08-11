/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:45:53 by rheringe          #+#    #+#             */
/*   Updated: 2025/08/11 10:18:15 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static bool	up_move(t_game *game)
{
	double	new_x;
	double	new_y;

	new_x = game->player->pos_x + (game->player->player_dir_x
			* game->player->move_speed);
	new_y = game->player->pos_y + (game->player->player_dir_y
			* game->player->move_speed);
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

static bool	down_move(t_game *game)
{
	double	new_x;
	double	new_y;

	new_x = game->player->pos_x - game->player->player_dir_x
		* game->player->move_speed;
	new_y = game->player->pos_y - game->player->player_dir_y
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

static bool	rot_left(t_game *game)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = game->player->player_dir_x;
	game->player->player_dir_x = game->player->player_dir_x
		* cos(-game->player->rotation_speed)
		- game->player->player_dir_y * sin(-game->player->rotation_speed);
	game->player->player_dir_y = old_dir_x
		* sin(-game->player->rotation_speed)
		+ game->player->player_dir_y * cos(-game->player->rotation_speed);
	old_plane_x = game->player->camera_dir_x;
	game->player->camera_dir_x = game->player->camera_dir_x
		* cos(-game->player->rotation_speed)
		- game->player->camera_dir_y * sin(-game->player->rotation_speed);
	game->player->camera_dir_y = old_plane_x
		* sin(-game->player->rotation_speed)
		+ game->player->camera_dir_y * cos(-game->player->rotation_speed);
	return (true);
}

static bool	rot_right(t_game *game)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = game->player->player_dir_x;
	game->player->player_dir_x = game->player->player_dir_x
		* cos(game->player->rotation_speed)
		- game->player->player_dir_y * sin(game->player->rotation_speed);
	game->player->player_dir_y = old_dir_x
		* sin(game->player->rotation_speed)
		+ game->player->player_dir_y * cos(game->player->rotation_speed);
	old_plane_x = game->player->camera_dir_x;
	game->player->camera_dir_x = game->player->camera_dir_x
		* cos(game->player->rotation_speed)
		- game->player->camera_dir_y * sin(game->player->rotation_speed);
	game->player->camera_dir_y = old_plane_x
		* sin(game->player->rotation_speed)
		+ game->player->camera_dir_y * cos(game->player->rotation_speed);
	return (true);
}

void	get_move(t_game *game)
{
	if (game->player->up)
		if (up_move(game))
			game->player->moved = true;
	if (game->player->down)
		if (down_move(game))
			game->player->moved = true;
	if (game->player->right)
		if (right_move(game))
			game->player->moved = true;
	if (game->player->left)
		if (left_move(game))
			game->player->moved = true;
	if (game->player->rot_left)
		if (rot_left(game))
			game->player->moved = true;
	if (game->player->rot_right)
		if (rot_right(game))
			game->player->moved = true;
}

void rotate_player_mouse(double xpos, double ypos, void *param)
{
    t_game *game;
    static double last_xpos = WIDTH / 2;
    double mouse_delta;
    double original_speed;
    (void)ypos;
    
    game = (t_game *)param;
    
    // Skip first call to avoid large jumps
    if (last_xpos == WIDTH / 2)
    {
        last_xpos = xpos;
        return;
    }
    
    // Normal movement based on mouse delta
    mouse_delta = 0.5;
    original_speed = game->player->rotation_speed;
    
    // Edge detection - continuous rotation when mouse at edges
    if (xpos < WIDTH * 0.3) // Left 10% of screen
    {
        // Continuous left rotation when at left edge
        game->player->rotation_speed = original_speed * -mouse_delta;
        rot_left(game);
        game->player->moved = true;
    }
    else if (xpos > WIDTH * 0.7) // Right 10% of screen
    {
        // Continuous right rotation when at right edge
        game->player->rotation_speed = original_speed * -mouse_delta;
        rot_right(game);
        game->player->moved = true;
    }
    game->player->rotation_speed = original_speed;
    last_xpos = xpos;
}
