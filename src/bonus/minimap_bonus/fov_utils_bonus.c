/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fov_utils_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:46:52 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/14 15:58:52 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

int	cxx(t_game *game, int scale)
{
	int	tile_x;
	int	px0;
	int	cx;

	tile_x = (int)game->player->pos_x;
	px0 = 5 + tile_x * scale;
	cx = px0 + (scale / 2);
	return (cx);
}

int	cyy(t_game *game, int scale)
{
	int	tile_y;
	int	py0;
	int	cy;

	tile_y = (int)game->player->pos_y;
	py0 = 5 + tile_y * scale;
	cy = py0 + (scale / 2);
	return (cy);
}

void	calculate_player_fov(t_game *game, int scale)
{
	double	len;

	if (!game || !game->player || !game->raycasting)
		return ;
	len = scale * 3.0;
	if (len < 16.0)
		len = 16.0;
	calculate_variables(game, len, scale);
}
