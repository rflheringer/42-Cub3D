/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:43:05 by rheringe          #+#    #+#             */
/*   Updated: 2025/07/25 18:44:16 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"
#include <stdio.h>

int	main(int argc, char **argv)
{
	t_game	*game;

	if (argc > 2)
	{
		return (1);
	}
	if (argc < 2)
	{
		return (1);
	}
	game = ft_calloc(1, sizeof(t_game));
	game->map = ft_calloc(1, sizeof(t_map));
	game->texture = ft_calloc(1, sizeof(t_texture));
	game->image = ft_calloc(1, sizeof(t_image));
	game->player = ft_calloc(1, sizeof(t_player));
	parser(game, argv[1]);
	for (int i = 0; i < ft_ptrlen(game->map->map); i++)
		ft_printf("%s\n", game->map->map[i]);
}

// ft_printf("%c - %d %d", game->player->direction, game->player->pos_y, game->player->pos_x);
// int i = 0;
// while (game->map->file_content[i])
// {
// 	printf("%s\n", game->map->file_content[i]);
// 	printf("%d\n", i);
// 	i++;
// }
