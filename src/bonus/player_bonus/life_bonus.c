/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 15:18:49 by rheringe          #+#    #+#             */
/*   Updated: 2025/08/15 14:21:03 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

void	put_sq_red(mlx_image_t *img, int x, int y, int s)
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

static int	life_ready(t_game *game)
{
	if (!game || !game->player || !game->raycasting
		|| !game->raycasting->image)
		return (0);
	return (1);
}

static void	life_ui_compute(t_life_ui *ui)
{
	ui->target_h = (HEIGHT * 40) / 100;
	if (ui->target_h < 120)
		ui->target_h = 120;
	ui->margin = 14;
	ui->gap = 4;
	ui->scale_cell = (ui->target_h - ui->gap * (LIFE_MAX - 1))
		/ (HEART_H * LIFE_MAX);
	if (ui->scale_cell < 1)
		ui->scale_cell = 1;
	ui->heart_w = HEART_W * ui->scale_cell;
	ui->heart_h = HEART_H * ui->scale_cell;
	ui->total_h = ui->heart_h * LIFE_MAX + ui->gap * (LIFE_MAX - 1);
	ui->y0 = ui->margin + (ui->target_h - ui->total_h) / 2;
	ui->x = WIDTH - ui->heart_w - (ui->margin + 2);
}

void	draw_life(t_game *game)
{
	mlx_image_t	*img;
	t_life_ui	ui;
	int			filled;
	int			i;
	int			iy;

	if (!life_ready(game))
		return ;
	img = game->raycasting->image;
	life_ui_compute(&ui);
	filled = game->player->hp;
	if (filled < 0)
		filled = 0;
	if (filled > LIFE_MAX)
		filled = LIFE_MAX;
	i = 0;
	while (i < LIFE_MAX)
	{
		iy = ui.y0 + i * (ui.heart_h + ui.gap);
		if (i < filled)
			heart_fill(img, ui.x, iy, ui.scale_cell);
		heart_outline(img, ui.x, iy, ui.scale_cell);
		i++;
	}
}
