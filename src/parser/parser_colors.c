/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_colors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:33:37 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/31 11:35:33 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

static bool	verify_rgb_format(char *color)
{
	int	i;

	i = 0;
	while (color[i])
	{
		if (color[i] < '0' || color[i] > '9')
			if (color[i] != ',')
				return (false);
		i++;
	}
	return (true);
}

static bool	verify_rgb_numbers(char **rgb)
{
	long	nb;
	int		i;

	i = 0;
	while (rgb[i])
	{
		nb = ft_atol(rgb[i]);
		if (nb < 0 || nb > 255)
			return (false);
		i++;
	}
	return (true);
}

static bool	is_valid_rgb(char *color)
{
	char	**rgb;

	if (!color || !*color)
		return (false);
	if (!verify_rgb_format(color))
		return (false);
	rgb = ft_split(color, ',');
	if (ft_ptrlen(rgb) != 3)
	{
		ft_free_matrix(rgb);
		return (false);
	}
	if (!verify_rgb_numbers(rgb))
	{
		ft_free_matrix(rgb);
		return (false);
	}
	ft_free_matrix(rgb);
	return (true);
}

void	validate_colors(t_game *game)
{
	if (game->texture->ceiling_color
		&& !is_valid_rgb(game->texture->ceiling_color))
		shutdown_program(game, EXIT_INVALID_RGB_COLOR);
	if (game->texture->floor_color
		&& !is_valid_rgb(game->texture->floor_color))
		shutdown_program(game, EXIT_INVALID_RGB_COLOR);
}
