/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 17:52:53 by rheringe          #+#    #+#             */
/*   Updated: 2025/08/14 20:52:10 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_BONUS_H
# define CUB3D_BONUS_H

# include "../MLX42/include/MLX42/MLX42.h"
# include "../lib/includes/libft.h"
# include <limits.h>
# include <fcntl.h>
# include <stdio.h>
# include <math.h>

# define WIDTH 1620
# define HEIGHT 880
# define W_NAME "Catacombs 42"
# define FOV 1.0472
# define R 0.05
# define LIFE_MAX 10
# define EXIT_TOO_MANY_ARGUMENTS 3
# define EXIT_FEW_ARGUMENTS 4
# define EXIT_ERROR_MEMORY_ALLOCATION 5
# define EXIT_INVALID_EXTENSION 6
# define EXIT_INVALID_FILE 7
# define EXIT_CHAR_CONTROL 10
# define EXIT_MAP_NOT_CLOSED 11
# define EXIT_UNEXPECTED_CHAR 12
# define EXIT_TOO_MANY_START_POS 13
# define EXIT_NO_START_POSITION 14
# define EXIT_DUPLICATE_TEXTURE 15
# define EXIT_DUPLICATE_COLOR 16
# define EXIT_INVALID_TEXTURE_PATH 17
# define EXIT_INVALID_RGB_COLOR 18
# define EXIT_INVALID_MAP 19
# define EXIT_TOO_MANY_BOSSES 20
# define EXIT_INVALID_START_POSITION 21

typedef enum e_state
{
	MOVING = 1,
	ALERT,
	ATTACK,
	DYING,
	DEAD,
	HITED,
	IDLE,
	DAMAGE,
}	t_state;

typedef struct s_attack
{
	double			pos_x;
	double			pos_y;
	double			dir_x;
	double			dir_y;
	double			move_delay;
	double			frame_delay;
	t_state			state;
	int				next_tile;
	int				current_frame;
	struct s_attack	*next;
}	t_attack;

typedef struct s_player
{
	int				p;
	int				hp;
	int				keys;
	char			start_dir;
	bool			up;
	bool			down;
	bool			right;
	bool			left;
	bool			rot_left;
	bool			rot_right;
	bool			moved;
	double			pos_x;
	double			pos_y;
	double			old_x;
	double			old_y;
	double			player_dir_x;
	double			player_dir_y;
	double			camera_dir_x;
	double			camera_dir_y;
	double			move_speed;
	double			rotation_speed;
	double			hit_x;
	double			hit_y;
	double			attack_delay;
	mlx_texture_t	*fireball_textures[5];
}	t_player;

typedef struct s_map
{
	char	*file;
	char	**file_content;
	char	**map;
	char	*file_name;
	int		height;
}	t_map;

typedef struct s_fov
{
	int	cx;
	int	cy;
	int	xl;
	int	yl;
	int	xr;
	int	yr;
	int	dx;
	int	dy;
	int	sx;
	int	sy;
	int	err;
	int	e2;
	int	minx;
	int	maxx;
	int	miny;
	int	maxy;
}	t_fov;

typedef struct s_image
{
	mlx_image_t		*game_over;
	mlx_image_t		*game_win;
	mlx_image_t		*hand;
	mlx_image_t		*keys_sprite;
	mlx_image_t		*keys_text;
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
	mlx_texture_t	*closed_door;
	mlx_texture_t	*open_door;
	mlx_texture_t	*potion;
	mlx_texture_t	*key;
	mlx_texture_t	*game_over;
	mlx_texture_t	*game_win;
	mlx_texture_t	*hand;
	int				north;
	int				south;
	int				east;
	int				west;
	int				floor;
	int				ceiling;
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
	mlx_image_t	*image;
	double		*buffer;
	int			scale;
}	t_raycasting;

typedef struct s_door
{
	double			pos_x;
	double			pos_y;
	bool			need_key;
	struct s_door	*next;
}	t_door;

typedef struct s_boss
{
	int				b;
	int				hp;
	double			pos_x;
	double			pos_y;
	double			distance;
	t_state			state;
	int				cur_sprite;
	int				dying_sprite;
	int				attack_sprite;
	bool			hit_player;
	double			move_speed;
	double			move_delay;
	double			attack_delay;
	double			death_delay;
	double			frame_delay;
	mlx_texture_t	*boss_text[10];
	mlx_texture_t	*bullet_textures[4];
}	t_boss;

typedef struct s_enemy_list
{
	double				pos_x;
	double				pos_y;
	double				distance;
	t_state				state;
	int					cur_sprite;
	int					dying_sprite;
	int					attack_sprite;
	double				move_delay;
	double				death_delay;
	double				frame_delay;
	struct s_enemy_list	*next;
	struct s_enemy_list	*prev;
}	t_enemy_list;

typedef struct s_enemy
{
	mlx_texture_t	*enemy_texture[10];
	double			move_speed;
	t_enemy_list	*list;
	int				texx;
	int				texy;
}	t_enemy;

typedef struct s_lightning {
	bool		is_active;
	uint32_t	original_color;
	uint32_t	flash_color;
	double		last_time;
	double		next_flash;
	double		duration;
}	t_lightning;

typedef struct s_game
{
	mlx_t			*mlx;
	double			delta_time;
	int				game_over;
	int				game_win;
	t_player		*player;
	t_map			*map;
	t_door			*doors;
	t_image			*image;
	t_texture		*texture;
	t_raycasting	*raycasting;
	t_wall			*wall;
	t_ray			*ray;
	t_boss			*boss;
	t_enemy			*enemy;
	t_attack		*bullets;
	t_attack		*fireballs;
	t_lightning		*lightning;
}	t_game;

// Function prototypes //

// Parser
void	parser(t_game *game, char *file);

// Parser textures
void	get_text_color_and_map(t_game *game, char **content);
void	validate_textures(t_game *game, char *path);

// Parser map
void	validate_map(t_game *game, char **map);
void	get_map(t_game *game, char **content, int *i);

// Parser utils
bool	only_spaces(char *str);
bool	has_control_char(char *str);
bool	is_valid_line(char *content);
bool	startswith(char *str, char *start);

// Parser player
void	check_player(t_game *game);
void	get_player_position(t_game *game, char **map, int i, int j);

// Parser colors
void	validate_colors(t_game *game);

// error_manager
void	error_messages(short error_code);
void	shutdown_program(t_game *game, short error_code);

// init_manager
int32_t	init_cub3d(t_game *game);

// movement
bool	rot_left(t_game *game);
bool	rot_right(t_game *game);
void	get_move(t_game *game);
void	call_clean_and_draw_functions(t_game *game);
void	draw_hand(t_game *game);
void	draw_keys(t_game *game);
void	check_key_and_potion(t_game *game, int x, int y);

// movement utils
bool	left_move(t_game *game);
bool	right_move(t_game *game);
void	handle_movement(void *param);
bool	can_move_to(t_game *game, double x, double y, t_enemy_list *enemy_list);

// raycasting
void	perform_raycasting(t_game *game);
void	init_ray(t_game *game, int x);
void	calculate_wall(t_game *game);

// controls
void	keypress(mlx_key_data_t keydata, void *param);
void	rotate_player_mouse(double xpos, double ypos, void *param);

// dda
void	perform_dda(t_game *game, int x);

// lightning_bonus
void	init_lightning(t_game *game);
void	update_lightning(t_game *game);

// enemy_bonus
void	manage_enemies(t_game *game);
void	init_bonus_images(t_game *game);
void	set_enemy_and_door(t_game *game, int i, int j);
void	calculate_enemie_position(t_game *game, double pos_x, double pos_y, mlx_texture_t *texture, int less_height);
void	sort_enemies(t_game *game, t_enemy_list *nav);
void	enemy_move(t_game *game, t_enemy_list *enemy, double dx, double dy);
void	sort_enemies(t_game *game, t_enemy_list *nav);

// minimap_bonus
void	draw_enemies(t_game *game);
void	update_minimap(t_game *game);
void	calculate_player_fov(t_game *game, int scale);
int		cyy(t_game *game, int scale);
int		cxx(t_game *game, int scale);
void	calculate_variables(t_game *game, int len, int scale);

// utils_bonus
double	get_delta_time(void);

// fireball
void	create_fireball(t_game *game);
void	update_fireballs(t_game *game);
void	render_fireballs(t_game *game, t_attack *fireball);
void	calculate_fireball_position(t_game *game, t_attack *fireball);

// update_game
void	update_game_state(t_game *game);

// life
void	draw_life(t_game *game);

// door
void	create_door(t_game *game, int i, int j);
void	can_open_or_close_door(t_game *game);

// boss_bonus
void	create_boss(t_game *game, int i, int j);
void	manage_boss(t_game *game);
void	fire_projectile(t_game *game);
void	update_bullets(t_game *game);
void	boss_move(t_game *game, double dx, double dy);
void	calculate_boss_position(t_game *game, double pos_x, double pos_y,
			mlx_texture_t *texture, int less_height);
void	calculate_bullet_position(t_game *game, t_attack *bullet);

#endif
