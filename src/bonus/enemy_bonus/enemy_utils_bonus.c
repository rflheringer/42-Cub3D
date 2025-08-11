/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 17:41:16 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/11 13:29:27 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

double	get_delta_time(void)
{
	static double	last_time = 0;
	double			current_time;
	double			delta_time;

	current_time = mlx_get_time();
	delta_time = current_time - last_time;
	last_time = current_time;
	return (delta_time);
}

void	update_enemy_animation(t_enemy_list *e, double dt)
{
    e->frame_delay += dt;
    if (e->frame_delay > 0.12)
    {
        e->cur_sprite++;
        if (e->cur_sprite >= e->frame_count)
            e->cur_sprite = 0;
        e->frame_delay = 0;
    }
}