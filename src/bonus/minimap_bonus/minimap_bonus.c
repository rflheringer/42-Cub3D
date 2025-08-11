/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:26:34 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/11 18:05:51 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static void	draw_minimap(t_game *game, int i, int j, int sy)
{
	int	x;
	int	y;
	int	sx;

	sx = 0;
	while (sx < game->raycasting->scale)
	{
		x = 10 + (j * game->raycasting->scale) + sx;
		y = 10 + (i * game->raycasting->scale) + sy;
		if (x < WIDTH && y < HEIGHT)
		{
			if (game->map->map[i][j] != ' ')
				mlx_put_pixel(game->raycasting->image, x, y, 0x000000FF);
			if (game->map->map[i][j] == '1')
				mlx_put_pixel(game->raycasting->image, x, y, 0x00000000);
			if (game->map->map[i][j] == 'D' || game->map->map[i][j] == 'O')
				mlx_put_pixel(game->raycasting->image, x, y, 0xFFFF00FF);
			if (game->map->map[i][j] == 'P')
				mlx_put_pixel(game->raycasting->image, x, y, 0xFFC0CBFF);
			if (game->map->map[i][j] == 'K')
				mlx_put_pixel(game->raycasting->image, x, y, 0x00FFFFFF);
		}
		sx++;
	}
}

static void	draw_minimap_on_screen(t_game *game, int i, int j)
{
	int	sy;

	sy = 0;
	while (sy < game->raycasting->scale)
	{
		draw_minimap(game, i, j, sy);
		sy++;
	}
}

static void	draw_player_on_screen(t_game *game)
{
	int	x;
	int	y;
	int	sx;
	int	sy;

	sy = 0;
	while (sy < game->raycasting->scale)
	{
		sx = 0;
		while (sx < game->raycasting->scale)
		{
			x = 10 + ((int)game->player->pos_x * game->raycasting->scale) + sx;
			y = 10 + ((int)game->player->pos_y * game->raycasting->scale) + sy;
			if (x < WIDTH && y < HEIGHT)
				mlx_put_pixel(game->raycasting->image, x, y, 0x00FF00FF);
			sx++;
		}
		sy++;
	}
}

static void	draw_enemies_on_screen(t_game *game, t_enemy_list *enemy)
{
	int	x;
	int	y;
	int	sx;
	int	sy;

	sy = 0;
	while (sy < game->raycasting->scale)
	{
		sx = 0;
		while (sx < game->raycasting->scale)
		{
			x = 10 + ((int)enemy->pos_x * game->raycasting->scale) + sx;
			y = 10 + ((int)enemy->pos_y * game->raycasting->scale) + sy;
			if (x < WIDTH && y < HEIGHT)
				mlx_put_pixel(game->raycasting->image, x, y, 0xFF0000FF);
			sx++;
		}
		sy++;
	}
}

static void	draw_enemies(t_game *game)
{
	t_enemy_list	*enemy;

	enemy = game->enemy->list;
	if (!enemy)
		return ;
	while (enemy)
	{
		draw_enemies_on_screen(game, enemy);
		enemy = enemy->next;
	}
}

static void	draw_line_mm(mlx_image_t *img, int x0, int y0, int x1, int y1, uint32_t col)
{
	int	dx;
	int	dy;
	int	sx;
	int	sy;
	int	err;
	int	e2;

	if (!img)
		return ;
	dx = x1 - x0;
	if (dx < 0)
		dx = -dx;
	dy = y1 - y0;
	if (dy < 0)
		dy = -dy;
	sx = 1;
	if (x0 >= x1)
		sx = -1;
	sy = 1;
	if (y0 >= y1)
		sy = -1;
	err = dx;
	if (dx <= dy)
		err = -dy;
	err /= 2;
	while (1)
	{
		if (x0 >= 0 && x0 < (int)img->width && y0 >= 0 && y0 < (int)img->height)
			mlx_put_pixel(img, x0, y0, col);
		if (x0 == x1 && y0 == y1)
			break ;
		e2 = err;
		if (e2 > -dx)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dy)
		{
			err += dx;
			y0 += sy;
		}
	}
}

