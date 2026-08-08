/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 20:22:02 by mirr              #+#    #+#             */
/*   Updated: 2026/08/08 15:37:31 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	ft_set_scheduler(t_config *configs, char *s)
{
	configs->scheduler = s;
}

int	ft_process_numeric_arg(t_config *configs, char *arg, int i)
{
	long	val;

	if (ft_strict_int(arg, &val))
		return (fprintf(stderr, "Error: Invalid argument: %s\n", arg), 0);
	if (!ft_validate_field(val, i))
		return (fprintf(stderr, "Error: Invalid argument: %s\n", arg), 0);
	ft_set_config_value(configs, val, i);
	return (1);
}

int	ft_validate_scheduler(const char *scheduler)
{
	if (!scheduler || !*scheduler)
		return (0);
	if (strcmp(scheduler, "fifo") == 0)
		return (1);
	if (strcmp(scheduler, "edf") == 0)
		return (1);
	return (0);
}

t_config	*ft_parsing_args(int argc, char **argv)
{
	int			i;
	t_config	*configs;

	if (argc != 9)
		return (fprintf(stderr, "Error: Invalid number of arguments.\n"), NULL);
	configs = (t_config *)malloc(sizeof(t_config));
	if (!configs)
		return (NULL);
	memset(configs, 0, sizeof(t_config));
	i = 1;
	while (i <= 7)
	{
		if (!ft_process_numeric_arg(configs, argv[i], i))
			return (free(configs), NULL);
		i++;
	}
	if (!ft_validate_scheduler(argv[8]))
		return (fprintf(stderr, "Error: Invalid scheduler: %s\n", argv[8]),
			free(configs), NULL);
	ft_set_scheduler(configs, argv[8]);
	return (configs);
}
