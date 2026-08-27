/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molahrac <molahrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 19:03:48 by molahrac          #+#    #+#             */
/*   Updated: 2026/08/23 17:10:10 by molahrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

int	create_coders_and_dongles_and_queue(t_state *state)
{
	state->coders = malloc(sizeof(t_coder) * state->cfg->number_of_coders);
	if (!state->coders)
		return (clean_and_print_err(MALLOC_ERROR, NULL, 1, state),
			EXIT_FAILURE);
	state->dongels = malloc(sizeof(t_dongel) * state->cfg->number_of_coders);
	if (!state->dongels)
		return (clean_and_print_err(MALLOC_ERROR, NULL, 1, state),
			EXIT_FAILURE);
	if (strcmp(state->cfg->scheduler, SCHEDULER_FIFO) == 0)
	{
		if (init_fifo_queue(state) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else if (strcmp(state->cfg->scheduler, SCHEDULER_EDF) == 0)
	{
		if (init_edf_queue(state) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
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
		state->dongels[i].last_released_ms = -1;
		if (pthread_mutex_init(&state->dongels[i].lock, NULL) != 0)
		{
			state->dongels_initialized = i;
			return (clean_and_print_err(MUTEX_DONGEL_ERROR, NULL, 1, state),
				EXIT_FAILURE);
		}
		i++;
	}
	state->dongels_initialized = i;
	return (EXIT_SUCCESS);
}

int	init_coders(t_state *state)
{
	int		i;
	int		tmp_wrapper;

	i = 0;
	if (state->cfg->number_of_coders > 1)
	{
		while (i < state->cfg->number_of_coders)
		{
			tmp_wrapper = (i + 1) % state->cfg->number_of_coders;
			init_coder_fields(&state->coders[i], i,
				&state->dongels[i], &state->dongels[tmp_wrapper]);
			state->coders[i].state = state;
			if (pthread_mutex_init(&state->coders[i].mutex_burnout, NULL) != 0)
				return (clean_and_print_err("Failed to init coder mutex",
						NULL, 1, state), EXIT_FAILURE);
			state->coders_initialized++;
			i++;
		}
	}
	else if (state->cfg->number_of_coders == 1)
		if (handle_single_coder_case(state) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	init_mutexes_and_cond(t_state *state)
{
	if (pthread_cond_init(&state->coder_wait_cond, NULL) != 0)
		return (clean_and_print_err(COND_QUEUE_ERROR, NULL, 1, state),
			EXIT_FAILURE);
	state->coder_wait_cond_initialized = 1;
	if (pthread_mutex_init(&state->print_lock, NULL) != 0)
		return (clean_and_print_err("Failed to init print mutex",
				NULL, 1, state), EXIT_FAILURE);
	state->print_lock_initialized = 1;
	return (EXIT_SUCCESS);
}

int	init_and_setup_all(t_state *state)
{
	if (create_coders_and_dongles_and_queue(state) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_dongles(state) || init_coders(state))
		return (EXIT_FAILURE);
	if (init_mutexes_and_cond(state) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
