/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:38:41 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/15 17:34:53 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3d_bonus.h"

void	clean_enemy_list(t_game *game)
{
	t_enemy_list	*current;
	t_enemy_list	*next;

	if (!game || !game->enemy)
		return ;
	current = game->enemy->list;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	game->enemy->list = NULL;
}

void	clean_fire_list(t_attack *list)
{
	t_attack	*current;
	t_attack	*next;

	if (!list)
		return ;
	current = list;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
}

void	clean_door_list(t_game *game)
{
	t_door	*current;
	t_door	*next;

	if (!game || !game->doors)
		return ;
	current = game->doors;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	game->doors = NULL;
}

void	delete_texture_array(mlx_texture_t **texture_array, int len)
{
	int	i;

	if (!texture_array)
		return ;
	i = 0;
	while (i < len)
	{
		if (texture_array[i])
			mlx_delete_texture(texture_array[i]);
		i++;
	}
}
