/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_enemies_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 18:20:34 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/08/14 18:20:53 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/cub3d_bonus.h"

static bool	ft_swap(t_enemy_list **head, t_enemy_list *cur, t_enemy_list *next)
{
	if (!head || !cur || !next || cur == next)
		return (false);
	if (cur->prev)
		cur->prev->next = next;
	else
		*head = next;
	if (next->next)
		next->next->prev = cur;
	next->prev = cur->prev;
	cur->prev = next;
	cur->next = next->next;
	next->next = cur;
	return (true);
}

static bool	ft_is_sorted(t_game *game)
{
	t_enemy_list	*nav;

	nav = game->enemy->list;
	while (nav->next)
	{
		if (nav->distance < nav->next->distance)
			return (false);
		nav = nav->next;
	}
	return (true);
}

void	sort_enemies(t_game *game, t_enemy_list *nav)
{
	if (!nav->next)
		return ;
	while (!ft_is_sorted(game))
	{
		nav = game->enemy->list;
		while (nav->next)
		{
			if (nav->distance < nav->next->distance)
				if (ft_swap(&game->enemy->list, nav, nav->next))
					break ;
			nav = nav->next;
		}
	}
}
