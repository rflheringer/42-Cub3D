/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_fov_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 12:52:47 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/14 15:51:32 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static void	draw_lines(mlx_image_t *img, t_fov *vars, int x1, int y1)
{
	while (1)
	{
		if (vars->cx >= 0 && vars->cx < (int)img->width && vars->cy >= 0
			&& vars->cy < (int)img->height)
			mlx_put_pixel(img, vars->cx, vars->cy, 0xFFFF00AA);
		if (vars->cx == x1 && vars->cy == y1)
			break ;
		vars->e2 = vars->err;
		if (vars->e2 > -vars->dx)
		{
			vars->err -= vars->dy;
			vars->cx += vars->sx;
		}
		if (vars->e2 < vars->dy)
		{
			vars->err += vars->dx;
			vars->cy += vars->sy;
		}
	}
}

static void	validate_vars(mlx_image_t *img, t_fov *vars, int x1, int y1)
{
	if (!img)
		return ;
	vars->dx = x1 - vars->cx;
	if (vars->dx < 0)
		vars->dx = -vars->dx;
	vars->dy = y1 - vars->cy;
	if (vars->dy < 0)
		vars->dy = -vars->dy;
	vars->sx = 1;
	if (vars->cx >= x1)
		vars->sx = -1;
	vars->sy = 1;
	if (vars->cy >= y1)
		vars->sy = -1;
	vars->err = vars->dx;
	if (vars->dx <= vars->dy)
		vars->err = -vars->dy;
	vars->err /= 2;
	draw_lines(img, vars, x1, y1);
}

static void	fill_triangle_mm(mlx_image_t *img, t_fov *vars)
{
	int		x;
	int		y;
	long	w0;
	long	w1;
	long	w2;

	y = vars->miny;
	while (y <= vars->maxy)
	{
		x = vars->minx;
		while (x <= vars->maxx)
		{
			w0 = (long)((x - vars->xl) * (vars->yr - vars->yl) - (y - vars->yl)
					* (vars->xr - vars->xl));
			w1 = (long)((x - vars->xr) * (vars->cy - vars->yr) - (y - vars->yr)
					* (vars->cx - vars->xr));
			w2 = (long)((x - vars->cx) * (vars->yl - vars->cy) - (y - vars->cy)
					* (vars->xl - vars->cx));
			if (!((w0 < 0 || w1 < 0 || w2 < 0) && (w0 > 0 || w1 > 0 || w2 > 0)))
				mlx_put_pixel(img, x, y, 0xFFFF0066);
			x += 1;
		}
		y += 1;
	}
}

static void	validate_triangle(mlx_image_t *img, t_fov *vars)
{
	if (vars->xl < vars->minx)
		vars->minx = vars->xl;
	if (vars->xr < vars->minx)
		vars->minx = vars->xr;
	if (vars->xl > vars->maxx)
		vars->maxx = vars->xl;
	if (vars->xr > vars->maxx)
		vars->maxx = vars->xr;
	if (vars->yl < vars->miny)
		vars->miny = vars->yl;
	if (vars->yr < vars->miny)
		vars->miny = vars->yr;
	if (vars->yl > vars->maxy)
		vars->maxy = vars->yl;
	if (vars->yr > vars->maxy)
		vars->maxy = vars->yr;
	if (vars->minx < 0)
		vars->minx = 0;
	if (vars->miny < 0)
		vars->miny = 0;
	if (vars->maxx > (int)img->width - 1)
		vars->maxx = (int)img->width - 1;
	if (vars->maxy > (int)img->height - 1)
		vars->maxy = (int)img->height - 1;
	fill_triangle_mm(img, vars);
}

void	calculate_variables(t_game *game, int len, int scale)
{
	t_fov	*vars;

	vars = ft_calloc(1, sizeof(t_fov));
	vars->cx = cxx(game, scale);
	vars->cy = cyy(game, scale);
	vars->xl = vars->cx + (int)((game->player->player_dir_x
				- game->player->camera_dir_x * 0.60) * len);
	vars->yl = vars->cy + (int)((game->player->player_dir_y
				- game->player->camera_dir_y * 0.60) * len);
	vars->xr = vars->cx + (int)((game->player->player_dir_x
				+ game->player->camera_dir_x * 0.60) * len);
	vars->yr = vars->cy + (int)((game->player->player_dir_y
				+ game->player->camera_dir_y * 0.60) * len);
	vars->minx = vars->cx;
	vars->maxx = vars->cx;
	vars->miny = vars->cy;
	vars->maxy = vars->cy;
	validate_triangle(game->raycasting->image, vars);
	validate_vars(game->raycasting->image, vars, vars->xl, vars->yl);
	validate_vars(game->raycasting->image, vars, vars->xr, vars->yr);
	free(vars);
}
