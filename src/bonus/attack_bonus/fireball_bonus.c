/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fireball_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:25:21 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/11 16:15:18 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static void	calculate_fireball_position(t_game *game, t_attack *fireball)
{
	double	sprite_x;
	double	sprite_y;
	double	inv_det;
	double	transform_x;
	double	transform_y;

	sprite_x = fireball->pos_x - game->player->pos_x;
	sprite_y = fireball->pos_y - game->player->pos_y;

	double distance = sqrt(sprite_x * sprite_x + sprite_y * sprite_y);
    
    // Apply visual offset when fireball is too close to player
    if (distance < 0.4)
    {
        double visual_offset = 0.4 - distance;  // More offset when closer
        sprite_x += fireball->dir_x * visual_offset;
        sprite_y += fireball->dir_y * visual_offset;
    }

	inv_det = 1.0 / (game->player->camera_dir_x * game->player->player_dir_y
			- game->player->player_dir_x * game->player->camera_dir_y);
	transform_x = inv_det * (game->player->player_dir_y * sprite_x
			- game->player->player_dir_x * sprite_y);
	transform_y = inv_det * (-game->player->camera_dir_y * sprite_x
			+ game->player->camera_dir_x * sprite_y);
	// Se está atrás da câmera, não renderizar
	if (transform_y <= 0)
		return ;
	// CORREÇÃO: sprite_screen_x sem multiplicação extra
	int sprite_screen_x;
	sprite_screen_x = (int)((WIDTH / 2) * (1 + transform_x / transform_y));
	// CORREÇÃO: usar HEIGHT para manter proporção
	int sprite_height;
	sprite_height = (int)fabs(HEIGHT / transform_y);
	double dist_proj_plane;
	dist_proj_plane =  (WIDTH / 2.0) / tan(FOV / 2.0);
	int floor_height;
	floor_height = (int)((1.0 / transform_y) * dist_proj_plane);
	int floor_draw_end;
	floor_draw_end = (HEIGHT / 2 + floor_height / 2);
	// Fator de escala para diminuir o tamanho do sprite (0.7 = 70% do tamanho original)
	double scale_factor;
	scale_factor = 0.7;
	sprite_height = (int)(sprite_height * scale_factor);
	// Calcular onde o chão aparece na perspectiva (mesma lógica das paredes)
	// Sprite "em pé" no chão - base toca onde o chão seria desenhado
	// int draw_end_y = sprite_height / 2 + HEIGHT / 2;
	int draw_end_y;
	draw_end_y = floor_draw_end;
	int drawStartY;
	drawStartY = (-sprite_height / 2 + HEIGHT / 2);
	if (drawStartY < 0) 
		drawStartY = 0;
	if (draw_end_y >= HEIGHT) 
		draw_end_y = HEIGHT - 1;
	// Calcular largura e posições X
	int sprite_width;
	sprite_width = (int)fabs(HEIGHT / transform_y);
	sprite_width = (int)(sprite_width * scale_factor); // Aplicar mesmo fator de escala
	int draw_start_x;
	draw_start_x = -sprite_width / 2 + sprite_screen_x;
	int originaldraw_start_x;
	originaldraw_start_x = draw_start_x; // Guardar posição original para texX
	if (draw_start_x < 0) 
		draw_start_x = 0;
	int draw_end_x;
	draw_end_x = sprite_width / 2 + sprite_screen_x;
	if (draw_end_x >= WIDTH) 
		draw_end_x = WIDTH - 1;
	mlx_texture_t *texture;
	texture = game->player->fireball_textures[fireball->current_frame];
	int stripe;
	stripe = draw_start_x;
	while (stripe < draw_end_x)
	{
		// Cálculo de texX como no Lode - usando posição original
		int texX;
		texX = (int)(256 * (stripe - originaldraw_start_x)
			* texture->width / sprite_width) / 256;
		if (texX < 0) texX = 0;
		if (texX >= (int)texture->width) texX = texture->width - 1;
		
		if (transform_y > 0 && stripe >= 0 && stripe < WIDTH
			&& transform_y < game->raycasting->buffer[stripe])
		{
			int y;
			y = drawStartY;
			while (y < draw_end_y)
			{
				// Cálculo de texY como no Lode para evitar floats
				int d; //256 e 128 fatores para evitar floats
				d = y * 256 - HEIGHT * 128 + sprite_height * 128;
          		int texY;
				texY = ((d * texture->height) / sprite_height) / 256;
				// Validar bounds da textura
				if (texY < 0) texY = 0;
				if (texY >= (int)texture->height) texY = texture->height - 1;
				
				uint8_t *pixel;
				pixel = &texture->pixels[(texture->width * texY + texX)
						* texture->bytes_per_pixel];
				uint32_t color;
				color = (pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | pixel[3];
				// Verificar transparência (assumindo que alpha = 0 é transparente)
				if (pixel[3] > 0)
					mlx_put_pixel(game->raycasting->image, stripe, y, color);
				y++;
			}
		}
		stripe++;
	}
}

static int	fireball_move(t_game *game, double x, double y)
{
	t_enemy_list	*tmp;

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
	tmp = game->enemy->list;
	while (tmp)
	{
		double dx = x - tmp->pos_x;
		double dy = y - tmp->pos_y;
		double distance = sqrt(dx * dx + dy * dy);
		if (distance < 0.3)
		{
			tmp->state = HITED;
			return (2);
		}
		tmp = tmp->next;
	}
	return (0);
}

static t_enemy_list	*find_enemy_hited(t_game *game)
{
	t_enemy_list	*nav;

	nav = game->enemy->list;
	while (nav)
	{
		if (nav->state == HITED)
			return (nav);
		nav = nav->next;
	}
	return (NULL);
}

static void	render_wall_hit(t_game *game, t_attack *fireball)
{
	fireball->frame_delay += game->delta_time;
	if (fireball->frame_delay > 0.1)
	{
		fireball->current_frame = (fireball->current_frame + 1) % 5;
		fireball->frame_delay = 0;
		if (fireball->current_frame == 0)
		{
			fireball->state = DEAD;
			return ;
		}
	}
	calculate_fireball_position(game, fireball);
}

static void	render_enemy_hit(t_game *game, t_attack *fireball)
{
	t_enemy_list	*enemy;

	fireball->frame_delay += game->delta_time;
	enemy = find_enemy_hited(game);
	if (enemy)
		enemy->state = DYING;
	if (fireball->frame_delay > 0.1)
	{
		fireball->current_frame = (fireball->current_frame + 1) % 5;
		fireball->frame_delay = 0;
		if (fireball->current_frame == 0)
		{
			fireball->state = DEAD;
			return ;
		}
	}
	calculate_fireball_position(game, fireball);
}

static void	render_fireballs(t_game *game, t_attack *fireball)
{
	if (fireball->state == DEAD)
		return ;
	if (fireball->next_tile == 1)
		render_wall_hit(game, fireball);
	else if (fireball->next_tile == 2)
		render_enemy_hit(game, fireball);
	else if (fireball->state == MOVING)
		calculate_fireball_position(game, fireball);
}

void	update_fireballs(t_game *game)
{
	double		next_x;
	double		next_y;
	t_attack	*nav;

	nav = game->fireballs;
	while (nav)
	{
		nav->move_delay += game->delta_time;
		if (nav->state == MOVING && nav->move_delay > 0.2)
		{
			next_x = nav->pos_x + nav->dir_x * 0.5;
			next_y = nav->pos_y + nav->dir_y * 0.5;
			nav->next_tile = fireball_move(game, next_x, next_y);
			if (nav->next_tile > 0)
				nav->state = HITED;
			else
			{
				nav->pos_x = next_x;
				nav->pos_y = next_y;
			}
		}
		render_fireballs(game, nav);
		nav = nav->next;
	}
}

static void	add_fireball_list(t_game *game, t_attack *fireball)
{
	t_attack	*last;

	if (game->fireballs == NULL)
		game->fireballs = fireball;
	else
	{
		last = game->fireballs;
		while (last->next)
			last = last->next;
		last->next = fireball;
	}
}

void	create_fireball(t_game *game)
{
	t_attack	*fireball;

	fireball = ft_calloc(1, sizeof(t_attack));
	fireball->pos_x = game->player->pos_x;
	fireball->pos_y = game->player->pos_y;
	fireball->dir_x = game->player->player_dir_x;
	fireball->dir_y = game->player->player_dir_y;
	fireball->state = MOVING;
	fireball->frame_delay = 0.0;
	fireball->current_frame = 0;
	add_fireball_list(game, fireball);
}
