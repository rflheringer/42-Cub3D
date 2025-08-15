/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_bonus_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 13:59:59 by rheringe          #+#    #+#             */
/*   Updated: 2025/08/15 14:20:57 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

void	put_sq_blk(mlx_image_t *img, int x, int y, int s)
{
	int	i;
	int	j;

	if (!img || s <= 0)
		return ;
	j = 0;
	while (j < s)
	{
		i = 0;
		while (i < s)
		{
			if (x + i >= 0 && x + i < (int)img->width
				&& y + j >= 0 && y + j < (int)img->height)
				mlx_put_pixel(img, x + i, y + j, 0xFF0000FF);
			i++;
		}
		j++;
	}
}

static int	heart_is_filled(int i, int j)
{
	if (j == 0)
		return (i == 2 || i == 8);
	if (j == 1)
		return ((i >= 1 && i <= 4) || (i >= 6 && i <= 9));
	if (j == 2)
		return (i >= 0 && i <= 10);
	if (j == 3)
		return (i >= 1 && i <= 9);
	if (j == 4)
		return (i >= 2 && i <= 8);
	if (j == 5)
		return (i >= 3 && i <= 7);
	if (j == 6)
		return (i >= 4 && i <= 6);
	if (j == 7)
		return (i == 5);
	if (j == 8)
		return (i == 5);
	return (0);
}

int	has_fill_neighbor(int i, int j)
{
	int	delta_i;
	int	delta_j;
	int	ii;
	int	jj;

	delta_j = -1;
	while (delta_j <= 1)
	{
		delta_i = -1;
		while (delta_i <= 1)
		{
			if (!(delta_i == 0 && delta_j == 0))
			{
				ii = i + delta_i;
				jj = j + delta_j;
				if (ii >= 0 && ii < HEART_W && jj >= 0 && jj < HEART_H)
					if (heart_is_filled(ii, jj))
						return (1);
			}
			delta_i++;
		}
		delta_j++;
	}
	return (0);
}

void	heart_fill(mlx_image_t *img, int x, int y, int s)
{
	int	i;
	int	j;
	int	cx;
	int	cy;

	j = 0;
	while (j < HEART_H)
	{
		i = 0;
		while (i < HEART_W)
		{
			if (heart_is_filled(i, j))
			{
				cx = x + i * s;
				cy = y + j * s;
				put_sq_red(img, cx, cy, s);
			}
			i++;
		}
		j++;
	}
}

void	heart_outline(mlx_image_t *img, int x, int y, int s)
{
	int	i;
	int	j;
	int	cx;
	int	cy;

	j = 0;
	while (j < HEART_H)
	{
		i = 0;
		while (i < HEART_W)
		{
			if (!heart_is_filled(i, j) && has_fill_neighbor(i, j))
			{
				if (!(j == 0 && (i == 0 || i == 5 || i == HEART_W - 1)))
				{
					cx = x + i * s;
					cy = y + j * s;
					put_sq_blk(img, cx, cy, s);
				}
			}
			i++;
		}
		j++;
	}
}
