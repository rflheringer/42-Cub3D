/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:30:52 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/25 16:20:24 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_substr(char const *s, int start, int len)
{
	char	*sub;
	int		j;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	if (start + len > ft_strlen(s))
		len = ft_strlen(s) - start;
	sub = (char *)malloc((len + 1) * sizeof(char));
	if (!sub)
		return (NULL);
	j = 0;
	while (j < len && s[start + j] != '\0')
	{
		sub[j] = s[start + j];
		j++;
	}
	sub[j] = '\0';
	return (sub);
}
