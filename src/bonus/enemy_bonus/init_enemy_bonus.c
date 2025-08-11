/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_enemy_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:27:01 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/11 15:50:51 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../../include/cub3d_bonus.h"

static void	add_enemy_list(t_game *game, t_enemy_list *enemy)
{
	t_enemy_list	*last;

	if (game->enemy->list == NULL)
		game->enemy->list = enemy;
	else
	{
		last = game->enemy->list;
		while (last->next)
			last = last->next;
		last->next = enemy;
		enemy->prev = last;
	}
}

static void	create_enemy(t_game *game, int i, int j)
{
	t_enemy_list	*enemy;

	enemy = ft_calloc(1, sizeof(t_enemy_list));
	enemy->pos_x = j + 0.5;
	enemy->pos_y = i + 0.5;
	enemy->cur_sprite = 0;
	add_enemy_list(game, enemy);
}

void	set_enemy(t_game *game, int i, int j)
{
	if (game->map->map[i][j] == 'I')
		create_enemy(game, i, j);
}

static void	load_skell_images(t_game *game, char *base_path, int len)
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
		game->enemy->skell_texture[i] = mlx_load_png(enemy_path);
		if (!game->enemy->skell_texture[i])
			shutdown_program(game, EXIT_INVALID_TEXTURE_PATH);
		i++;
	}
}

void	set_default_enemy(t_game *game)
{
	if (ft_strnstr(game->map->file_name, "sewer", ft_strlen(game->map->file_name)))
		load_skell_images(game, "assets/enemy/sea_dragon/sea_dragon_", 3);
	else
		load_skell_images(game, "assets/enemy/skeleton/skeleton_", 11);
	game->enemy->move_speed = 0.2;
}
