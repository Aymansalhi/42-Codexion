/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_thread.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 00:30:56 by mirr              #+#    #+#             */
/*   Updated: 2026/08/19 01:21:05 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

void	join_threads(t_state *state)
{
	int	i;

	i = 0;
	while (i < state->cfg->number_of_coders)
	{
		if (pthread_join(state->coders[i].thread, NULL))
		{
			clean_and_print_err(THREAD_JOIN_ERROR, NULL, 0, state);
		}
		i++;
	}
	if (pthread_join(state->monitor_thread, NULL))
		clean_and_print_err(THREAD_JOIN_ERROR, NULL, 0, state);
}

int	create_coder_threads(t_state *state)
{
	int	i;

	i = 0;
	while (i < state->cfg->number_of_coders)
	{
		if (pthread_create(&state->coders[i].thread, NULL,
				coder_thread_routine, &state->coders[i]) != 0)
		{
			clean_and_print_err(THREAD_CREATION_ERROR, NULL, 1, state);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

int	create_threads(t_state *state)
{
	if (pthread_create(&state->monitor_thread, NULL, monitor, state) != 0)
		return (clean_and_print_err(THREAD_CREATION_ERROR, NULL, 1, state),
			EXIT_FAILURE);
	if (create_coder_threads(state) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static void	set_start_time_burnout(t_state *state)
{
	int	i;

	state->start_time = get_time_in_ms();
	i = 0;
	while (i < state->cfg->number_of_coders)
	{
		state->coders[i].last_compile_start = state->start_time;
		i++;
	}
}

int	start_simulation(t_state *state)
{
	set_start_time_burnout(state);
	if (create_threads(state) == EXIT_FAILURE)
		return (join_threads(state), EXIT_FAILURE);
	join_threads(state);
	return (EXIT_SUCCESS);
}
