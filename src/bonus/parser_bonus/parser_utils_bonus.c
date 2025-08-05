/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:45:03 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/05 16:29:17 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

void	check_player(t_game *game)
{
	if (game->player->p > 1)
		shutdown_program(game, EXIT_TOO_MANY_START_POS);
	else if (game->player->p < 1)
		shutdown_program(game, EXIT_NO_START_POSITION);
}

bool	only_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isspace(str[i]))
			return (false);
		i++;
	}
	return (true);
}

bool	has_control_char(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] >= 9 && str[i] <= 13)
			return (true);
		i++;
	}
	return (false);
}

bool	is_valid_line(char *content)
{
	int	i;

	i = 0;
	while (content[i])
	{
		if (!ft_strchr(" 01NSWEID", content[i]))
			return (false);
		i++;
	}
	return (true);
}

bool	startswith(char *str, char *start)
{
	char	*trimmed;
	int		len;

	len = ft_strlen(start);
	trimmed = ft_strtrim(str, " ");
	if (ft_strnstr(trimmed, start, len) && trimmed[len] == 32)
	{
		free(trimmed);
		return (true);
	}
	free(trimmed);
	return (false);
}
