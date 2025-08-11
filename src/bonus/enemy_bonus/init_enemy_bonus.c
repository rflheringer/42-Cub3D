/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_enemy_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:27:01 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/11 13:38:48 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../../include/cub3d_bonus.h"

static void	load_tex_array(t_game *game, mlx_texture_t **dst, const char *base, int len)
{
	char	path[128];
	char	*num;
	int		i;

	i = 0;
	while (i < len)
	{
		num = ft_itoa(i);
		if (!num)
			shutdown_program(game, EXIT_ERROR_MEMORY_ALLOCATION);
		ft_strlcpy(path, base, sizeof(path));
		ft_strlcat(path, num, sizeof(path));
		ft_strlcat(path, ".png", sizeof(path));
		free(num);
		dst[i] = mlx_load_png(path);
		if (!dst[i])
			shutdown_program(game, EXIT_INVALID_TEXTURE_PATH);
		i++;
	}
}

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

static void	create_enemy(t_game *game, int i, int j, t_enemy_type type)
{
    t_enemy_list	*e;

    e = ft_calloc(1, sizeof(t_enemy_list));
    if (!e)
        shutdown_program(game, EXIT_ERROR_MEMORY_ALLOCATION);
    e->pos_x = j + 0.5;
    e->pos_y = i + 0.5;
    e->type = type;
    if (type == ENEMY_BOSS)
    {
        e->tex_set = game->enemy->boss_texture;
        e->frame_count = ENEMY_BOSS_FRAMES;
    }
    else
    {
        e->tex_set = game->enemy->skell_texture;
        e->frame_count = ENEMY_SKELL_FRAMES;
    }
    e->cur_sprite = 0;
    e->frame_delay = 0;
    add_enemy_list(game, e);
}

void	set_enemy(t_game *game, int i, int j)
{
    char	c;

    c = game->map->map[i][j];
    if (c == 'I')
	{
		create_enemy(game, i, j, ENEMY_NORMAL);
		game->map->map[i][j] = '0';
	}
    else if (c == 'B')
	{
		create_enemy(game, i, j, ENEMY_BOSS);
		game->map->map[i][j] = '0';
	}
}

void	set_default_enemy(t_game *game)
{ 
	load_tex_array(game, game->enemy->skell_texture,
        "assets/enemy/skeleton/skeleton_", ENEMY_SKELL_FRAMES);
    load_tex_array(game, game->enemy->boss_texture,
        "assets/enemy/sea_dragon/sea_dragon_", ENEMY_BOSS_FRAMES);
    game->enemy->move_speed = 0.2;
}
