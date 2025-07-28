/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 17:52:53 by rheringe          #+#    #+#             */
/*   Updated: 2025/07/28 13:02:42 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../MLX42/include/MLX42/MLX42.h"
# include "../lib/includes/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <math.h>

# define WIDTH 640
# define HEIGHT 480
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
	int		pos_x;
	int		pos_y;
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
	char			*ceiling_color;
}	t_texture;

typedef struct s_game
{
	void		*mlx;
	t_player	*player;
	t_map		*map;
	t_image		*image;
	t_texture	*texture;
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

#endif
