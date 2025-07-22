/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:43:05 by rheringe          #+#    #+#             */
/*   Updated: 2025/07/22 18:55:14 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	main(int argc, char **argv)
{
	t_game	*game;

	if (argc > 2)
	{
		ft_putstr_fd("Error\nToo many arguments\n", 2);
		return (1);
	}
	if (argc < 2)
	{
		ft_putstr_fd("Error\nToo few arguments\n", 2);
		return (1);
	}
	game = ft_calloc(1, sizeof(t_game));
	/*parser*/
	/*init progam = load texture, images, screen*/
}