static inline long	edge_func(int px, int py, int ax, int ay, int bx, int by)
{
	return ((long)(px - ax) * (long)(by - ay) - (long)(py - ay) * (long)(bx - ax));
}

static void	fill_triangle_mm(mlx_image_t *img,
	int x0, int y0, int x1, int y1, int x2, int y2, uint32_t col)
{
	int		minx = x0, maxx = x0, miny = y0, maxy = y0;
	int		x, y;
	long	w0, w1, w2;
	int		w = (int)img->width - 1;
	int		h = (int)img->height - 1;

	if (x1 < minx)
		minx = x1;
	if (x2 < minx)
		minx = x2;
	if (x1 > maxx)
		maxx = x1;
	if (x2 > maxx) 
		maxx = x2;
	if (y1 < miny)
		miny = y1;
	if (y2 < miny)
		miny = y2;
	if (y1 > maxy)
		maxy = y1;
	if (y2 > maxy)
		maxy = y2;
	if (minx < 0)
		minx = 0;
	if (miny < 0)
		miny = 0;
	if (maxx > w)
		maxx = w;
	if (maxy > h)
		maxy = h;
	y = miny;
	while (y <= maxy)
	{
		x = minx;
		while (x <= maxx)
		{
			w0 = edge_func(x, y, x1, y1, x2, y2);
			w1 = edge_func(x, y, x2, y2, x0, y0);
			w2 = edge_func(x, y, x0, y0, x1, y1);
			if (!((w0 < 0 || w1 < 0 || w2 < 0) && (w0 > 0 || w1 > 0 || w2 > 0)))
				mlx_put_pixel(img, x, y, col);
			x += 1;
		}
		y += 1;
	}
}
static int	cxx(t_game *game, int scale)
{
	int	tile_x;
	int	px0;
	int	cx;

	tile_x = (int)game->player->pos_x;
	px0 = 10 + tile_x * scale;
	cx = px0 + (scale / 2);
	return (cx);
}

static int	cyy(t_game *game, int scale)
{
	int	tile_y;
	int	py0;
	int	cy;

	tile_y = (int)game->player->pos_y;
	py0 = 10 + tile_y * scale;
	cy = py0 + (scale / 2);
	return (cy);
}

static void	draw_player_fov(t_game *game, int scale)
{
	mlx_image_t	*img;
	int			cx, cy;
	double		len;
	double		dirx, diry;
	double		plx, ply;
	double		k;
	int			xl, yl, xr, yr;
	int			scalee;

	scalee = scale;
	if (!game || !game->player || !game->raycasting)
		return ;
	img = game->raycasting->image;
	cx = cxx(game, scalee);
	cy = cyy(game, scalee);
	len = scale * 3.0;
	if (len < 16.0)
		len = 16.0;
	dirx = game->player->player_dir_x;
	diry = game->player->player_dir_y;
	plx = game->player->camera_dir_x;
	ply = game->player->camera_dir_y;
	k = 0.60;
	xl = cx + (int)((dirx - plx * k) * len);
	yl = cy + (int)((diry - ply * k) * len);
	xr = cx + (int)((dirx + plx * k) * len);
	yr = cy + (int)((diry + ply * k) * len);
	fill_triangle_mm(img, cx, cy, xl, yl, xr, yr, 0xFFFF0066);
	draw_line_mm(img, cx, cy, xl, yl, 0xFFFF00AA);
	draw_line_mm(img, cx, cy, xr, yr, 0xFFFF00AA);
}

void	update_minimap(t_game *game)
{
	int	i;
	int	j;

	game->raycasting->scale = game->map->height * 0.10;
	if (game->raycasting->scale > 5)
		game->raycasting->scale = 2;
	else
		game->raycasting->scale = 5;
	i = 0;
	while (game->map->map[i])
	{
		j = 0;
		while (game->map->map[i][j])
		{
			draw_minimap_on_screen(game, i, j);
			j++;
		}
		i++;
	}
	draw_enemies(game);
	draw_player_on_screen(game);
	draw_player_fov(game, game->raycasting->scale);
}
