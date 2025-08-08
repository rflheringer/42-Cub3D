/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_move_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:37:18 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/08 17:27:09 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../../include/cub3d_bonus.h"

static bool	can_enemy_move_to(t_game *game, double x, double y, t_enemy_list *current_enemy)
{
	t_enemy_list	*tmp;

	if (game->map->map[(int)floor(y + 0.2)][(int)floor(x + 0.2)] == '1')
		return (false);
	if (game->map->map[(int)floor(y + 0.2)][(int)floor(x - 0.2)] == '1')
		return (false);
	if (game->map->map[(int)floor(y - 0.2)][(int)floor(x + 0.2)] == '1')
		return (false);
	if (game->map->map[(int)floor(y - 0.2)][(int)floor(x - 0.2)] == '1')
		return (false);
	tmp = game->enemy->list;
	while (tmp)
	{
		if (tmp != current_enemy)
		{
			double dx = x - tmp->pos_x;
			double dy = y - tmp->pos_y;
			double distance = sqrt(dx * dx + dy * dy);
			if (distance < 0.6)
				return (false);
		}
		tmp = tmp->next;
	}
	return (true);
}

static void	enemy_move(t_game *game, t_enemy_list *enemy, double dx, double dy)
{
	double	inv_len;
	double	dirx;
	double	diry;
	double	next_x;
	double	next_y;

	inv_len = 1.0 / enemy->distance;
	dirx = dx * inv_len;
	diry = dy * inv_len;
	next_x = enemy->pos_x + dirx * game->enemy->move_speed;
	next_y = enemy->pos_y + diry * game->enemy->move_speed;
	
	// Tentar movimento direto primeiro
	if (can_enemy_move_to(game, next_x, next_y, enemy))
	{
		enemy->pos_x = next_x;
		enemy->pos_y = next_y;
		enemy->move_delay = 0;  // Reset delay quando consegue se mover
		return ;
	}

	// Se não conseguir ir direto, tentar movimento perpendicular (contorno)
	double alt_x1 = enemy->pos_x + diry * game->enemy->move_speed;
	double alt_y1 = enemy->pos_y - dirx * game->enemy->move_speed;
	if (can_enemy_move_to(game, alt_x1, alt_y1, enemy))
	{
		enemy->pos_x = alt_x1;
		enemy->pos_y = alt_y1;
		enemy->move_delay = 0;  // Reset delay quando consegue se mover
		return ;
	}
	
	// Tentar movimento perpendicular no sentido oposto
	double alt_x2 = enemy->pos_x - diry * game->enemy->move_speed;
	double alt_y2 = enemy->pos_y + dirx * game->enemy->move_speed;
	if (can_enemy_move_to(game, alt_x2, alt_y2, enemy))
	{
		enemy->pos_x = alt_x2;
		enemy->pos_y = alt_y2;
		enemy->move_delay = 0;  // Reset delay quando consegue se mover
	}
	// Se chegou aqui, não conseguiu se mover em nenhuma direção
	// O move_delay continua acumulando, evitando tentativas muito frequentes
}

static void	calculate_distance_to_player(t_game *game, t_enemy_list *enemy)
{
	double	px;
	double	py;
	double	dx;
	double	dy;

	enemy->move_delay += game->delta_time;
	px = game->player->pos_x;
	py = game->player->pos_y;
	dx = px - enemy->pos_x;
	dy = py - enemy->pos_y;
	enemy->distance = sqrt(dx * dx + dy * dy);
	if (enemy->distance > 0.6 && enemy->move_delay > 0.1)
		enemy_move(game, enemy, dx, dy);
}

