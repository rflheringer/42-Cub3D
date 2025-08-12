/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_boss_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:27:59 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/12 19:21:32 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

void	calculate_boss_position(t_game *game, double pos_x, double pos_y, mlx_texture_t *texture, int less_height)
{
	double	sprite_x;
	double	sprite_y;
	double	inv_det;
	double	transform_x;
	double	transform_y;

	sprite_x = pos_x - game->player->pos_x;
	sprite_y = pos_y - game->player->pos_y;
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
	scale_factor = 1.0;
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
				texY = ((d * texture->height) / sprite_height - less_height) / 256;
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

void	create_boss(t_game *game, int i, int j)
{
	if (!game->boss || game->boss->b == 1)
		shutdown_program(game, EXIT_TOO_MANY_BOSSES);
	game->boss->b = 1;
	game->boss->hp = 60;
	game->boss->pos_x = j + 0.5;
	game->boss->pos_y = i + 0.5;
	game->boss->state = IDLE;
	game->boss->attack_sprite = 3;
	game->boss->dying_sprite = 7;
	game->boss->move_speed = 0.2;
	game->boss->boss_text = NULL;
	game->boss->hit_player = false;
}

static bool	can_boss_move_to(t_game *game, double x, double y)
{
	t_enemy_list	*tmp;

	if (game->map->map[(int)floor(y + 0.2)][(int)floor(x + 0.2)] == '1'
		|| game->map->map[(int)floor(y + 0.2)][(int)floor(x + 0.2)] == 'D')
		return (false);
	if (game->map->map[(int)floor(y + 0.2)][(int)floor(x - 0.2)] == '1'
		|| game->map->map[(int)floor(y + 0.2)][(int)floor(x - 0.2)] == 'D')
		return (false);
	if (game->map->map[(int)floor(y - 0.2)][(int)floor(x + 0.2)] == '1'
		|| game->map->map[(int)floor(y - 0.2)][(int)floor(x + 0.2)] == 'D')
		return (false);
	if (game->map->map[(int)floor(y - 0.2)][(int)floor(x - 0.2)] == '1'
		|| game->map->map[(int)floor(y - 0.2)][(int)floor(x - 0.2)] == 'D')
		return (false);
	tmp = game->enemy->list;
	while (tmp)
	{
		double dx = x - tmp->pos_x;
		double dy = y - tmp->pos_y;
		double distance = sqrt(dx * dx + dy * dy);
		if (distance < 1)
			return (false);
		tmp = tmp->next;
	}
	return (true);
}

static void	boss_move(t_game *game, double dx, double dy)
{
	double	inv_len;
	double	dirx;
	double	diry;
	double	next_x;
	double	next_y;

	inv_len = 1.0 / game->boss->distance;
	dirx = dx * inv_len;
	diry = dy * inv_len;
	next_x = game->boss->pos_x + dirx * game->boss->move_speed;
	next_y = game->boss->pos_y + diry * game->boss->move_speed;
	
	// Tentar movimento direto primeiro
	if (can_boss_move_to(game, next_x, next_y))
	{
		game->boss->pos_x = next_x;
		game->boss->pos_y = next_y;
		game->boss->move_delay = 0;  // Reset delay quando consegue se mover
		return ;
	}

	// Se não conseguir ir direto, tentar movimento perpendicular (contorno)
	double alt_x1 = game->boss->pos_x + diry * game->boss->move_speed;
	double alt_y1 = game->boss->pos_y - dirx * game->boss->move_speed;
	if (can_boss_move_to(game, alt_x1, alt_y1))
	{
		game->boss->pos_x = alt_x1;
		game->boss->pos_y = alt_y1;
		game->boss->move_delay = 0;  // Reset delay quando consegue se mover
		return ;
	}
	
	// Tentar movimento perpendicular no sentido oposto
	double alt_x2 = game->boss->pos_x - diry * game->boss->move_speed;
	double alt_y2 = game->boss->pos_y + dirx * game->boss->move_speed;
	if (can_boss_move_to(game, alt_x2, alt_y2))
	{
		game->boss->pos_x = alt_x2;
		game->boss->pos_y = alt_y2;
		game->boss->move_delay = 0;  // Reset delay quando consegue se mover
	}
	// Se chegou aqui, não conseguiu se mover em nenhuma direção
	// O move_delay continua acumulando, evitando tentativas muito frequentes
}

static void	calculate_distance_to_player(t_game *game)
{
	double	px;
	double	py;
	double	dx;
	double	dy;

	game->boss->move_delay += game->delta_time;
	game->boss->attack_delay += game->delta_time;
	px = game->player->pos_x;
	py = game->player->pos_y;
	dx = px - game->boss->pos_x;
	dy = py - game->boss->pos_y;
	game->boss->distance = sqrt(dx * dx + dy * dy);
	if (game->boss->state == DYING)
		return ;
	if (game->boss->distance <= 15.0 && game->boss->attack_delay > 3.0)
		game->boss->state = ATTACK;
	else if (game->boss->distance <= 18.0)
		game->boss->state = ALERT;
	if (game->boss->state == ALERT && game->boss->distance > 8.0 && game->boss->move_delay > 0.1)
		boss_move(game, dx, dy);
	else if (game->boss->state == ALERT && game->boss->distance <= 15.0 && game->boss->attack_delay > 3.0)
		game->boss->state = ATTACK;
}

static void	calculate_sprite_change(t_game *game)
{
	if (game->boss->state == ALERT || game->boss->state == ATTACK)
		game->boss->frame_delay += game->delta_time;
	else if (game->boss->state == DYING)
		game->boss->death_delay += game->delta_time;
	if (game->boss->frame_delay > 0.3 && game->boss->state == ALERT)
	{
		game->boss->cur_sprite = (game->boss->cur_sprite + 1) % 3;
		game->boss->frame_delay = 0;
	}
	else if (game->boss->frame_delay > 0.5 && game->boss->state == ATTACK)
	{
		game->boss->attack_sprite++;
		game->boss->frame_delay = 0;
		if (game->boss->attack_sprite > 6)
		{
			fire_projectile(game);
			game->boss->state = IDLE;
			game->boss->attack_delay = 1;
			game->boss->attack_sprite = 3;
		}
	}
	else if (game->boss->death_delay > 0.4 && game->boss->state == DYING)
	{
		game->boss->dying_sprite++;
		game->boss->death_delay = 0;
		if (game->boss->dying_sprite > 9)
			game->boss->state = DEAD;
	}
}

void	manage_boss(t_game *game)
{
	if (game->boss == NULL || game->boss->boss_text == NULL)
		return ;
	calculate_distance_to_player(game);
	if (game->boss->state != DEAD)
		calculate_sprite_change(game);
	if (game->boss->state == ALERT)
		calculate_boss_position(game, game->boss->pos_x, game->boss->pos_y, game->boss->boss_text[game->boss->cur_sprite], 6500);
	else if (game->boss->state == ATTACK)
		calculate_boss_position(game, game->boss->pos_x, game->boss->pos_y, game->boss->boss_text[game->boss->attack_sprite], 6500);
	else if (game->boss->state == DYING)
		calculate_boss_position(game, game->boss->pos_x, game->boss->pos_y, game->boss->boss_text[game->boss->dying_sprite], 6500);
}
