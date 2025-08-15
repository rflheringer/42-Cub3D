/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_position_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 11:38:51 by rheringe          #+#    #+#             */
/*   Updated: 2025/08/15 15:30:50 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static void	put_y_on_screen(t_game *game, t_render *rend, int stripe,
	mlx_texture_t *texture)
{
	int			y;
	int			d;
	int			tex_y;
	uint8_t		*pixel;
	uint32_t	color;

	y = rend->draw_start_y;
	while (y < rend->draw_end_y)
	{
		d = y * 256 - HEIGHT * 128 + rend->sprite_height * 128;
		tex_y = ((d * texture->height) / rend->sprite_height
				- rend->less_height) / 256;
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= (int)texture->height)
			tex_y = texture->height - 1;
		pixel = &texture->pixels[(texture->width * tex_y + rend->tex_x)
			* texture->bytes_per_pixel];
		color = (pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8)
			| pixel[3];
		if (pixel[3] > 0)
			mlx_put_pixel(game->raycasting->image, stripe, y, color);
		y++;
	}
}

void	put_x_on_screen(t_game *game, t_render *rend, mlx_texture_t *texture)
{
	int	stripe;
	int	draw_end_x;

	draw_end_x = rend->sprite_width / 2 + rend->sprite_screen_x;
	if (draw_end_x >= WIDTH)
		draw_end_x = WIDTH - 1;
	stripe = rend->draw_start_x;
	while (stripe < draw_end_x)
	{
		rend->tex_x = (int)(256 * (stripe - rend->original_start_x)
				* texture->width / rend->sprite_width) / 256;
		if (rend->tex_x < 0)
			rend->tex_x = 0;
		if (rend->tex_x >= (int)texture->width)
			rend->tex_x = texture->width - 1;
		if (rend->transform_y > 0 && stripe >= 0 && stripe < WIDTH
			&& rend->transform_y < game->raycasting->buffer[stripe])
			put_y_on_screen(game, rend, stripe, texture);
		stripe++;
	}
}

void	calculate_sprite(t_render *rend)
{
	double	dist_proj_plane;
	int		floor_height;
	int		floor_draw_end;
	double	scale_factor;

	dist_proj_plane = (WIDTH / 2.0) / tan(FOV / 2.0);
	floor_height = (int)((1.0 / rend->transform_y) * dist_proj_plane);
	floor_draw_end = (HEIGHT / 2 + floor_height / 2);
	scale_factor = 0.8;
	rend->sprite_height = (int)(rend->sprite_height * scale_factor);
	rend->draw_end_y = floor_draw_end;
	rend->draw_start_y = (-rend->sprite_height / 2 + HEIGHT / 2);
	if (rend->draw_start_y < 0)
		rend->draw_start_y = 0;
	if (rend->draw_end_y >= HEIGHT)
		rend->draw_end_y = HEIGHT - 1;
	rend->sprite_width = (int)fabs(HEIGHT / rend->transform_y);
	rend->sprite_width = (int)(rend->sprite_width * scale_factor);
	rend->draw_start_x = -rend->sprite_width / 2 + rend->sprite_screen_x;
	rend->original_start_x = rend->draw_start_x;
	if (rend->draw_start_x < 0)
		rend->draw_start_x = 0;
}

void	calculate_enemie_position(t_game *game, double pos_x, double pos_y,
	mlx_texture_t *texture)
{
	t_render	*rend;
	double		inv_det;

	rend = ft_calloc(1, sizeof(t_render));
	rend->less_height = 7000;
	rend->sprite_x = pos_x - game->player->pos_x;
	rend->sprite_y = pos_y - game->player->pos_y;
	inv_det = 1.0 / (game->player->camera_dir_x * game->player->player_dir_y
			- game->player->player_dir_x * game->player->camera_dir_y);
	rend->transform_x = inv_det * (game->player->player_dir_y * rend->sprite_x
			- game->player->player_dir_x * rend->sprite_y);
	rend->transform_y = inv_det * (-game->player->camera_dir_y * rend->sprite_x
			+ game->player->camera_dir_x * rend->sprite_y);
	if (rend->transform_y <= 0)
		return ;
	rend->sprite_height = (int)fabs(HEIGHT / rend->transform_y);
	rend->sprite_screen_x = (int)((WIDTH / 2) * (1 + rend->transform_x
				/ rend->transform_y));
	calculate_sprite(rend);
	put_x_on_screen(game, rend, texture);
	free(rend);
}
