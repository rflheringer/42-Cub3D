/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:37:21 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/31 18:37:30 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

bool	can_move_to(char **map, double x, double y)
{
	int	ix;
	int	iy;

	ix = (int)floor(x + R);
	iy = (int)floor(y + R);
	if (map[iy][ix] == '1')
		return (false);
	ix = (int)floor(x - R);
	iy = (int)floor(y + R);
	if (map[iy][ix] == '1')
		return (false);
	ix = (int)floor(x + R);
	iy = (int)floor(y - R);
	if (map[iy][ix] == '1')
		return (false);
	ix = (int)floor(x - R);
	iy = (int)floor(y - R);
	if (map[iy][ix] == '1')
		return (false);
	return (true);
}
