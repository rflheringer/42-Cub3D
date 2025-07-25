/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafaelheringer <rafaelheringer@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:46:49 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/07/23 18:49:46 by rafaelherin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void    error_messages(short error_code)
{
    if (error_code == EXIT_TOO_MANY_ARGUMENTS)
        ft_printf_fd(STDERR_FILENO, "Error\nToo many arguments\n");
    if (error_code == EXIT_FEW_ARGUMENTS)
        ft_printf_fd(STDERR_FILENO, "Error\nToo few arguments\n");
    if (error_code == EXIT_INVALID_EXTENSION)
        ft_printf_fd(STDERR_FILENO, "Error\nInvalid file extension\n");
    if (error_code == EXIT_INVALID_FILE)
        ft_printf_fd(STDERR_FILENO, "Error\nInvalid file\n"); 
}