/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molahrac <molahrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 20:22:02 by molahrac          #+#    #+#             */
/*   Updated: 2026/08/22 02:02:57 by molahrac         ###   ########.fr       */
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
		return (EXIT_FAILURE);
	if (!ft_validate_field(val, i))
		return (EXIT_FAILURE);
	ft_set_config_value(configs, val, i);
	return (EXIT_SUCCESS);
}

int	ft_validate_scheduler(const char *scheduler)
{
	if (!scheduler || !*scheduler)
		return (EXIT_FAILURE);
	if (strcmp(scheduler, "fifo") == 0)
		return (EXIT_SUCCESS);
	if (strcmp(scheduler, "edf") == 0)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int	ft_parsing_args(int argc, char **argv, t_state *state)
{
	int			i;

	if (argc != 9)
		return (clean_and_print_err(INVALD_ARGS, NULL, 0, state), EXIT_FAILURE);
	state->cfg = (t_config *)malloc(sizeof(t_config));
	if (!state->cfg)
		return (clean_and_print_err(MALLOC_ERROR, NULL, 0, state),
			EXIT_FAILURE);
	memset(state->cfg, 0, sizeof(t_config));
	i = 1;
	while (i <= 7)
	{
		if (ft_process_numeric_arg(state->cfg, argv[i], i))
			return (clean_and_print_err(INVALID_ARG, argv[i], 1, state),
				EXIT_FAILURE);
		i++;
	}
	if (ft_validate_scheduler(argv[8]))
		return (clean_and_print_err(INVALID_SCHEDULER, argv[8], 1, state),
			EXIT_FAILURE);
	ft_set_scheduler(state->cfg, argv[8]);
	return (EXIT_SUCCESS);
}
