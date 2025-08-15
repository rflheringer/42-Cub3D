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

static void	calculate_potion_position(t_game *game, double pos_x, double pos_y,
	mlx_texture_t *texture)
{
	t_render	*rend;
	double		inv_det;

	rend = ft_calloc(1, sizeof(t_render));
	rend->less_height = 12000;
	rend->sprite_x = pos_x - game->player->pos_x;
	rend->sprite_y = pos_y - game->player->pos_y;
	inv_det = 1.0 / (game->player->camera_dir_x * game->player->player_dir_y
			- game->player->player_dir_x * game->player->camera_dir_y);
	rend->transform_x = inv_det * (game->player->player_dir_y * rend->sprite_x
			- game->player->player_dir_x * rend->sprite_y);
	rend->transform_y = inv_det * (-game->player->camera_dir_y * rend->sprite_x
			+ game->player->camera_dir_x * rend->sprite_y);
	if (rend->transform_y <= 0)
		return ;
	rend->sprite_height = (int)fabs(HEIGHT / rend->transform_y);
	rend->sprite_screen_x = (int)((WIDTH / 2) * (1 + rend->transform_x
				/ rend->transform_y));
	calculate_sprite(rend);
	put_x_on_screen(game, rend, texture);
	free(rend);
}

static void	calculate_key_position(t_game *game, double pos_x, double pos_y,
	mlx_texture_t *texture)
{
	t_render	*rend;
	double		inv_det;

	rend = ft_calloc(1, sizeof(t_render));
	rend->less_height = 10000;
	rend->sprite_x = pos_x - game->player->pos_x;
	rend->sprite_y = pos_y - game->player->pos_y;
	inv_det = 1.0 / (game->player->camera_dir_x * game->player->player_dir_y
			- game->player->player_dir_x * game->player->camera_dir_y);
	rend->transform_x = inv_det * (game->player->player_dir_y * rend->sprite_x
			- game->player->player_dir_x * rend->sprite_y);
	rend->transform_y = inv_det * (-game->player->camera_dir_y * rend->sprite_x
			+ game->player->camera_dir_x * rend->sprite_y);
	if (rend->transform_y <= 0)
		return ;
	rend->sprite_height = (int)fabs(HEIGHT / rend->transform_y);
	rend->sprite_screen_x = (int)((WIDTH / 2) * (1 + rend->transform_x
				/ rend->transform_y));
	calculate_sprite(rend);
	put_x_on_screen(game, rend, texture);
	free(rend);
}

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
				calculate_potion_position(game, (double)j + 0.5,
					(double)i + 0.5, game->texture->potion);
			if (game->map->map[i][j] == 'K')
				calculate_key_position(game, (double)j + 0.5,
					(double)i + 0.5, game->texture->key);
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
