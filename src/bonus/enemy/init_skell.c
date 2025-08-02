/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_skell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafaelheringer <rafaelheringer@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 23:42:11 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/08/02 00:04:18 by rafaelherin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

void    load_skell_images(t_game *game)
{
    char    enemy_path[42]; 
    char    *num;
    int     i;

    i = -1;
    while (i++, i < 10)
    {
        num = ft_itoa(i);
        if (!num)
            shutdown_program(game, EXIT_ERROR_MEMORY_ALLOCATION);
        ft_strlcpy(enemy_path, "assets/enemy/skeleton/skeleton_", 42);
        ft_strlcat(enemy_path, num, sizeof(enemy_path));
        ft_strlcat(enemy_path, ".png", sizeof(enemy_path));
        free(num);
        
        game->enemy->skell_texture[i] = mlx_load_png(enemy_path);
        if(!game->enemy->skell_texture[i])
            shutdown_program(game, EXIT_INVALID_TEXTURE_PATH);
        game->enemy->skell_images[i] = mlx_texture_to_image(game->mlx,
                game->enemy->skell_texture[i]);
        if (!game->enemy->skell_images[i])
            shutdown_program(game, EXIT_INVALID_TEXTURE_PATH);
    }
}

