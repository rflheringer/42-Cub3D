/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafaelheringer <rafaelheringer@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 17:52:53 by rheringe          #+#    #+#             */
/*   Updated: 2025/07/23 18:46:45 by rafaelherin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../MLX42/include/MLX42/MLX42.h"
# include "../lib/includes/libft.h"
# include <fcntl.h>

# define EXIT_TOO_MANY_ARGUMENTS 3
# define EXIT_FEW_ARGUMENTS 4
# define EXIT_ERROR_MEMORY_ALLOCATION 5
# define EXIT_INVALID_EXTENSION 6
# define EXIT_INVALID_FILE 7

typedef struct s_player
{
	char	*pos;
	int		pos_x;
	int		pos_y;
}t_player;

typedef struct s_map
{
	char	*file;
	char	**map;
	char	**map_copy;
	int		width;
	int		heigth;
}t_map;

typedef struct s_image
{
	mlx_image_t		*north_wall;
	mlx_image_t		*south_wall;
	mlx_image_t		*east_wall;
	mlx_image_t		*west_wall;
}t_image;

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
}t_texture;

typedef struct s_game
{
	void		*mlx;
	t_player	*player;
	t_map		*map;
	t_image		*image;
	t_texture	*texture;
}t_game;


// Function prototypes //

//error_manager
void	error_messages(short error_code);

//inits_manager
int32_t	init_cub3d(t_game *game);

#endif
