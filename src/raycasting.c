/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 14:03:07 by rheringe          #+#    #+#             */
/*   Updated: 2025/07/29 16:36:41 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

//kill this monster!!!!!

static uint32_t	get_color_with_alpha(int r, int g, int b, int a)
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
		if(!rgb_value)
			return 1; //colocar erro
	r = ft_atoi(rgb_value[0]);
	g = ft_atoi(rgb_value[1]);
	b = ft_atoi(rgb_value[2]);
	ft_free_matrix(rgb_value);

	return (get_color_with_alpha(r, g, b, 255));
}

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
        // Usar as funções definidas mas não utilizadas
        init_ray(game, x);
        perform_dda(game);
        //calculate_wall(game);
        
        // Desenhar a linha vertical
        int y = 0;
        while (y < HEIGHT)
        {
            if (y < game->raycasting->wall.draw_start)
                mlx_put_pixel(game->raycasting->image, x, y, game->texture->ceiling_color_hex);
            else if (y < game->raycasting->wall.draw_end)
            {
                // Escolher cor da parede baseada na direção
                uint32_t color;
                if (game->raycasting->ray.side == 0)
                    color = 0xD7CBA6FF; // Bege claro
                else
                    color = 0xC2B692FF; // Bege escuro
                    
                mlx_put_pixel(game->raycasting->image, x, y, color);
            }
            else
                mlx_put_pixel(game->raycasting->image, x, y, game->texture->floor_color_hex);
            y++;
        }
        
        x++;
    }
    
    // Colocar a imagem na janela
    mlx_image_to_window(game->mlx, game->raycasting->image, 0, 0);
}
