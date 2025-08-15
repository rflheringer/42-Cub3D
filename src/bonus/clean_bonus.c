/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:24:26 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/15 17:39:46 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d_bonus.h"

static void	delete_images(t_game *game)
{
	if (!game->image)
		return ;
	if (game->raycasting->image)
		mlx_delete_image(game->mlx, game->raycasting->image);
	if (game->image->game_over)
		mlx_delete_image(game->mlx, game->image->game_over);
	if (game->image->game_win)
		mlx_delete_image(game->mlx, game->image->game_win);
	if (game->image->hand)
		mlx_delete_image(game->mlx, game->image->hand);
	if (game->image->keys_sprite)
		mlx_delete_image(game->mlx, game->image->keys_sprite);
	if (game->image->keys_text)
		mlx_delete_image(game->mlx, game->image->keys_text);
	free(game->image);
}

static void	free_texture_path(t_game *game)
{
	if (!game || !game->texture)
		return ;
	free(game->texture->north_path);
	free(game->texture->south_path);
	free(game->texture->east_path);
	free(game->texture->west_path);
	free(game->texture->floor_color);
	free(game->texture->ceiling_color);
	if (game->game_over >= 1)
	{
		delete_texture_array(game->boss->boss_text, 10);
		delete_texture_array(game->boss->bullet_textures, 4);
	}
}

static void	delete_texture(t_game *game)
{
	(void)game;
	if (game->texture->north_wall)
		mlx_delete_texture(game->texture->north_wall);
	if (game->texture->south_wall)
		mlx_delete_texture(game->texture->south_wall);
	if (game->texture->east_wall)
		mlx_delete_texture(game->texture->east_wall);
	if (game->texture->west_wall)
		mlx_delete_texture(game->texture->west_wall);
	if (game->texture->closed_door)
		mlx_delete_texture(game->texture->closed_door);
	if (game->texture->potion)
		mlx_delete_texture(game->texture->potion);
	if (game->texture->key)
		mlx_delete_texture(game->texture->key);
	if (game->texture->game_over)
		mlx_delete_texture(game->texture->game_over);
	if (game->texture->game_win)
		mlx_delete_texture(game->texture->game_win);
	if (game->texture->hand)
		mlx_delete_texture(game->texture->hand);
	delete_texture_array(game->player->fireball_textures, 5);
	delete_texture_array(game->enemy->enemy_texture, 10);
	free_texture_path(game);
}

static void	free_other_stuff(t_game *game)
{
	if (game->enemy->list)
		clean_enemy_list(game);
	if (game->fireballs)
	{
		clean_fire_list(game->fireballs);
		game->fireballs = NULL;
	}
	if (game->bullets)
	{
		clean_fire_list(game->bullets);
		game->bullets = NULL;
	}
	if (game->doors)
		clean_door_list(game);
	if (game->ray)
	{
		free(game->ray);
		free(game->wall);
		free(game->raycasting->buffer);
		free(game->raycasting);
	}
	if (game->ui)
		free(game->ui);
	if (game->lightning)
		free(game->lightning);
}

void	shutdown_program(t_game *game, short error_code)
{
	if (error_code != 0)
		error_messages(error_code);
	delete_images(game);
	if (game->texture)
	{
		delete_texture(game);
		free(game->texture);
	}
	if (game->mlx)
		mlx_terminate(game->mlx);
	ft_free_matrix(game->map->file_content);
	ft_free_matrix(game->map->map);
	free_other_stuff(game);
	if (game->boss)
		free(game->boss);
	free(game->enemy);
	free(game->player);
	free(game->map);
	free(game);
	exit(error_code);
}
