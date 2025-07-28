/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 14:03:07 by rheringe          #+#    #+#             */
/*   Updated: 2025/07/28 18:08:42 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

//kill this monster!!!!!

static void init_ray(t_game *game, int x)
{
	// Calcular a posição x no espaço da câmera
	double camera_x = 2 * x / (double)WIDTH - 1;
	
	// Direção do raio
	game->raycasting->ray.ray_dir_x = game->player->player_dir_x + 
									 game->player->camera_dir_x * camera_x;
	game->raycasting->ray.ray_dir_y = game->player->player_dir_y + 
									 game->player->camera_dir_y * camera_x;
	
	// Posição atual no mapa
	game->raycasting->ray.map_x = (int)game->player->pos_x;
	game->raycasting->ray.map_y = (int)game->player->pos_y;
	
	// PROTEÇÃO CONTRA DIVISÃO POR ZERO
	if (game->raycasting->ray.ray_dir_x == 0)
		game->raycasting->ray.delta_dist_x = 1e30; // Um número muito grande
	else
		game->raycasting->ray.delta_dist_x = fabs(1 / game->raycasting->ray.ray_dir_x);
	
	if (game->raycasting->ray.ray_dir_y == 0)
		game->raycasting->ray.delta_dist_y = 1e30; // Um número muito grande
	else
		game->raycasting->ray.delta_dist_y = fabs(1 / game->raycasting->ray.ray_dir_y);
	
	// Resto do código igual...
}

static void perform_dda(t_game *game)
{
	// PROTEÇÃO CONTRA LOOP INFINITO
	int max_iterations = 100; // Limite máximo de iterações
	int iterations = 0;
	
	// Executar o algoritmo DDA com limite de iterações
	while (game->raycasting->ray.hit == 0 && iterations < max_iterations)
	{
		// Código existente...
		iterations++;
		
		// VERIFICAÇÃO DE LIMITES MAIS ROBUSTA
		if (game->raycasting->ray.map_y < 0 || game->raycasting->ray.map_x < 0)
		{
			game->raycasting->ray.hit = 1;
			continue;
		}
		
		int map_height = ft_ptrlen(game->map->map);
		if (game->raycasting->ray.map_y >= map_height)
		{
			game->raycasting->ray.hit = 1;
			continue;
		}
		
		int map_width = ft_strlen(game->map->map[game->raycasting->ray.map_y]);
		if (game->raycasting->ray.map_x >= map_width)
		{
			game->raycasting->ray.hit = 1;
			continue;
		}
		
		// Verificar se é uma parede
		if (game->map->map[game->raycasting->ray.map_y][game->raycasting->ray.map_x] == '1')
			game->raycasting->ray.hit = 1;
	}
	
	// PROTEÇÃO CONTRA DIVISÃO POR ZERO NO CÁLCULO DE DISTÂNCIA
	if (game->raycasting->ray.side == 0 && game->raycasting->ray.ray_dir_x != 0)
		game->raycasting->ray.perp_wall_dist = (game->raycasting->ray.map_x - 
											game->player->pos_x + 
											(1 - game->raycasting->ray.step_x) / 2) / 
											game->raycasting->ray.ray_dir_x;
	else if (game->raycasting->ray.ray_dir_y != 0)
		game->raycasting->ray.perp_wall_dist = (game->raycasting->ray.map_y - 
											game->player->pos_y + 
											(1 - game->raycasting->ray.step_y) / 2) / 
											game->raycasting->ray.ray_dir_y;
	else
		game->raycasting->ray.perp_wall_dist = 1.0; // Valor padrão seguro
}

static void calculate_wall(t_game *game)
{
	// PROTEÇÃO CONTRA DIVISÃO POR ZERO
	if (game->raycasting->ray.perp_wall_dist <= 0)
		game->raycasting->ray.perp_wall_dist = 0.1; // Valor mínimo para evitar divisão por zero
	
	// Calcular altura da linha a ser desenhada
	game->raycasting->wall.height = (int)(HEIGHT / game->raycasting->ray.perp_wall_dist);
	
	// Resto do código igual...
}

void perform_raycasting(t_game *game)
{
	int x;
	
	// VERIFICAÇÃO DE ALOCAÇÃO
	if (!game || !game->mlx || !game->raycasting)
		return;
	
	// Criar uma nova imagem para o raycasting
	game->raycasting->image = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	if (!game->raycasting->image)
		return;
	
	// Para cada coluna da tela
	x = 0;
	while (x < WIDTH)
	{
		// Inicializar o raio
		double camera_x = 2 * x / (double)WIDTH - 1;
		game->raycasting->ray.ray_dir_x = game->player->player_dir_x + 
										 game->player->camera_dir_x * camera_x;
		game->raycasting->ray.ray_dir_y = game->player->player_dir_y + 
										 game->player->camera_dir_y * camera_x;
		
		// Posição atual no mapa
		game->raycasting->ray.map_x = (int)game->player->pos_x;
		game->raycasting->ray.map_y = (int)game->player->pos_y;
		
		// Calcular delta_dist
		game->raycasting->ray.delta_dist_x = (game->raycasting->ray.ray_dir_x == 0) ? 
											1e30 : fabs(1 / game->raycasting->ray.ray_dir_x);
		game->raycasting->ray.delta_dist_y = (game->raycasting->ray.ray_dir_y == 0) ? 
											1e30 : fabs(1 / game->raycasting->ray.ray_dir_y);
		
		game->raycasting->ray.hit = 0;
		
		// Calcular step e side_dist
		if (game->raycasting->ray.ray_dir_x < 0)
		{
			game->raycasting->ray.step_x = -1;
			game->raycasting->ray.side_dist_x = (game->player->pos_x - 
											 game->raycasting->ray.map_x) * 
											 game->raycasting->ray.delta_dist_x;
		}
		else
		{
			game->raycasting->ray.step_x = 1;
			game->raycasting->ray.side_dist_x = (game->raycasting->ray.map_x + 1.0 - 
											 game->player->pos_x) * 
											 game->raycasting->ray.delta_dist_x;
		}
		
		if (game->raycasting->ray.ray_dir_y < 0)
		{
			game->raycasting->ray.step_y = -1;
			game->raycasting->ray.side_dist_y = (game->player->pos_y - 
											 game->raycasting->ray.map_y) * 
											 game->raycasting->ray.delta_dist_y;
		}
		else
		{
			game->raycasting->ray.step_y = 1;
			game->raycasting->ray.side_dist_y = (game->raycasting->ray.map_y + 1.0 - 
											 game->player->pos_y) * 
											 game->raycasting->ray.delta_dist_y;
		}
		
		// Executar DDA
		while (game->raycasting->ray.hit == 0)
		{
			if (game->raycasting->ray.side_dist_x < game->raycasting->ray.side_dist_y)
			{
				game->raycasting->ray.side_dist_x += game->raycasting->ray.delta_dist_x;
				game->raycasting->ray.map_x += game->raycasting->ray.step_x;
				game->raycasting->ray.side = 0;
			}
			else
			{
				game->raycasting->ray.side_dist_y += game->raycasting->ray.delta_dist_y;
				game->raycasting->ray.map_y += game->raycasting->ray.step_y;
				game->raycasting->ray.side = 1;
			}
			
			// Verificação de limites
			if (game->raycasting->ray.map_y < 0 || game->raycasting->ray.map_x < 0 || 
				game->raycasting->ray.map_y >= ft_ptrlen(game->map->map) || 
				game->raycasting->ray.map_x >= (int)ft_strlen(game->map->map[game->raycasting->ray.map_y]))
				break;
			
			// Verificar se atingiu uma parede
			if (game->map->map[game->raycasting->ray.map_y][game->raycasting->ray.map_x] == '1')
				game->raycasting->ray.hit = 1;
		}
		
		// Calcular distância perpendicular
		if (game->raycasting->ray.side == 0)
			game->raycasting->ray.perp_wall_dist = (game->raycasting->ray.map_x - 
												game->player->pos_x + 
												(1 - game->raycasting->ray.step_x) / 2) / 
												game->raycasting->ray.ray_dir_x;
		else
			game->raycasting->ray.perp_wall_dist = (game->raycasting->ray.map_y - 
												game->player->pos_y + 
												(1 - game->raycasting->ray.step_y) / 2) / 
												game->raycasting->ray.ray_dir_y;
		
		// Calcular altura da linha
		int line_height;
		if (game->raycasting->ray.perp_wall_dist > 0)
			line_height = (int)(HEIGHT / game->raycasting->ray.perp_wall_dist);
		else
			line_height = HEIGHT;
		
		// Calcular onde desenhar
		int draw_start = -line_height / 2 + HEIGHT / 2;
		if (draw_start < 0) 
			draw_start = 0;
		int draw_end = line_height / 2 + HEIGHT / 2;
		if (draw_end >= HEIGHT) 
			draw_end = HEIGHT - 1;
		
		// Escolher cor
		uint32_t color;
		if (game->raycasting->ray.side == 0)
			color = 0xD7CBA6FF; // Vermelho
		else
			color = 0xC2B692FF; // Vermelho escuro
		
		// Desenhar a linha vertical
		int y = 0;
		while (y < HEIGHT)
		{
			if (y < draw_start)
				mlx_put_pixel(game->raycasting->image, x, y, 0x1A1A1AFF); // black sky
			else if (y < draw_end)
				mlx_put_pixel(game->raycasting->image, x, y, color); // Parede
			else
				mlx_put_pixel(game->raycasting->image, x, y, 0x808080FF); // floor gray
			y++;
		}
		
		x++;
	}
	
	// Colocar a imagem na janela
	mlx_image_to_window(game->mlx, game->raycasting->image, 0, 0);
}
