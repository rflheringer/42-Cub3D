/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:45:53 by rheringe          #+#    #+#             */
/*   Updated: 2025/07/29 13:16:57 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	keypress(mlx_key_data_t keydata, void *param)
{
	t_game *game;
	
	game = (t_game *)param;
	
	if (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT)
	{
		if (keydata.key == MLX_KEY_W)
			game->player->up = true;
		else if (keydata.key == MLX_KEY_S)
			game->player->down = true;
		else if (keydata.key == MLX_KEY_A)
			game->player->rot_left = true;
		else if (keydata.key == MLX_KEY_D)
			game->player->rot_right = true;
	}
	else if (keydata.action == MLX_RELEASE)
	{
		if (keydata.key == MLX_KEY_W)
			game->player->up = false;
		else if (keydata.key == MLX_KEY_S)
			game->player->down = false;
		else if (keydata.key == MLX_KEY_A)
			game->player->rot_left = false;
		else if (keydata.key == MLX_KEY_D)
			game->player->rot_right =  false;
	}
}

void handle_movement(void *param)
{
    t_game *game = (t_game *)param;
    bool moved = false;
    
    // Movimento para frente
    if (game->player->up)
    {
        double new_x = game->player->pos_x + game->player->player_dir_x * game->player->move_speed;
        double new_y = game->player->pos_y + game->player->player_dir_y * game->player->move_speed;
        
        // Verificação de colisão básica
        if (game->map->map[(int)new_y][(int)new_x] != '1')
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
        
        if (game->map->map[(int)new_y][(int)new_x] != '1')
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
    }
    
    // Se houve movimento, atualizar a visualização
    if (moved)
    {
        // Limpar imagem anterior se existir
        if (game->raycasting->image)
            mlx_delete_image(game->mlx, game->raycasting->image);
            
        // Realizar raycasting novamente para atualizar a visualização
        perform_raycasting(game);
    }
}
