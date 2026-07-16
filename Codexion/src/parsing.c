/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 20:22:02 by mirr              #+#    #+#             */
/*   Updated: 2026/07/16 22:09:04 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/codexion.h"

t_config	*ft_parsing_args(int arg_len, char **args)
{
	int			i;
	t_config	*configs;

	configs = (t_config *)malloc(sizeof(t_config));
	if (!configs)
		return (NULL);

	i = 0;
	while (i != arg_len)
	{
		/* Parsiing Logic*/
	}

}
