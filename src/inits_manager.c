/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafaelheringer <rafaelheringer@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 14:38:28 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/07/23 14:52:12 by rafaelherin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int32_t init_cub3d(t_game *game)
{
    game->mlx = mlx_init(800, 600, "Cub3D", true);
    mlx_loop(game->mlx);
    return (0);
}