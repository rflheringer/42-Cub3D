/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 00:00:00 by rheringe          #+#    #+#             */
/*   Updated: 2025/07/29 16:31:07 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"
#include <math.h>

// Função para normalizar os vetores de direção e câmera
static void normalize_vectors(t_game *game)
{
    // Normalizar vetores de direção
    double dir_len = sqrt(game->player->player_dir_x * game->player->player_dir_x + 
                         game->player->player_dir_y * game->player->player_dir_y);
    if (dir_len > 0) {
        game->player->player_dir_x /= dir_len;
        game->player->player_dir_y /= dir_len;
    }
    
    // Normalizar vetores de câmera
    double cam_len = sqrt(game->player->camera_dir_x * game->player->camera_dir_x + 
                         game->player->camera_dir_y * game->player->camera_dir_y);
    if (cam_len > 0) {
        game->player->camera_dir_x /= cam_len;
        game->player->camera_dir_y /= cam_len;
    }
}

// Função para lidar com o movimento do jogador
void handle_movement(void *param)
{
    t_game *game = (t_game *)param;
    bool moved = false;
    
    // Movimento para frente
    if (game->player->up)
    {
        double new_x = game->player->pos_x + game->player->player_dir_x * game->player->move_speed;
        double new_y = game->player->pos_y + game->player->player_dir_y * game->player->move_speed;
        
        // Adicionar margem de segurança para evitar aproximação excessiva
        double buffer = 0.4; // Aumentado para 0.4
        
        if (game->map->map[(int)new_y][(int)new_x] != '1' && 
            game->map->map[(int)(new_y + buffer * (new_y > game->player->pos_y ? 1 : -1))][(int)new_x] != '1' &&
            game->map->map[(int)new_y][(int)(new_x + buffer * (new_x > game->player->pos_x ? 1 : -1))] != '1')
        {
            game->player->pos_x = new_x;
            game->player->pos_y = new_y;
            moved = true;
        }
    }
    
    // Movimento para trás
    if (game->player->down)
    {
        double new_x = game->player->pos_x - game->player->player_dir_x * game->player->move_speed;
        double new_y = game->player->pos_y - game->player->player_dir_y * game->player->move_speed;
        
        // Adicionar a mesma margem de segurança para movimento para trás
        double buffer = 0.4;
        
        if (game->map->map[(int)new_y][(int)new_x] != '1' && 
            game->map->map[(int)(new_y + buffer * (new_y > game->player->pos_y ? 1 : -1))][(int)new_x] != '1' &&
            game->map->map[(int)new_y][(int)(new_x + buffer * (new_x > game->player->pos_x ? 1 : -1))] != '1')
        {
            game->player->pos_x = new_x;
            game->player->pos_y = new_y;
            moved = true;
        }
    }
    
    // Movimento para a esquerda (perpendicular à direção)
    if (game->player->rot_left)
    {
        double new_x = game->player->pos_x - game->player->camera_dir_x * game->player->move_speed;
        double new_y = game->player->pos_y - game->player->camera_dir_y * game->player->move_speed;
        
        double buffer = 0.4;
        
        if (game->map->map[(int)new_y][(int)new_x] != '1' && 
            game->map->map[(int)(new_y + buffer * (new_y > game->player->pos_y ? 1 : -1))][(int)new_x] != '1' &&
            game->map->map[(int)new_y][(int)(new_x + buffer * (new_x > game->player->pos_x ? 1 : -1))] != '1')
        {
            game->player->pos_x = new_x;
            game->player->pos_y = new_y;
            moved = true;
        }
    }
    
    // Movimento para a direita (perpendicular à direção)
    if (game->player->rot_right)
    {
        double new_x = game->player->pos_x + game->player->camera_dir_x * game->player->move_speed;
        double new_y = game->player->pos_y + game->player->camera_dir_y * game->player->move_speed;
        
        double buffer = 0.4;
        
        if (game->map->map[(int)new_y][(int)new_x] != '1' && 
            game->map->map[(int)(new_y + buffer * (new_y > game->player->pos_y ? 1 : -1))][(int)new_x] != '1' &&
            game->map->map[(int)new_y][(int)(new_x + buffer * (new_x > game->player->pos_x ? 1 : -1))] != '1')
        {
            game->player->pos_x = new_x;
            game->player->pos_y = new_y;
            moved = true;
        }
    }
    
    // Rotação para a esquerda
    if (game->player->rot_left)
    {
        double old_dir_x = game->player->player_dir_x;
        game->player->player_dir_x = game->player->player_dir_x * cos(game->player->rotation_speed) - 
                                     game->player->player_dir_y * sin(game->player->rotation_speed);
        game->player->player_dir_y = old_dir_x * sin(game->player->rotation_speed) + 
                                     game->player->player_dir_y * cos(game->player->rotation_speed);
        double old_plane_x = game->player->camera_dir_x;
        game->player->camera_dir_x = game->player->camera_dir_x * cos(game->player->rotation_speed) - 
                                    game->player->camera_dir_y * sin(game->player->rotation_speed);
        game->player->camera_dir_y = old_plane_x * sin(game->player->rotation_speed) + 
                                    game->player->camera_dir_y * cos(game->player->rotation_speed);
        moved = true;
        
        // Normalizar vetores após a rotação
        normalize_vectors(game);
    }
    
    // Rotação para a direita
    if (game->player->rot_right)
    {
        double old_dir_x = game->player->player_dir_x;
        game->player->player_dir_x = game->player->player_dir_x * cos(-game->player->rotation_speed) - 
                                     game->player->player_dir_y * sin(-game->player->rotation_speed);
        game->player->player_dir_y = old_dir_x * sin(-game->player->rotation_speed) + 
                                     game->player->player_dir_y * cos(-game->player->rotation_speed);
        double old_plane_x = game->player->camera_dir_x;
        game->player->camera_dir_x = game->player->camera_dir_x * cos(-game->player->rotation_speed) - 
                                    game->player->camera_dir_y * sin(-game->player->rotation_speed);
        game->player->camera_dir_y = old_plane_x * sin(-game->player->rotation_speed) + 
                                    game->player->camera_dir_y * cos(-game->player->rotation_speed);
        moved = true;
        
        // Normalizar vetores após a rotação
        normalize_vectors(game);
    }
    
    // Se houve movimento, atualizar o raycasting
    if (moved)
    {
        // Limpar a imagem anterior
        if (game->raycasting->image)
            mlx_delete_image(game->mlx, game->raycasting->image);
        
        // Refazer o raycasting
        perform_raycasting(game);
    }
}