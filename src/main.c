/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:43:05 by rheringe          #+#    #+#             */
/*   Updated: 2025/07/25 16:25:23 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"
#include <stdio.h>

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
	game->map = ft_calloc(1, sizeof(t_map));
	game->texture = ft_calloc(1, sizeof(t_texture));
	game->image = ft_calloc(1, sizeof(t_image));
	game->player = ft_calloc(1, sizeof(t_player));
	if (!game)
		error_messages(EXIT_ERROR_MEMORY_ALLOCATION);
  parser(game, argv[1]);
	init_cub3d(game);
}
// for (int i = 0; i < ft_ptrlen(game->map->map); i++)
// 	ft_printf("%s\n", game->map->map[i]);
// int i = 0;
// while (game->map->file_content[i])
// {
// 	printf("%s\n", game->map->file_content[i]);
// 	printf("%d\n", i);
// 	i++;
// }
/*parser*/
/*init progam = load texture, images, screen*/
