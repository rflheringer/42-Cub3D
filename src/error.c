/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:46:49 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/07/28 12:03:20 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	error_messages(short error_code)
{
	if (error_code == EXIT_TOO_MANY_ARGUMENTS)
		ft_printf_fd(STDERR_FILENO, "Error\nToo many arguments\n");
	if (error_code == EXIT_FEW_ARGUMENTS)
		ft_printf_fd(STDERR_FILENO, "Error\nToo few arguments\n");
	if (error_code == EXIT_INVALID_EXTENSION)
		ft_printf_fd(STDERR_FILENO, "Error\nInvalid file extension\n");
	if (error_code == EXIT_INVALID_FILE)
		ft_printf_fd(STDERR_FILENO, "Error\nInvalid file\n");
	if (error_code == EXIT_FAILED_TO_LOAD_TEXTURE)
		ft_printf_fd(STDERR_FILENO, "Error\nFailed to load texture\n");
	if (error_code == EXIT_FAILED_TO_LOAD_IMAGE)
		ft_printf_fd(STDERR_FILENO, "Error\nFailed to load images\n");
}
