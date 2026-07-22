/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 20:22:02 by mirr              #+#    #+#             */
/*   Updated: 2026/07/22 16:59:29 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	ft_set_config_value(t_config *configs, char *va, int i)
{
	if (i == 1)
		configs->number_of_coders = atoi(va);
	else if (i == 2)
		configs->time_to_burnout = atoi(va);
	else if (i == 3)
		configs->time_to_compile = atoi(va);
	else if (i == 4)
		configs->time_to_debug = atoi(va);
	else if (i == 5)
		configs->time_to_refactor = atoi(va);
	else if (i == 6)
		configs->number_of_compiles_required = atoi(va);
	else if (i == 7)
		configs->dongle_cooldown = atoi(va);
	else if (i == 8)
		configs->scheduler = va;
}


t_config	*ft_parsing_args(int argc, char **argv)
{
	int			i;
	t_config	*configs;

	if (argc != 9)
		return (printf("Error: Invalid number of arguments.\n"), NULL);
	if (atoi(argv[1]) <= 0 || atoi(argv[2]) <= 0 || atoi(argv[3]) <= 0 ||
		atoi(argv[4]) <= 0 || atoi(argv[5]) <= 0 || atoi(argv[6]) <= 0 ||
		atoi(argv[7]) <= 0)
		return (printf("Error: Invalid argument values.\n"), NULL);

	configs = (t_config *)malloc(sizeof(t_config));
	if (!configs)
		return (NULL);
	memset(configs, 0, sizeof(t_config));
	i = 1;
	while (argc != i)
	{
		ft_set_config_value(configs, argv[i], i);
		i += 1;
	}
	return (configs);
}
