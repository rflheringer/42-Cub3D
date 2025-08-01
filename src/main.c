/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:43:05 by rheringe          #+#    #+#             */
/*   Updated: 2025/08/01 15:21:03 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	main(int argc, char **argv)
{
	t_game	*game;

	if (argc > 2)
	{
		ft_printf_fd(2, "Error\nToo many arguments\n");
		return (1);
	}
	if (argc < 2)
	{
		ft_printf_fd(2, "Error\nToo few arguments\n");
		return (1);
	}
	game = ft_calloc(1, sizeof(t_game));
	game->map = ft_calloc(1, sizeof(t_map));
	game->player = ft_calloc(1, sizeof(t_player));
	parser(game, argv[1]);
	init_cub3d(game);
}
