/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafaelheringer <rafaelheringer@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:38:01 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/01 22:26:22 by rafaelherin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static void	draw_wall_end(t_game *game, int x, int y, double step)
{
	int			wall_start;
	int			actual_y;
	int			tex_y;
	uint32_t	color;
	uint8_t		*pixel;

	wall_start = HEIGHT / 2 - game->wall->height / 2;
	actual_y = y - wall_start;
	step = (double)game->wall->s_texture->height
		/ game->wall->height;
	tex_y = (int)(actual_y * step);
	if (tex_y < 0)
		tex_y = 0;
	if (tex_y >= (int)game->wall->s_texture->height)
		tex_y = game->wall->s_texture->height - 1;
	pixel = &game->wall->s_texture->pixels[
		(tex_y * game->wall->s_texture->width
			+ game->wall->text_x)
		* game->wall->s_texture->bytes_per_pixel];
	color = (pixel[0] << 24) | (pixel[1] << 16)
		| (pixel[2] << 8) | pixel[3];
	mlx_put_pixel(game->raycasting->image, x, y, color);
}

static void	draw_wall(t_game *game, int x)
{
	int		y;
	double	step;

	step = 0.0;
	y = 0;
	while (y < HEIGHT)
	{
		if (y < game->wall->draw_start)
		{
			mlx_put_pixel(game->raycasting->image, x, y,
				game->texture->ceiling_color_hex);
		}
		else if (y < game->wall->draw_end)
			draw_wall_end(game, x, y, step);
		else
		{
			mlx_put_pixel(game->raycasting->image, x, y,
				game->texture->floor_color_hex);
		}
		y++;
	}
}

void	perform_raycasting(t_game *game)
{
	int	x;

	if (!game || !game->mlx || !game->raycasting)
		return ;
	game->raycasting->image = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	if (!game->raycasting->image)
		return ;
	x = -1;
	while (x++ < WIDTH)
	{
		init_ray(game, x);
		perform_dda(game);
		calculate_wall(game);
		draw_wall(game, x);
	}
	mlx_image_to_window(game->mlx, game->raycasting->image, 0, 0);
}
