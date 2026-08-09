/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 19:03:48 by mirr              #+#    #+#             */
/*   Updated: 2026/08/09 23:33:03 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

int	create_coders_and_dongles(t_state *state)
{
	state->coders = malloc(sizeof(t_coder) * state->cfg->number_of_coders);
	if (!state->coders)
		return (EXIT_FAILURE);
	state->dongels = malloc(sizeof(t_dongel) * state->cfg->number_of_coders);
	if (!state->dongels)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	init_dongles(t_state *state)
{
	int	i;

	i = 0;
	while (i < state->cfg->number_of_coders)
	{
		state->dongels[i].id = i;
		state->dongels[i].available = 1;
		if (!pthread_mutex_init(&state->dongels[i].lock, NULL))
			return (printf("Mutex initialization failed\n"), EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	init_coders(t_state *state)
{
}

int	create_and_init_coders_and_dongles(t_state *state)
{
	if (create_coders_and_dongles(state) == EXIT_FAILURE)
		return (EXIT_FAILURE);

	if (!init_dongles(state) || !init_coders(state))
		return ()

	return (EXIT_SUCCESS);
}
