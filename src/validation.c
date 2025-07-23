/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafaelheringer <rafaelheringer@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 18:32:54 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/07/23 18:55:46 by rafaelherin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void validate_map(char *file, t_game *game)
{
    int fd;
    
    if (!(ft_strnstr(file, ".cub", ft_strlen(file))))
        message_error(EXIT_INVALID_EXTENSION);
    fd = open(file, O_RDONLY);
    if (fd < 0)
		message_error(EXIT_INVALID_FILE);
    
}