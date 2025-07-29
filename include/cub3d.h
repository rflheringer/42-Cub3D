/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 17:52:53 by rheringe          #+#    #+#             */
/*   Updated: 2025/07/29 16:35:52 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../MLX42/include/MLX42/MLX42.h"
# include "../lib/includes/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <math.h>

# define WIDTH 800
# define HEIGHT 600
# define W_NAME "DOOM 42"
# define FOV (M_PI / 3)
# define EXIT_TOO_MANY_ARGUMENTS 3
# define EXIT_FEW_ARGUMENTS 4
# define EXIT_ERROR_MEMORY_ALLOCATION 5
# define EXIT_INVALID_EXTENSION 6
# define EXIT_INVALID_FILE 7
# define EXIT_FAILED_TO_LOAD_TEXTURE 8
# define EXIT_FAILED_TO_LOAD_IMAGE 9

typedef struct s_player
{
	char	*pos;
	bool	up;
	bool	down;
	bool	rot_left;
	bool	rot_right;
	double	pos_x;
	double	pos_y;
	double	player_dir_x;
	double	player_dir_y;
	double	camera_dir_x;
	double	camera_dir_y;
	double	move_speed;
	double	rotation_speed;
}	t_player;

typedef struct s_map
{
	char	*file;
	char	**file_content;
	char	**map;
	char	**map_copy;
	int		heigth;
}	t_map;

typedef struct s_image
{
	mlx_image_t		*north_wall;
	mlx_image_t		*south_wall;
	mlx_image_t		*east_wall;
	mlx_image_t		*west_wall;
}	t_image;

typedef struct s_texture
{
	char			*north_path;
	mlx_texture_t	*north_wall;
	char			*south_path;
	mlx_texture_t	*south_wall;
	char			*east_path;
	mlx_texture_t	*east_wall;
	char			*west_path;
	mlx_texture_t	*west_wall;
	char			*floor_color;
	uint32_t		floor_color_hex;
	char			*ceiling_color;
	uint32_t		ceiling_color_hex;
}	t_texture;

typedef struct s_ray
{
	double	perp_wall_dist;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	ray_dir_x;
	double	ray_dir_y;
	int		step_x;
	int		step_y;
	int		map_x;
	int		map_y;
	int		hit;
	int		side;
}	t_ray;

typedef struct s_wall
{
	int				height;
	int				draw_start;
	int				draw_end;
	int				text_x;
	mlx_texture_t	*s_texture;
}	t_wall;

typedef struct s_raycasting
{
	t_ray		ray;
	t_wall		wall;
	mlx_image_t	*image;
}	t_raycasting;

typedef struct s_game
{
	mlx_t			*mlx;
	t_player		*player;
	t_map			*map;
	t_image			*image;
	t_texture		*texture;
	t_raycasting	*raycasting;
}	t_game;

// Parser
void	parser(t_game *game, char *file);

// Function prototypes //

//error_manager
void	error_messages(short error_code);

//inits_manager
int32_t	init_cub3d(t_game *game);

//render
void	load_screen(t_game *game);

//movement
void	keypress(mlx_key_data_t keydata, void *param);
void	handle_movement(void *param);

//testes
void perform_raycasting(t_game *game);
void raycasting_loop(void *param);

//
uint32_t	convert_rgb_string(char *rgb_str);

#endif
