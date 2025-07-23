/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafaelheringer <rafaelheringer@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:46:49 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/07/23 16:41:50 by rafaelherin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void    error_messages(short error_code)
{
    if (error_code == EXIT_TOO_MANY_ARGUMENTS)
        ft_printf_fd(STDERR_FILENO, "Error\nToo many arguments\n");
    if (error_code == EXIT_FEW_ARGUMENTS)
        ft_printf_fd(STDERR_FILENO, "Error\nToo few arguments\n");
}