static void	calculate_enemie_position(t_game *game, t_enemy_list *enemy)
{
	double spriteX = enemy->pos_x - game->player->pos_x;
	double spriteY = enemy->pos_y - game->player->pos_y;
	
	double invDet = 1.0 / (game->player->camera_dir_x * game->player->player_dir_y - game->player->player_dir_x * game->player->camera_dir_y);
	
	double transformX = invDet * (game->player->player_dir_y * spriteX - game->player->player_dir_x * spriteY);
	double transformY = invDet * (-game->player->camera_dir_y * spriteX + game->player->camera_dir_x * spriteY);

	// Se está atrás da câmera, não renderizar
	if (transformY <= 0)
		return;
	// CORREÇÃO: spriteScreenX sem multiplicação extra
	int spriteScreenX = (int)((WIDTH / 2) * (1 + transformX / transformY));

	// CORREÇÃO: usar HEIGHT para manter proporção
	int spriteHeight = (int)fabs(HEIGHT / transformY);
	
	double dist_proj_plane = (WIDTH / 2.0) / tan(FOV / 2.0);
	int floorHeight = (int)((1.0 / transformY) * dist_proj_plane);
	int floorDrawEnd = (HEIGHT / 2 + floorHeight / 2);
	// Fator de escala para diminuir o tamanho do sprite (0.7 = 70% do tamanho original)
	double scaleFactor = 0.7;
	spriteHeight = (int)(spriteHeight * scaleFactor);

	// Calcular onde o chão aparece na perspectiva (mesma lógica das paredes)
	
	// Sprite "em pé" no chão - base toca onde o chão seria desenhado
	// int drawEndY = spriteHeight / 2 + HEIGHT / 2;
	int drawEndY = floorDrawEnd;
	int drawStartY = (-spriteHeight / 2 + HEIGHT / 2);
	if (drawStartY < 0) 
		drawStartY = 0;
	if (drawEndY >= HEIGHT) 
		drawEndY = HEIGHT - 1;

	// Calcular largura e posições X
	int spriteWidth = (int)fabs(HEIGHT / transformY);
	spriteWidth = (int)(spriteWidth * scaleFactor); // Aplicar mesmo fator de escala
	int drawStartX = -spriteWidth / 2 + spriteScreenX;
	int originalDrawStartX = drawStartX; // Guardar posição original para texX
	if (drawStartX < 0) 
		drawStartX = 0;
	int drawEndX = spriteWidth / 2 + spriteScreenX;
	if (drawEndX >= WIDTH) 
		drawEndX = WIDTH - 1;

	mlx_texture_t *texture = game->enemy->skell_texture[enemy->cur_sprite];
	int stripe = drawStartX;
	while (stripe < drawEndX)
	{
		// Cálculo de texX como no Lode - usando posição original
		int texX = (int)(256 * (stripe - originalDrawStartX) * texture->width / spriteWidth) / 256;
		if (texX < 0) texX = 0;
		if (texX >= (int)texture->width) texX = texture->width - 1;
		
		if (transformY > 0 && stripe >= 0 && stripe < WIDTH
			&& transformY < game->raycasting->buffer[stripe])
		{
			int y = drawStartY;
			while (y < drawEndY)
			{
				// Cálculo de texY como no Lode para evitar floats
				int d = y * 256 - HEIGHT * 128 + spriteHeight * 128; //256 e 128 fatores para evitar floats
				int texY = ((d * texture->height) / spriteHeight - 6000) / 256;
				// Validar bounds da textura
				if (texY < 0) texY = 0;
				if (texY >= (int)texture->height) texY = texture->height - 1;
				
				uint8_t *pixel = &texture->pixels[(texture->width * texY + texX) * texture->bytes_per_pixel];
				uint32_t color = (pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | pixel[3];
				// Verificar transparência (assumindo que alpha = 0 é transparente)
				if (pixel[3] > 0)
					mlx_put_pixel(game->raycasting->image, stripe, y, color);
				y++;
			}
		}
		stripe++;
	}
}

static void	calculate_sprite_change(t_game *game, t_enemy_list *enemy)
{
	enemy->frame_delay += game->delta_time;
	if (enemy->frame_delay > 0.3)
	{
		enemy->cur_sprite = (enemy->cur_sprite + 1) % 3;
		enemy->frame_delay = 0;
	}
}

static bool	ft_swap(t_enemy_list **head, t_enemy_list *cur, t_enemy_list *next)
{
	if (!head || !cur || !next || cur == next)
		return (false);
	if (cur->prev)
		cur->prev->next = next;
	else
		*head = next;
	if (next->next)
		next->next->prev = cur;
	next->prev = cur->prev;
	cur->prev = next;
	cur->next = next->next;
	next->next = cur;
	return (true);
}

static bool	ft_is_sorted(t_game *game)
{
	t_enemy_list	*nav;

	nav = game->enemy->list;
	while (nav->next)
	{
		if (nav->distance < nav->next->distance)
			return (false);
		nav = nav->next;
	}
	return (true);
}

static void	sort_enemies(t_game *game, t_enemy_list *nav)
{
	if (!nav->next)
		return ;
	while (nav)
	{
		calculate_distance_to_player(game, nav);
		nav = nav->next;
	}
	while (!ft_is_sorted(game))
	{
		nav = game->enemy->list;
		while (nav->next)
		{
			if (nav->distance < nav->next->distance)
				if(ft_swap(&game->enemy->list, nav, nav->next))
					break ;
			nav = nav->next;
		}
	}
}

void	manage_enemies(t_game *game)
{
	t_enemy_list	*nav;

	if (game->enemy->list == NULL)
		return ;
	sort_enemies(game, game->enemy->list);
	nav = game->enemy->list;
	while (nav)
	{
		calculate_sprite_change(game, nav);
		calculate_enemie_position(game, nav);
		nav = nav->next;
	}
}
