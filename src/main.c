/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafaelheringer <rafaelheringer@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:43:05 by rheringe          #+#    #+#             */
/*   Updated: 2025/08/02 01:10:03 by rafaelherin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	main(int argc, char **argv)
{
	t_game	*game;

	if (argc > 2)
	{
		error_messages(EXIT_TOO_MANY_ARGUMENTS);
		return (EXIT_TOO_MANY_ARGUMENTS);
	}
	if (argc < 2)
	{
		error_messages(EXIT_FEW_ARGUMENTS);
		return (EXIT_FEW_ARGUMENTS);
	}
	game = ft_calloc(1, sizeof(t_game));
	if (!game)
		error_messages(EXIT_ERROR_MEMORY_ALLOCATION);
	game->map = ft_calloc(1, sizeof(t_map));
	game->player = ft_calloc(1, sizeof(t_player));
	parser(game, argv[1]);
	init_cub3d(game);
}
