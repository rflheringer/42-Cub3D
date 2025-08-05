/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_move_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:37:18 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/05 19:18:01 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../../../include/cub3d_bonus.h"

static bool	validate_moves(t_game *game, t_enemy_list *enemy, double next_x,
		double next_y)
{
	if (game->map->map[(int)enemy->pos_y][(int)next_x] == '1')
		return (false);
	if (game->map->map[(int)next_y][(int)enemy->pos_x] == '1')
		return (false);
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
	if (!validate_moves(game, enemy, next_x, next_y))
		return ;
	enemy->pos_x = next_x;
	enemy->pos_y = next_y;
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
	if (enemy->distance > 0.4 && enemy->move_delay > 20)
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
	printf("Enemy at (%.2f, %.2f), transformY: %.2f\n", enemy->pos_x, enemy->pos_y, transformY);
	// CORREÇÃO: spriteScreenX sem multiplicação extra
	int spriteScreenX = (int)((WIDTH / 2) * (1 + transformX / transformY));

	// CORREÇÃO: usar HEIGHT para manter proporção
	int spriteHeight = (int)fabs(HEIGHT / transformY);
	int drawStartY = -spriteHeight / 2 + HEIGHT / 2;
	if (drawStartY < 0)
		drawStartY = 0;
	int drawEndY = spriteHeight / 2 + HEIGHT / 2;
	if (drawEndY >= HEIGHT)
		drawEndY = HEIGHT - 1;

	int spriteWidth = (int)fabs(HEIGHT / transformY);
	int drawStartX = -spriteWidth / 2 + spriteScreenX;
	if (drawStartX < 0)
		drawStartX = 0;
	int drawEndX = spriteWidth / 2 + spriteScreenX;
	if (drawEndX >= WIDTH)
		drawEndX = WIDTH - 1;

	mlx_texture_t *texture = game->enemy->skell_texture[0];
	int stripe = drawStartX;
	while (stripe < drawEndX)
	{
		int texX = (int)(texture->width * (stripe - (-spriteWidth / 2 + spriteScreenX)) / spriteWidth);
		if (texX < 0) texX = 0;
		if (texX >= (int)texture->width) texX = texture->width - 1;
		
		if (transformY > 0 && stripe >= 0 && stripe < WIDTH
			&& transformY < game->raycasting->buffer[stripe])
		{
			int y = drawStartY;
			while (y < drawEndY)
			{
				int d = (y) * 256 - HEIGHT * 128 + spriteHeight * 128;
				int texY = ((d * texture->height) / spriteHeight) / 256;
				
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


void	manage_enemies(t_game *game)
{
	t_enemy_list	*nav;

	if (!game->enemy->list)
		return ;
	nav = game->enemy->list;
	while (nav)
	{
		calculate_distance_to_player(game, nav);
		calculate_enemie_position(game, nav);
		nav = nav->next;
	}
}
