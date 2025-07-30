/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:24:26 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/30 16:55:38 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	delete_images(t_game *game)
{
	if (game->image->north_wall)
		mlx_delete_image(game->mlx, game->image->north_wall);
	if (game->image->south_wall)
		mlx_delete_image(game->mlx, game->image->south_wall);
	if (game->image->east_wall)
		mlx_delete_image(game->mlx, game->image->east_wall);
	if (game->image->west_wall)
		mlx_delete_image(game->mlx, game->image->west_wall);
	if (game->raycasting->image)
		mlx_delete_image(game->mlx, game->raycasting->image);
}

static void	delete_texture(t_game *game)
{
	free(game->texture->north_path);
	free(game->texture->south_path);
	free(game->texture->east_path);
	free(game->texture->west_path);
	free(game->texture->floor_color);
	free(game->texture->ceiling_color);
	if (game->texture->north_wall)
		mlx_delete_texture(game->texture->north_wall);
	if (game->texture->south_wall)
		mlx_delete_texture(game->texture->south_wall);
	if (game->texture->east_wall)
		mlx_delete_texture(game->texture->east_wall);
	if (game->texture->west_wall)
		mlx_delete_texture(game->texture->west_wall);
}

void	shutdown_program(t_game *game, short error_code)
{
	if (error_code != 0)
		error_messages(error_code);
	delete_images(game);
	delete_texture(game);
	mlx_terminate(game->mlx);
	ft_free_matrix(game->map->file_content);
	ft_free_matrix(game->map->map);
	free(game->texture);
	free(game->wall);
	free(game->ray);
	free(game->raycasting);
	free(game->image);
	free(game->player);
	free(game->map);
	free(game);
	exit(error_code);
}
