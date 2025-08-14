/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bullet_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:25:21 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/14 19:20:35 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static void	calculate_bullet_position(t_game *game, t_attack *bullet)
{
	double	sprite_x;
	double	sprite_y;
	double	inv_det;
	double	transform_x;
	double	transform_y;

	sprite_x = bullet->pos_x - game->player->pos_x;
	sprite_y = bullet->pos_y - game->player->pos_y;
	inv_det = 1.0 / (game->player->camera_dir_x * game->player->player_dir_y
			- game->player->player_dir_x * game->player->camera_dir_y);
	transform_x = inv_det * (game->player->player_dir_y * sprite_x
			- game->player->player_dir_x * sprite_y);
	transform_y = inv_det * (-game->player->camera_dir_y * sprite_x
			+ game->player->camera_dir_x * sprite_y);
	if (transform_y <= 0)
		return ;
	int	sprite_screen_x;
	sprite_screen_x = (int)((WIDTH / 2) * (1 + transform_x / transform_y));
	int	sprite_height;
	sprite_height = (int)fabs(HEIGHT / transform_y);
	double	dist_proj_plane;
	dist_proj_plane = (WIDTH / 2.0) / tan(FOV / 2.0);
	int	floor_height;
	floor_height = (int)((1.0 / transform_y) * dist_proj_plane);
	int	floor_draw_end;
	floor_draw_end = (HEIGHT / 2 + floor_height / 2);
	double	scale_factor;
	scale_factor = 1.5;
	sprite_height = (int)(sprite_height * scale_factor);
	int	draw_end_y;
	draw_end_y = floor_draw_end;
	int	draw_start_y;
	draw_start_y = (-sprite_height / 2 + HEIGHT / 2);
	if (draw_start_y < 0)
		draw_start_y = 0;
	if (draw_end_y >= HEIGHT)
		draw_end_y = HEIGHT - 1;
	int	sprite_width;
	sprite_width = (int)fabs(HEIGHT / transform_y);
	sprite_width = (int)(sprite_width * scale_factor);
	int draw_start_x;
	draw_start_x = -sprite_width / 2 + sprite_screen_x;
	int originaldraw_start_x;
	originaldraw_start_x = draw_start_x;
	if (draw_start_x < 0) 
		draw_start_x = 0;
	int	draw_end_x;
	draw_end_x = sprite_width / 2 + sprite_screen_x;
	if (draw_end_x >= WIDTH) 
		draw_end_x = WIDTH - 1;
	mlx_texture_t *texture;
	texture = game->boss->bullet_textures[bullet->current_frame];
	int	stripe;
	stripe = draw_start_x;
	while (stripe < draw_end_x)
	{
		int texX;
		texX = (int)(256 * (stripe - originaldraw_start_x)
			* texture->width / sprite_width) / 256;
		if (texX < 0) texX = 0;
		if (texX >= (int)texture->width) texX = texture->width - 1;
		
		if (transform_y > 0 && stripe >= 0 && stripe < WIDTH
			&& transform_y < game->raycasting->buffer[stripe])
		{
			int y;
			y = draw_start_y;
			while (y < draw_end_y)
			{
				int	d;
				d = y * 256 - HEIGHT * 128 + sprite_height * 128;
          		int	texY;
				texY = ((d * texture->height) / sprite_height - 2000) / 256;
				if (texY < 0) texY = 0;
				if (texY >= (int)texture->height) texY = texture->height - 1;
				uint8_t *pixel;
				pixel = &texture->pixels[(texture->width * texY + texX)
						* texture->bytes_per_pixel];
				uint32_t color;
				color = (pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | pixel[3];
				if (pixel[3] > 0)
					mlx_put_pixel(game->raycasting->image, stripe, y, color);
				y++;
			}
		}
		stripe++;
	}
}

static int	bullet_move(t_game *game, double x, double y)
{
	double	dx;
	double	dy;
	double	distance;

	dx = x - game->player->pos_x;
	dy = y - game->player->pos_y;
	distance = sqrt(dx * dx + dy * dy);
	if (distance < 0.3)
	{
		game->boss->hit_player = true;
		return (2);
	}
	if (game->map->map[(int)floor(y + 0.1)][(int)floor(x + 0.1)] == '1'
		|| game->map->map[(int)floor(y + 0.1)][(int)floor(x + 0.1)] == 'D')
		return (1);
	if (game->map->map[(int)floor(y + 0.1)][(int)floor(x - 0.1)] == '1'
		|| game->map->map[(int)floor(y + 0.1)][(int)floor(x - 0.1)] == 'D')
		return (1);
	if (game->map->map[(int)floor(y - 0.1)][(int)floor(x + 0.1)] == '1'
		|| game->map->map[(int)floor(y - 0.1)][(int)floor(x + 0.1)] == 'D')
		return (1);
	if (game->map->map[(int)floor(y - 0.1)][(int)floor(x - 0.1)] == '1'
		|| game->map->map[(int)floor(y - 0.1)][(int)floor(x - 0.1)] == 'D')
		return (1);
	return (0);
}

static void	render_wall_hit(t_game *game, t_attack *bullet)
{
	bullet->frame_delay += game->delta_time;
	if (bullet->frame_delay > 0.1)
	{
		bullet->current_frame = (bullet->current_frame + 1) % 4;
		bullet->frame_delay = 0;
		if (bullet->current_frame == 0)
		{
			bullet->state = DEAD;
			return ;
		}
	}
	calculate_bullet_position(game, bullet);
}

static void	render_player_hit(t_game *game, t_attack *bullet)
{
	bullet->frame_delay += game->delta_time;
	if (bullet->frame_delay > 0.05)
	{
		bullet->current_frame = (bullet->current_frame + 1) % 4;
		bullet->frame_delay = 0;
		if (bullet->current_frame == 0)
		{
			bullet->state = DEAD;
			return ;
		}
	}
	calculate_bullet_position(game, bullet);
}

static void	render_bullets(t_game *game, t_attack *bullet)
{
	if (bullet->state == DEAD)
		return ;
	if (bullet->next_tile == 1)
		render_wall_hit(game, bullet);
	else if (bullet->next_tile == 2)
		render_player_hit(game, bullet);
	else if (bullet->state == MOVING)
		calculate_bullet_position(game, bullet);
}

void	update_bullets(t_game *game)
{
	double		next_x;
	double		next_y;
	t_attack	*nav;

	nav = game->bullets;
	while (nav)
	{
		nav->move_delay += game->delta_time;
		if (nav->state == MOVING && nav->move_delay > 0.2)
		{
			next_x = nav->pos_x + nav->dir_x * 0.5;
			next_y = nav->pos_y + nav->dir_y * 0.5;
			nav->next_tile = bullet_move(game, next_x, next_y);
			if (nav->next_tile > 0)
				nav->state = HITED;
			else
			{
				nav->pos_x = next_x;
				nav->pos_y = next_y;
			}
		}
		render_bullets(game, nav);
		nav = nav->next;
	}
}

static void	add_bullet_list(t_game *game, t_attack *bullet)
{
	t_attack	*last;

	if (game->bullets == NULL)
		game->bullets = bullet;
	else
	{
		last = game->bullets;
		while (last->next)
			last = last->next;
		last->next = bullet;
	}
}

void	fire_projectile(t_game *game)
{
	t_attack	*bullet;
	double		inv_len;
	double		dx;
	double		dy;

	bullet = ft_calloc(1, sizeof(t_attack));
	dx = game->player->pos_x - game->boss->pos_x;
	dy = game->player->pos_y - game->boss->pos_y;
	inv_len = 1.0 / game->boss->distance;
	bullet->pos_x = game->boss->pos_x;
	bullet->pos_y = game->boss->pos_y;
	bullet->dir_x = dx * inv_len;
	bullet->dir_y = dy * inv_len;
	bullet->state = MOVING;
	bullet->move_delay = 0.11;
	bullet->frame_delay = 0.0;
	bullet->current_frame = 0;
	add_bullet_list(game, bullet);
}
