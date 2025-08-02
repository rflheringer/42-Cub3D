/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lightning_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafaelheringer <rafaelheringer@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 15:48:31 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/08/02 15:58:00 by rafaelherin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

void init_lightning(t_game *game)
{
    game->lightning = ft_calloc(1, sizeof(t_lightning));
    game->lightning->is_active = false;
    game->lightning->original_color = game->texture->ceiling_color_hex;
    game->lightning->flash_color = 0xFFFFFFFF;
    game->lightning->last_time = 0.0;
    game->lightning->next_flash = (rand() % 60) + 30;
}

void update_lightning(t_game *game)
{
    double  cur_time;
    double  elapsed;

    cur_time = mlx_get_time();
    elapsed = cur_time - game->lightning->last_time;
    if (game->lightning->is_active)
    {
        if (elapsed >= game->lightning->duration)
        {
            game->lightning->is_active = false;
            game->texture->ceiling_color_hex = game->lightning->original_color;
            game->lightning->duration = ((rand() % 3) + 1) / 10.0;
            game->lightning->last_time = cur_time;
        }
    }
    else if (elapsed >= game->lightning->next_flash)
    {
        game->lightning->is_active = true;
        game->texture->ceiling_color_hex = game->lightning->flash_color;
        game->lightning->duration = ((rand() % 3) + 1) / 10.0; // 0.1-0.3 segundos
        game->lightning->last_time = cur_time;
        if (rand() % 4 == 0)
            game->lightning->next_flash = ((rand() % 3) + 1) / 10.0;
    }
}