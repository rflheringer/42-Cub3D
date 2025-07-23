/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:26:22 by marvin            #+#    #+#             */
/*   Updated: 2025/07/23 13:26:22 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d.h"

void	read_file(t_game *game, int	fd)
{
	char	*line;
	char	*temp;
	char	*aux;

	line = ft_strdup("");
	while (1)
	{
		temp = get_next_line(fd);
		if (!temp)
			break ;
		aux = ft_strjoin(line, temp);
		free(line);
		free(temp);
		line = aux;
	}
	game->map->file_content = ft_split(line, '\n');
	free(line);
}

void	validate_file(t_game *game, char *file)
{
	int	fd;

	if (!ft_strnstr(file, ".cub", ft_strlen(file)))
	{
		(void)game;
		exit(1);
	}
	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		(void)game;
		exit(1);
	}
	read_file(game, fd);
}
#include <stdio.h>
int	count_matrix(char **matrix)
{
	char	*temp;
	int		len;
	int		i;

	i = 0;
	len = 0;
	while (matrix[i])
	{
		temp = ft_strtrim(matrix[i], " \t");
		printf("TEMP:'%s'\n", temp);
		if (temp[0] == '\0')
		{
			printf("here\n");
			free(temp);
			i++;
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

void	ft_free_matrix(char **ptr_matrix)
{
	int	i;

	i = 0;
	if (!ptr_matrix)
		return ;
	while (ptr_matrix[i] != NULL)
	{
		free(ptr_matrix[i]);
		i++;
	}
	free(ptr_matrix);
	ptr_matrix = NULL;
}

void	clear_white_spaces(t_game *game, char **content)
{
	char	**new_content;
	char	*temp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_content = ft_calloc(count_matrix(content), sizeof(char *));
	if (!new_content)
	{
		exit(1);
	}
	while (content[i])
	{
		temp = ft_strtrim(content[i], " \t");
		if (temp[0] == '\0')
		{
			free(temp);
			i++;
		}
		else
		{
			new_content[j] = temp;
			j++;
			i++;
		}
	}
	ft_free_matrix(game->map->file_content);
	game->map->file_content = new_content;
}

void	parser(t_game *game, char *file)
{
	validate_file(game, file);
	clear_white_spaces(game, game->map->file_content);
}