/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_enemy_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:27:01 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/05 18:57:59 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../../include/cub3d_bonus.h"

static void	add_enemy_list(t_game *game, t_enemy_list *enemy)
{
	t_enemy_list	*last;

	if (!game->enemy->list)
		game->enemy->list = enemy;
	else
	{
		last = game->enemy->list;
		while (last->next)
			last = last->next;
		last->next = enemy;
	}
}

static void	create_enemy(t_game *game, int i, int j)
{
	t_enemy_list	*enemy;

	enemy = ft_calloc(1, sizeof(t_enemy_list));
	enemy->pos_x = j;
	enemy->pos_y = i;
	ft_printf("Enemy set x:%d y:%d\n", j, i);
	enemy->cur_sprite = 0;
	add_enemy_list(game, enemy);
}

void	set_enemy_or_door(t_game *game, int i, int j)
{
	if (game->map->map[i][j] == 'I')
		create_enemy(game, i, j);
}

static void	load_skell_images(t_game *game)
{
	char	enemy_path[42];
	char	*num;
	int		i;

	i = 0;
	while (i < 10)
	{
		num = ft_itoa(i);
		if (!num)
			shutdown_program(game, EXIT_ERROR_MEMORY_ALLOCATION);
		ft_strlcpy(enemy_path, "assets/enemy/skeleton/skeleton_", 42);
		ft_strlcat(enemy_path, num, sizeof(enemy_path));
		ft_strlcat(enemy_path, ".png", sizeof(enemy_path));
		free(num);
		game->enemy->skell_texture[i] = mlx_load_png(enemy_path);
		if (!game->enemy->skell_texture[i])
			shutdown_program(game, EXIT_INVALID_TEXTURE_PATH);
		game->enemy->skell_images[i] = mlx_texture_to_image(game->mlx,
				game->enemy->skell_texture[i]);
		if (!game->enemy->skell_images[i])
			shutdown_program(game, EXIT_INVALID_TEXTURE_PATH);
		i++;
	}
}

void	set_default_enemy(t_game *game)
{
	game->enemy->move_speed = 0.03;
	load_skell_images(game);
}
