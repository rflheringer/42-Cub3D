/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:43:05 by rheringe          #+#    #+#             */
/*   Updated: 2025/07/29 17:32:58 by rheringe         ###   ########.fr       */
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
	game->map = ft_calloc(1, sizeof(t_map));
	game->texture = ft_calloc(1, sizeof(t_texture));
	game->image = ft_calloc(1, sizeof(t_image));
	game->player = ft_calloc(1, sizeof(t_player));
	game->raycasting = ft_calloc(1, sizeof(t_raycasting));
	if (!game)
		error_messages(EXIT_ERROR_MEMORY_ALLOCATION);
	parser(game, argv[1]);
	init_cub3d(game);
}
// int i = 0;
// while (game->map->file_content[i])
// {
// 	printf("%s\n", game->map->file_content[i]);
// 	printf("%d\n", i);
// 	i++;
// }
/*parser*/
/*init progam = load texture, images, screen*/
