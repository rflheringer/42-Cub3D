NAME		=	cub3D
NAME_BONUS	=	cub3D_bonus

CC			=	cc
FLAGS		=	-Wall -Wextra -Werror -g3
LIBFT		=	./lib
LIBMLX		=	./MLX42

LIBS		=	$(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm $(LIBFT)/libft.a

HEADERS			=	-I $(LIBFT)/Includes -I $(LIBMLX)/build
HEADERS_BONUS	=	-I $(LIBFT)/Includes -I $(LIBMLX)/build

SRCS		=	./src/mandatory/main.c \
				./src/mandatory/parser/parser.c \
				./src/mandatory/parser/parser_player.c \
				./src/mandatory/parser/parser_textures.c \
				./src/mandatory/parser/parser_map.c \
				./src/mandatory/parser/parser_utils.c \
				./src/mandatory/parser/parser_colors.c \
				./src/mandatory/player/movement.c \
				./src/mandatory/player/controls.c \
        		./src/mandatory/error.c \
				./src/mandatory/raycasting/raycasting.c \
				./src/mandatory/raycasting/raycasting_utils.c \
				./src/mandatory/raycasting/dda.c \
				./src/mandatory/raycasting/init_ray.c \
				./src/mandatory/initializers.c \
				./src/mandatory/clean.c \
				./src/mandatory/player/move_utils.c

SRCS_BONUS	=	./src/bonus/main_bonus.c \
				./src/bonus/error_bonus.c \
				./src/bonus/clean_bonus.c \
				./src/bonus/free_bonus.c \
				./src/bonus/initializers_bonus/initializers_bonus.c \
				./src/bonus/initializers_bonus/lightning_bonus.c \
				./src/bonus/parser_bonus/parser_bonus.c \
				./src/bonus/parser_bonus/parser_colors_bonus.c \
				./src/bonus/parser_bonus/parser_map_bonus.c \
				./src/bonus/parser_bonus/parser_textures_bonus.c \
				./src/bonus/parser_bonus/parser_utils_bonus.c \
				./src/bonus/player_bonus/init_player_bonus.c \
				./src/bonus/player_bonus/controls_bonus.c \
				./src/bonus/player_bonus/move_utils_bonus.c \
				./src/bonus/player_bonus/movement_bonus.c \
				./src/bonus/player_bonus/life_bonus_utils.c \
				./src/bonus/player_bonus/life_bonus.c \
				./src/bonus/raycasting_bonus/dda_bonus.c \
				./src/bonus/raycasting_bonus/init_ray_bonus.c \
				./src/bonus/raycasting_bonus/raycasting_bonus.c \
				./src/bonus/raycasting_bonus/raycasting_utils_bonus.c \
				./src/bonus/enemy_bonus/enemy_images_bonus.c \
				./src/bonus/enemy_bonus/enemy_init_bonus.c \
				./src/bonus/enemy_bonus/enemy_manage_bonus.c \
				./src/bonus/enemy_bonus/enemy_utils_bonus.c \
				./src/bonus/enemy_bonus/enemy_move_bonus.c \
				./src/bonus/enemy_bonus/enemy_position_bonus.c \
				./src/bonus/enemy_bonus/enemy_sort_bonus.c \
				./src/bonus/door_bonus/door_bonus.c \
				./src/bonus/minimap_bonus/minimap_bonus.c \
				./src/bonus/minimap_bonus/minimap_utils_bonus.c \
				./src/bonus/minimap_bonus/draw_fov_bonus.c \
				./src/bonus/minimap_bonus/fov_utils_bonus.c \
				./src/bonus/attack_bonus/create_bullet_bonus.c \
				./src/bonus/attack_bonus/create_fireball_bonus.c \
				./src/bonus/attack_bonus/render_bullet_bonus.c \
				./src/bonus/attack_bonus/render_fireball_bonus.c \
				./src/bonus/attack_bonus/update_bullet_bonus.c \
				./src/bonus/attack_bonus/update_fireball_bonus.c \
				./src/bonus/boss_bonus/init_boss_bonus.c \
				./src/bonus/boss_bonus/move_boss_bonus.c \
				./src/bonus/boss_bonus/render_boss_bonus.c \
				./src/bonus/game_bonus/handle_clean_bonus.c \
				./src/bonus/game_bonus/handle_game_bonus.c \
				./src/bonus/game_bonus/handle_utils_bonus.c \
				./src/bonus/game_bonus/update_game_bonus.c


CYAN		=	\033[1;96m
GREEN		=	\033[1;92m
RED			=	\033[1;91m
NC			=	\033[0m

OBJ_DIR		=	objs
OBJS		=	$(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))
OBJS_BONUS	=	$(addprefix $(OBJ_DIR)/, $(notdir $(SRCS_BONUS:.c=.o)))

VPATH			=	./src/mandatory ./src/mandatory/parser ./src/mandatory/player ./src/mandatory/raycasting
VPATH_BONUS		=	./src/bonus ./src/bonus/parser_bonus ./src/bonus/player_bonus ./src/bonus/raycasting_bonus ./src/bonus/initializers_bonus ./src/bonus/minimap_bonus ./src/bonus/enemy_bonus/ ./src/bonus/attack_bonus ./src/bonus/game_bonus/ ./src/bonus/boss_bonus/ ./src/bonus/door_bonus/

all: libmlx libft
	@if $(MAKE) -q ${NAME} 2>/dev/null; then \
		echo "${GREEN}✅ cub3D already compiled!${NC}"; \
	else \
		$(MAKE) ${NAME} --no-print-directory; \
	fi

libmlx:
	@if cmake $(LIBMLX) -B $(LIBMLX)/build > /dev/null 2>&1 && make -C $(LIBMLX)/build -j4 --no-print-directory > /dev/null 2>&1; then \
		echo "${GREEN}✅ MLX42 compiled successfully!${NC}"; \
	else \
		cmake $(LIBMLX) -B $(LIBMLX)/build > /dev/null 2>&1 && make -C $(LIBMLX)/build -j4 --no-print-directory > /dev/null 2>&1; \
	fi

libft:
	@if $(MAKE) -q -C $(LIBFT); then \
		echo "${GREEN}✅ libft already compiled!${NC}"; \
	else \
		make -C $(LIBFT) --no-print-directory; \
	fi

${NAME}: ${OBJS}
	@$(CC) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME)
	@echo "${CYAN}cub3D compiled successfully!${NC}"

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@${CC} ${FLAGS} ${HEADERS} -o $@ -c $<

bonus: libmlx libft
	@if $(MAKE) -q ${NAME_BONUS} VPATH="$(VPATH) $(VPATH_BONUS)" 2>/dev/null; then \
		echo "${GREEN}✅ cub3D bonus already compiled!${NC}"; \
	else \
		$(MAKE) ${NAME_BONUS} VPATH="$(VPATH) $(VPATH_BONUS)" --no-print-directory; \
	fi

${NAME_BONUS}: ${OBJS_BONUS}
	@$(CC) $(OBJS_BONUS) $(LIBS) $(HEADERS_BONUS) -o $(NAME_BONUS)
	@echo "${CYAN}cub3D bonus compiled successfully!${NC}"

$(OBJ_DIR)/%_bonus.o: %_bonus.c
	@mkdir -p $(OBJ_DIR)
	@${CC} ${FLAGS} ${HEADERS_BONUS} -o $@ -c $<

clean:
	@rm -rf ${OBJS}
	@rm -rf ${OBJS_BONUS}
	@rm -rf $(LIBMLX)/build
	@$(MAKE) -C $(LIBFT) clean --no-print-directory
	@echo "${RED}Object files cleaned!${NC}"

fclean:	clean
	@rm -rf ${NAME} ${NAME_BONUS}
	@$(MAKE) -C $(LIBFT) fclean --no-print-directory
	@echo "${RED}All files cleaned!${NC}"

re: fclean all

.PHONY: all clean fclean re libft bonus