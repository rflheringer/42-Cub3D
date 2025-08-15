/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_utils_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 19:48:16 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/15 18:46:12 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

void	draw_hand(t_game *game)
{
	int	instance;

	if (game->image->hand)
		mlx_delete_image(game->mlx, game->image->hand);
	game->texture->hand = mlx_load_png("assets/player/player_hand_white.png");
	game->image->hand = mlx_texture_to_image(game->mlx, game->texture->hand);
	mlx_resize_image(game->image->hand, 500, 350);
	instance = mlx_image_to_window(game->mlx, game->image->hand, 590, 600);
	mlx_set_instance_depth(&game->image->hand->instances[instance], 1);
}

void	draw_keys(t_game *game)
{
	char	*nb;

	if (game->image->keys_text)
		mlx_delete_image(game->mlx, game->image->keys_text);
	if (game->image->keys_sprite)
		mlx_delete_image(game->mlx, game->image->keys_sprite);
	nb = ft_itoa(game->player->keys);
	game->image->keys_sprite = mlx_texture_to_image(game->mlx,
			game->texture->key);
	mlx_image_to_window(game->mlx, game->image->keys_sprite, 1536, 830);
	mlx_set_instance_depth(&game->image->keys_sprite->instances[0], 1);
	game->image->keys_text = mlx_put_string(game->mlx, nb, 1600, 850);
	mlx_set_instance_depth(&game->image->keys_text->instances[0], 1);
	free(nb);
}

void	check_key_and_potion(t_game *game, int x, int y)
{
	if (game->map->map[(int)y][(int)x] == 'P')
	{
		game->player->hp += 3;
		if (game->player->hp > 10)
			game->player->hp = 10;
		game->map->map[(int)y][(int)x] = '0';
	}
	if (game->map->map[(int)y][(int)x] == 'K')
	{
		game->player->keys += 1;
		game->map->map[(int)y][(int)x] = '0';
	}
}
