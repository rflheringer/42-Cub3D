/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:46:49 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/08/15 14:18:03 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static void	error_messages_plus(short error_code)
{
	if (error_code == EXIT_NO_START_POSITION)
		ft_printf_fd(2, "Error\nNo start position was found\n");
	if (error_code == EXIT_DUPLICATE_TEXTURE)
		ft_printf_fd(2, "Error\nDuplicate texture found\n");
	if (error_code == EXIT_DUPLICATE_COLOR)
		ft_printf_fd(2, "Error\nDuplicate color found\n");
	if (error_code == EXIT_INVALID_TEXTURE_PATH)
		ft_printf_fd(2, "Error\nInvalid texture path\n");
	if (error_code == EXIT_INVALID_RGB_COLOR)
		ft_printf_fd(2, "Error\nInvalid RGB color\n");
	if (error_code == EXIT_INVALID_MAP)
		ft_printf_fd(2, "Error\nInvalid map\n");
	if (error_code == EXIT_INVALID_START_POSITION)
		ft_printf_fd(2, "Error\nInvalid start position\n");
}

void	error_messages(short error_code)
{
	if (error_code == EXIT_INVALID_EXTENSION)
		ft_printf_fd(2, "Error\nInvalid file extension\n");
	if (error_code == EXIT_INVALID_FILE)
		ft_printf_fd(2, "Error\nInvalid file\n");
	if (error_code == EXIT_CHAR_CONTROL)
		ft_printf_fd(2, "Error\nControl char found when expected space\n");
	if (error_code == EXIT_MAP_NOT_CLOSED)
		ft_printf_fd(2, "Error\nMap not closed\n");
	if (error_code == EXIT_UNEXPECTED_CHAR)
		ft_printf_fd(2, "Error\nFound a unexpected character\n");
	if (error_code == EXIT_TOO_MANY_START_POS)
		ft_printf_fd(2, "Error\nMore than one starting position was found\n");
	error_messages_plus(error_code);
}
