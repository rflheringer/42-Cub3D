/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 15:18:49 by rheringe          #+#    #+#             */
/*   Updated: 2025/08/11 17:53:17 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static void	put_rect(mlx_image_t *img, int x, int y, int w, int h, uint32_t c)
{
	int	i;
	int	j;

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

void	draw_life(t_game *game)
{
	int	box_w;
	int	box_h;
	int	seg_gap;
	int	seg_h;
	int	total_h;
	int	i;
	int	x;
	int	y0;
	int	filled;
	int	inner_w;

	if (!game || !game->player || !game->raycasting
		|| !game->raycasting->image)
		return ;
	box_h = (int)(HEIGHT * 0.40);
	if (box_h < 80)
		box_h = 80;
	box_w = (int)(WIDTH * 0.025);
	if (box_w < 24)
		box_w = 24;
	seg_gap = 3;
	seg_h = (box_h - (LIFE_MAX - 1) * seg_gap) / LIFE_MAX;
	if (seg_h < 5)
		seg_h = 5;
	total_h = seg_h * LIFE_MAX + seg_gap * (LIFE_MAX - 1);
	y0 = 14 + (box_h - total_h) / 2;
	x = WIDTH - box_w - 14;
	put_rect(game->raycasting->image, x - 4, 10, box_w + 8, box_h + 8, 0x000000D0);
	put_rect(game->raycasting->image, x - 2, 12, box_w + 4, box_h + 4, 0x111111FF);
	filled = game->player->hp;
	if (filled < 0)
		filled = 0;
	if (filled > LIFE_MAX)
		filled = LIFE_MAX;
	inner_w = box_w - 2;
	i = 0;
	while (i < LIFE_MAX)
	{
		int	seg_y = y0 + i * (seg_h + seg_gap);
		put_rect(game->raycasting->image, x, seg_y, box_w, seg_h, 0x000000FF);
		if (i < filled)
			put_rect(game->raycasting->image, x + 1, seg_y + 1,
				inner_w - 2, seg_h - 2, 0xFF0000FF);
		else
			put_rect(game->raycasting->image, x + 1, seg_y + 1,
				inner_w - 2, seg_h - 2, 0x2A2A2AFF);
		i++;
	}
}
