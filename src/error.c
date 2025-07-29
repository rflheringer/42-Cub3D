/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:46:49 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/07/29 13:10:49 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	error_messages(short error_code)
{
	if (error_code == EXIT_TOO_MANY_ARGUMENTS)
		ft_printf_fd(2, "Error\nToo many arguments\n");
	if (error_code == EXIT_FEW_ARGUMENTS)
		ft_printf_fd(2, "Error\nToo few arguments\n");
	if (error_code == EXIT_INVALID_EXTENSION)
		ft_printf_fd(2, "Error\nInvalid file extension\n");
	if (error_code == EXIT_INVALID_FILE)
		ft_printf_fd(2, "Error\nInvalid file\n");
	if (error_code == EXIT_FAILED_TO_LOAD_TEXTURE)
		ft_printf_fd(2, "Error\nFailed to load texture\n");
	if (error_code == EXIT_FAILED_TO_LOAD_IMAGE)
		ft_printf_fd(2, "Error\nFailed to load images\n");
	if (error_code == EXIT_CHAR_CONTROL)
		ft_printf_fd(2, "Error\nControl char found when expected space\n");
	if (error_code == EXIT_MAP_NOT_CLOSED)
		ft_printf_fd(2, "Error\nMap not closed\n");
	if (error_code == EXIT_UNEXPECTED_CHAR)
		ft_printf_fd(2, "Error\nFound a unexpected character\n");
	if (error_code == EXIT_TOO_MANY_START_POS)
		ft_printf_fd(2, "Error\nMore than one starting position was found\n");
	if (error_code == EXIT_NO_START_POSITION)
		ft_printf_fd(2, "Error\nNo start position was found");
}

static void	delete_images(t_game *game)
{
	mlx_delete_image(game->mlx, game->image->north_wall);
	mlx_delete_image(game->mlx, game->image->south_wall);
	mlx_delete_image(game->mlx, game->image->east_wall);
	mlx_delete_image(game->mlx, game->image->west_wall);
}

void	shutdown_program(t_game *game, short error_code)
{
	if (error_code != 0)
		error_messages(error_code);
	delete_images(game);
	// delete_texture(game);
	mlx_terminate(game->mlx);
	ft_free_matrix(game->map->map);
	free(game->raycasting);
	free(game->image);
	free(game->player);
	free(game->map);
	free(game);
}
