/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_game_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 11:40:07 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/12 15:25:18 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static void	update_game_over(t_game *game)
{
	if (game->raycasting->image)
		mlx_delete_image(game->mlx, game->raycasting->image);
	game->texture->game_over = mlx_load_png("assets/player/end_game.png");
	game->image->game_over = mlx_texture_to_image(game->mlx, game->texture->game_over);
	mlx_resize_image(game->image->game_over, WIDTH, HEIGHT);
	mlx_image_to_window(game->mlx, game->image->game_over, 0, 0);
}

static void	update_game_win(t_game *game)
{
	if (game->raycasting->image)
		mlx_delete_image(game->mlx, game->raycasting->image);
	game->texture->game_win = mlx_load_png("assets/player/win_game.png");
	game->image->game_win = mlx_texture_to_image(game->mlx, game->texture->game_win);
	mlx_resize_image(game->image->game_win, WIDTH, HEIGHT);
	mlx_image_to_window(game->mlx, game->image->game_win, 0, 0);
}

void	update_game_state(t_game *game)
{
	if (game->game_over == 1)
	{
		update_game_over(game);
		game->game_over += 1;
	}
	if (game->game_win == 1)
	{
		update_game_win(game);
		game->game_win += 1;
	}
}
