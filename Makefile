NAME		=	cub3D

CC			=	cc
FLAGS		=	-Wextra
LIBFT		=	./lib
LIBMLX		=	./MLX42

LIBS		=	$(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm $(LIBFT)/libft.a

HEADERS		=	-I $(LIBFT)/Includes -I $(LIBMLX)/build

SRCS		=	./src/main.c \
				./src/parser/parser.c
        ./src/error.c \
				./src/initializers.c \

OBJ_DIR		=	objs
OBJS		=	$(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))
VPATH		=	./src ./src/parser

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

clean:
	@rm -rf ${OBJS}
	@rm -rf $(LIBMLX)/build
	@$(MAKE) -C $(LIBFT) clean

fclean:	clean
	@rm -rf ${NAME}
	@$(MAKE) -C $(LIBFT) fclean

re: fclean all

.PHONY: all clean fclean re libft