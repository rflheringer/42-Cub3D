/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 17:41:16 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/05 17:41:50 by rdel-fra         ###   ########.fr       */
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
