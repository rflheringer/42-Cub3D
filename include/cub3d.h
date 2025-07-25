/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 17:52:53 by rheringe          #+#    #+#             */
/*   Updated: 2025/07/25 14:49:24 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../MLX42/include/MLX42/MLX42.h"
# include "../lib/includes/libft.h"
# include <fcntl.h>

typedef struct s_player
{
	char	*pos;
	int		pos_x;
	int		pos_y;
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

#endif /* cub3D */
