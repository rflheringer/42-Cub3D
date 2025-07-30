/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 14:03:07 by rheringe          #+#    #+#             */
/*   Updated: 2025/07/30 15:19:36 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static	uint32_t	get_color_with_alpha(int r, int g, int b, int a)
{
	return ((r & 0xFF) << 24 | (g & 0xFF) << 16 | (b & 0xFF) << 8 | (a & 0xFF));
}

uint32_t	convert_rgb_string(char *rgb_str)
{
	char	**rgb_value;
	int		r;
	int		g;
	int		b;

	rgb_value = ft_split(rgb_str, ',');
	if (!rgb_value)
		error_messages(EXIT_INVALID_RGB_COLOR);
	r = ft_atoi(rgb_value[0]);
	g = ft_atoi(rgb_value[1]);
	b = ft_atoi(rgb_value[2]);
	ft_free_matrix(rgb_value);
	return (get_color_with_alpha(r, g, b, 255));
}

static void	calculate_wall(t_game *game)
{
	double	wall_x;
	double	ray_angle;
	double	player_angle;
	double	angle_diff;
	double	corrected_dist;

    if (game->ray->perp_wall_dist <= 0)
		game->ray->perp_wall_dist = 0.1;
    
	ray_angle = atan2(game->ray->ray_dir_y,
			game->ray->ray_dir_x);
    player_angle = atan2(game->player->player_dir_y,
			game->player->player_dir_x);
    angle_diff = ray_angle - player_angle;
	
	while (angle_diff > M_PI)
		angle_diff -= 2 * M_PI;
	while (angle_diff < -M_PI)
		angle_diff += 2 * M_PI;
		
    corrected_dist = game->ray->perp_wall_dist * cos(angle_diff);
    game->wall->height = (int)(HEIGHT / corrected_dist);
    
    game->wall->draw_start = -game->wall->height / 2 + HEIGHT / 2;
    if (game->wall->draw_start < 0)
		game->wall->draw_start = 0;
		
	game->wall->draw_end = game->wall->height / 2 + HEIGHT / 2;
    if (game->wall->draw_end >= HEIGHT)
	{
        game->wall->draw_end = HEIGHT - 1;
	}
	if (game->ray->side == 0)
	{
		if (game->ray->ray_dir_x > 0)
			game->wall->s_texture = game->texture->east_wall;
		else
			game->wall->s_texture = game->texture->west_wall;
	}
	else
	{
		if (game->ray->ray_dir_y > 0)
			game->wall->s_texture = game->texture->south_wall;
		else
			game->wall->s_texture = game->texture->north_wall;
	}
	if (game->ray->side == 0)
		wall_x = game->player->pos_y + game->ray->perp_wall_dist
			* game->ray->ray_dir_y;
	else
		wall_x = game->player->pos_x + game->ray->perp_wall_dist
			* game->ray->ray_dir_x;
	wall_x -= floor(wall_x);
	game->wall->text_x = (int)(wall_x * game->wall->s_texture->width);
	if ((game->ray->side == 0 && game->ray->ray_dir_x < 0) ||
		(game->ray->side == 1 && game->ray->ray_dir_y < 0))
		game->wall->text_x = game->wall->s_texture->width
			- game->wall->text_x - 1;
}

static void init_ray(t_game *game, int x)
{
	double camera_x = 2.0 * x / (double)WIDTH - 1;
	
	game->ray->ray_dir_x = game->player->player_dir_x + 
									 game->player->camera_dir_x * camera_x;
	game->ray->ray_dir_y = game->player->player_dir_y + 
									 game->player->camera_dir_y * camera_x;
	game->ray->map_x = (int)game->player->pos_x;
	game->ray->map_y = (int)game->player->pos_y;
	if (game->ray->ray_dir_x == 0)
		game->ray->delta_dist_x = 1e30;
	else
		game->ray->delta_dist_x = fabs(1 / game->ray->ray_dir_x);
	
	if (game->ray->ray_dir_y == 0)
		game->ray->delta_dist_y = 1e30;
	else
		game->ray->delta_dist_y = fabs(1 / game->ray->ray_dir_y);
	game->ray->hit = 0;
	if (game->ray->ray_dir_x < 0)
	{
		game->ray->step_x = -1;
		game->ray->side_dist_x = (game->player->pos_x - 
										 game->ray->map_x) * 
										 game->ray->delta_dist_x;
	}
	else
	{
		game->ray->step_x = 1;
		game->ray->side_dist_x = (game->ray->map_x + 1.0 - 
										 game->player->pos_x) * 
										 game->ray->delta_dist_x;
	}
	if (game->ray->ray_dir_y < 0)
	{
		game->ray->step_y = -1;
		game->ray->side_dist_y = (game->player->pos_y - 
										 game->ray->map_y) * 
										 game->ray->delta_dist_y;
	}
	else
	{
		game->ray->step_y = 1;
		game->ray->side_dist_y = (game->ray->map_y + 1.0 - 
										 game->player->pos_y) * 
										 game->ray->delta_dist_y;
	}
}

static void perform_dda(t_game *game)
{
	int max_iterations = 100;
	int iterations = 0;
	
	while (game->ray->hit == 0 && iterations < max_iterations)
	{
		 if (game->ray->side_dist_x < game->ray->side_dist_y)
		{
			game->ray->side_dist_x += game->ray->delta_dist_x;
			game->ray->map_x += game->ray->step_x;
			game->ray->side = 0;
		}
		else
		{
			game->ray->side_dist_y += game->ray->delta_dist_y;
			game->ray->map_y += game->ray->step_y;
			game->ray->side = 1;
		}
		iterations++;
		if (game->ray->map_y < 0 || game->ray->map_x < 0)
		{
			game->ray->hit = 1;
			continue;
		}
		int map_height = ft_ptrlen(game->map->map);
		if (game->ray->map_y >= map_height)
		{
			game->ray->hit = 1;
			continue;
		}
		int map_width = ft_strlen(game->map->map[game->ray->map_y]);
		if (game->ray->map_x >= map_width)
		{
			game->ray->hit = 1;
			continue;
		}
		if (game->map->map[game->ray->map_y][game->ray->map_x] == '1')
			game->ray->hit = 1;
	}
	if (game->ray->side == 0 && game->ray->ray_dir_x != 0)
		game->ray->perp_wall_dist = (game->ray->map_x - 
											game->player->pos_x + 
											(1 - game->ray->step_x) / 2) / 
											game->ray->ray_dir_x;
	else if (game->ray->ray_dir_y != 0)
		game->ray->perp_wall_dist = (game->ray->map_y - 
											game->player->pos_y + 
											(1 - game->ray->step_y) / 2) / 
											game->ray->ray_dir_y;
	else
		game->ray->perp_wall_dist = 1.0;
}

void perform_raycasting(t_game *game)
{
	int x;
	
	if (!game || !game->mlx || !game->raycasting)
		return;
	
	game->raycasting->image = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	if (!game->raycasting->image)
		return;
	
	x = 0;
	while (x < WIDTH)
	{
		init_ray(game, x);
		perform_dda(game);
		calculate_wall(game);
		int y = 0;
		while (y < HEIGHT)
		{
			if (y < game->wall->draw_start)
			{
				mlx_put_pixel(game->raycasting->image, x, y, game->texture->ceiling_color_hex);
			}
			else if (y < game->wall->draw_end)
			{
				if (!game->wall->s_texture) {
					error_messages(EXIT_INVALID_RGB_COLOR);
				} 
				else {
					// my texture
					double tex_pos = (y - game->wall->draw_start) / 
									(double)(game->wall->draw_end - game->wall->draw_start);
					int tex_y = (int)(tex_pos * game->wall->s_texture->height);
					uint8_t *pixel = &game->wall->s_texture->pixels[
						(tex_y * game->wall->s_texture->width + game->wall->text_x) * 
						game->wall->s_texture->bytes_per_pixel];
					uint32_t color = (pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | pixel[3];
					mlx_put_pixel(game->raycasting->image, x, y, color);
				}
			}
			else
			{
				mlx_put_pixel(game->raycasting->image, x, y, game->texture->floor_color_hex);
			}
			y++;
		}
		x++;
	}
	mlx_image_to_window(game->mlx, game->raycasting->image, 0, 0);
}
