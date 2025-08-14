/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_images_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 18:14:11 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/14 19:02:33 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static void	load_bonus_images(t_game *game, char *base_path, int len)
{
	char	enemy_path[100];
	char	*num;
	int		i;

	i = 0;
	while (i < len)
	{
		num = ft_itoa(i);
		if (!num)
			shutdown_program(game, EXIT_ERROR_MEMORY_ALLOCATION);
		ft_strlcpy(enemy_path, base_path, sizeof(enemy_path));
		ft_strlcat(enemy_path, num, sizeof(enemy_path));
		ft_strlcat(enemy_path, ".png", sizeof(enemy_path));
		free(num);
		game->enemy->enemy_texture[i] = mlx_load_png(enemy_path);
		if (!game->enemy->enemy_texture[i])
			shutdown_program(game, EXIT_INVALID_TEXTURE_PATH);
		i++;
	}
}

static void	load_player_attack(t_game *game, char *base_path, int len)
{
	char	attack_path[100];
	char	*num;
	int		i;

	i = 0;
	while (i < len)
	{
		num = ft_itoa(i);
		if (!num)
			shutdown_program(game, EXIT_ERROR_MEMORY_ALLOCATION);
		ft_strlcpy(attack_path, base_path, sizeof(attack_path));
		ft_strlcat(attack_path, num, sizeof(attack_path));
		ft_strlcat(attack_path, ".png", sizeof(attack_path));
		free(num);
		game->player->fireball_textures[i] = mlx_load_png(attack_path);
		if (!game->player->fireball_textures[i])
			shutdown_program(game, EXIT_INVALID_TEXTURE_PATH);
		i++;
	}
}

static void	load_boss_images(t_game *game, char *path, int len)
{
	char	attack_path[100];
	char	*num;
	int		i;

	i = 0;
	while (i < len)
	{
		num = ft_itoa(i);
		if (!num)
			shutdown_program(game, EXIT_ERROR_MEMORY_ALLOCATION);
		ft_strlcpy(attack_path, path, sizeof(attack_path));
		ft_strlcat(attack_path, num, sizeof(attack_path));
		ft_strlcat(attack_path, ".png", sizeof(attack_path));
		free(num);
		game->boss->boss_text[i] = mlx_load_png(attack_path);
		if (!game->boss->boss_text[i])
			shutdown_program(game, EXIT_INVALID_TEXTURE_PATH);
		i++;
	}
}

static void	load_boss_attack(t_game *game, char *path, int len)
{
	char	attack_path[100];
	char	*num;
	int		i;

	i = 0;
	while (i < len)
	{
		num = ft_itoa(i);
		if (!num)
			shutdown_program(game, EXIT_ERROR_MEMORY_ALLOCATION);
		ft_strlcpy(attack_path, path, sizeof(attack_path));
		ft_strlcat(attack_path, num, sizeof(attack_path));
		ft_strlcat(attack_path, ".png", sizeof(attack_path));
		free(num);
		game->boss->bullet_textures[i] = mlx_load_png(attack_path);
		if (!game->boss->bullet_textures[i])
			shutdown_program(game, EXIT_INVALID_TEXTURE_PATH);
		i++;
	}
}

void	init_bonus_images(t_game *game)
{
	if (ft_strnstr(game->map->file_name, "sewer",
			ft_strlen(game->map->file_name)))
	{
		load_bonus_images(game, "assets/enemy/sea_dragon/sea_dragon_", 10);
	}
	else
	{
		load_bonus_images(game, "assets/enemy/skeleton/skeleton_", 10);
	}
	load_player_attack(game, "assets/player/fireball_", 5);
	load_boss_attack(game, "assets/enemy/boss_attack/boss_attack_", 4);
	load_boss_images(game, "assets/enemy/boss_skeleton/boss_skeleton_", 10);
	game->enemy->move_speed = 0.15;
}
