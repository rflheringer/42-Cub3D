/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:43:05 by rheringe          #+#    #+#             */
/*   Updated: 2025/07/31 13:48:07 by rdel-fra         ###   ########.fr       */
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
// int i = 0;
// while (game->map->file_content[i])
// {
// 	printf("%s\n", game->map->file_content[i]);
// 	printf("%d\n", i);
// 	i++;
// }
/*parser*/
/*init progam = load texture, images, screen*/
