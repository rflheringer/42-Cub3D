NAME		=	cub3D

CC			=	cc
FLAGS		=	-Wextra
LIBFT		=	./lib
LIBMLX		=	./MLX42

LIBS		=	$(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm $(LIBFT)/libft.a

HEADERS		=	-I $(libs)/Includes -I $(LIBMLX)/build

SRCS		=	./src/error.c \
				./src/initializers.c \
				./src/main.c

OBJS			=	${SRCS:.c=.o}

all: libmlx libft ${NAME}

libmlx:
	@cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

libft:
	@make -C $(LIBFT)

${NAME}: ${OBJS}
	@$(CC) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME)

%.o:%.c
	${CC} ${FLAGS} -o $@ -c $< ${HEADERS}

clean:
	@rm -rf ${OBJS}
	@rm -rf $(LIBMLX)/build
	@$(MAKE) -C $(LIBFT) clean

fclean:	clean
	@rm -rf ${NAME}
	@$(MAKE) -C $(LIBFT) fclean

re: fclean all

.PHONY: all clean fclean re libft