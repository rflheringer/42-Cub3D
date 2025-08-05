NAME		=	cub3D
NAME_BONUS	=	cub3D_bonus

CC			=	cc
FLAGS		=	-Wall -Wextra -Werror -g3
LIBFT		=	./lib
LIBMLX		=	./MLX42

LIBS		=	$(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm $(LIBFT)/libft.a

HEADERS			=	-I $(LIBFT)/Includes -I $(LIBMLX)/build
HEADERS_BONUS	=	-I $(LIBFT)/Includes -I $(LIBMLX)/build

SRCS		=	./src/main.c \
				./src/parser/parser.c \
				./src/parser/parser_player.c \
				./src/parser/parser_textures.c \
				./src/parser/parser_map.c \
				./src/parser/parser_utils.c \
				./src/parser/parser_colors.c \
				./src/player/movement.c \
				./src/player/controls.c \
        		./src/error.c \
				./src/raycasting/raycasting.c \
				./src/raycasting/raycasting_utils.c \
				./src/raycasting/dda.c \
				./src/raycasting/init_ray.c \
				./src/initializers.c \
				./src/clean.c \
				./src/player/move_utils.c

SRCS_BONUS	=	./src/bonus/main_bonus.c \
				./src/bonus/error_bonus.c \
				./src/bonus/clean_bonus.c \
				./src/bonus/initializers_bonus/initializers_bonus.c \
				./src/bonus/initializers_bonus/lightning_bonus.c \
				./src/bonus/parser_bonus/parser_bonus.c \
				./src/bonus/parser_bonus/parser_colors_bonus.c \
				./src/bonus/parser_bonus/parser_map_bonus.c \
				./src/bonus/parser_bonus/parser_player_bonus.c \
				./src/bonus/parser_bonus/parser_textures_bonus.c \
				./src/bonus/parser_bonus/parser_utils_bonus.c \
				./src/bonus/player_bonus/controls_bonus.c \
				./src/bonus/player_bonus/move_utils_bonus.c \
				./src/bonus/player_bonus/movement_bonus.c \
				./src/bonus/raycasting_bonus/dda_bonus.c \
				./src/bonus/raycasting_bonus/init_ray_bonus.c \
				./src/bonus/raycasting_bonus/raycasting_bonus.c \
				./src/bonus/raycasting_bonus/raycasting_utils_bonus.c \
				./src/bonus/enemy_bonus/init_enemy_bonus.c \
				./src/bonus/minimap_bonus/minimap_bonus.c
				

OBJ_DIR		=	objs
OBJS		=	$(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))
OBJS_BONUS	=	$(addprefix $(OBJ_DIR)/, $(notdir $(SRCS_BONUS:.c=.o)))

VPATH			=	./src ./src/parser ./src/player ./src/raycasting
VPATH_BONUS		=	./src/bonus ./src/bonus/parser_bonus ./src/bonus/player_bonus ./src/bonus/raycasting_bonus ./src/bonus/initializers_bonus ./src/bonus/minimap_bonus ./src/bonus/enemy_bonus/

all: libmlx libft ${NAME}

libmlx:
	@cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4 --no-print-directory

libft:
	@make -C $(LIBFT) --no-print-directory

${NAME}: ${OBJS}
	$(CC) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@${CC} ${FLAGS} ${HEADERS} -o $@ -c $<

bonus: libmlx libft
	@$(MAKE) ${NAME_BONUS} VPATH="$(VPATH) $(VPATH_BONUS)"

${NAME_BONUS}: ${OBJS_BONUS}
	$(CC) $(OBJS_BONUS) $(LIBS) $(HEADERS_BONUS) -o $(NAME_BONUS)

$(OBJ_DIR)/%_bonus.o: %_bonus.c
	@mkdir -p $(OBJ_DIR)
	@${CC} ${FLAGS} ${HEADERS_BONUS} -o $@ -c $<

clean:
	@rm -rf ${OBJS}
	@rm -rf ${OBJS_BONUS}
	@rm -rf $(LIBMLX)/build
	@$(MAKE) -C $(LIBFT) clean

fclean:	clean
	@rm -rf ${NAME} ${NAME_BONUS}
	@$(MAKE) -C $(LIBFT) fclean

re: fclean all

.PHONY: all clean fclean re libft bonus