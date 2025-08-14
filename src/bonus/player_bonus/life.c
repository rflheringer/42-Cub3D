/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 15:18:49 by rheringe          #+#    #+#             */
/*   Updated: 2025/08/14 17:42:42 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"
#include <stdint.h>

#define HEART_W 11
#define HEART_H 9

static void	put_rect(mlx_image_t *img, int x, int y, int w, int h, uint32_t c)
{
	int				i;
	int				j;

	if (!img || w <= 0 || h <= 0)
		return ;
	j = 0;
	while (j < h)
	{
		i = 0;
		while (i < w)
		{
			if (x + i >= 0 && x + i < (int)img->width
				&& y + j >= 0 && y + j < (int)img->height)
				mlx_put_pixel(img, x + i, y + j, c);
			i++;
		}
		j++;
	}
}

static int	has_fill_neighbor(int i, int j, uint8_t heart_mask[HEART_H][HEART_W])
{
	int	delta_i;
	int	delta_j;
	int	ii;
	int	jj;

	delta_j = -2;
	while (delta_j++, delta_j <= 1)
	{
		delta_i = -2;
		while (delta_i++, delta_i <= 1)
		{
			if (!(delta_i == 0 && delta_j == 0))
			{
				ii = i + delta_i;
				jj = j + delta_j;
				if (ii >= 0 && ii < HEART_W && jj >= 0 && jj < HEART_H)
				{
					if (heart_mask[jj][ii] == 1)
						return (1);
				}
			}
		}
	}
	return (0);
}

static void	draw_heart_icon(mlx_image_t *img, int x, int y, int s, int filled)
{
	int				i;
	int				j;
	int				cx;
	int				cy;
	static uint8_t	heart_mask[HEART_H][HEART_W] = {
	{0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0},
	{0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
	{0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
	{0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}
	};

	j = 0;
	while (j < HEART_H)
	{
		i = 0;
		while (i < HEART_W)
		{
			if (heart_mask[j][i] == 0 && has_fill_neighbor(i, j, heart_mask))
			{
				if (!(j == 0 && (i == 0 || i == 5 || i == HEART_W - 1)))
				{
					cx = x + i * s;
					cy = y + j * s;
					put_rect(img, cx, cy, s, s, 0x000000FF);
				}
			}
			i++;
		}
		j++;
	}
	if (filled)
	{
		j = 0;
		while (j < HEART_H)
		{
			i = 0;
			while (i < HEART_W)
			{
				if (heart_mask[j][i] == 1)
				{
					cx = x + i * s;
					cy = y + j * s;
					put_rect(img, cx, cy, s, s, 0xFF0000FF);
				}
				i++;
			}
			j++;
		}
	}
}

void	draw_life(t_game *game)
{
	mlx_image_t	*img;
	int			target_h;
	int			margin;
	int			gap;
	int			scale_cell;
	int			heart_w;
	int			heart_h;
	int			total_h;
	int			filled;
	int			i;
	int			x;
	int			y0;
	int			iy;

	if (!game || !game->player || !game->raycasting
		|| !game->raycasting->image)
		return ;
	img = game->raycasting->image;
	target_h = (int)(HEIGHT * 0.40);
	if (target_h < 120)
		target_h = 120;
	margin = 14;
	gap = 4;
	scale_cell = (target_h - gap * (LIFE_MAX - 1)) / (HEART_H * LIFE_MAX);
	if (scale_cell < 1)
		scale_cell = 1;
	heart_w = HEART_W * scale_cell;
	heart_h = HEART_H * scale_cell;
	total_h = heart_h * LIFE_MAX + gap * (LIFE_MAX - 1);
	y0 = margin + (target_h - total_h) / 2;
	x = WIDTH - heart_w - (margin + 2);
	filled = game->player->hp;
	if (filled < 0)
		filled = 0;
	if (filled > LIFE_MAX)
		filled = LIFE_MAX;
	i = 0;
	while (i < LIFE_MAX)
	{
		iy = y0 + i * (heart_h + gap);
		draw_heart_icon(img, x, iy, scale_cell, (i < filled));
		i++;
	}
}
