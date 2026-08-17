/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 19:03:48 by mirr              #+#    #+#             */
/*   Updated: 2026/08/17 21:04:51 by mirr             ###   ########.fr       */
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
	state->queue = malloc(sizeof(t_queue));
	if (!state->queue)
		return (clean_and_print_err(MALLOC_ERROR, NULL, 1, state),
			EXIT_FAILURE);
	state->queue->head = NULL;
	state->queue->tail = NULL;
	state->queue->size = 0;
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
		state->dongels[i].cooldown = state->cfg->dongle_cooldown;
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
			state->coders[i].id = i;
			state->coders[i].left_dongel = &state->dongels[i];
			tmp_wrapper = (i + 1) % state->cfg->number_of_coders;
			state->coders[i].right_dongel = &state->dongels[tmp_wrapper];
			state->coders[i].state = state;
			i++;
		}
	}
	else if (state->cfg->number_of_coders == 1)
	{
		state->coders[0].id = 0;
		state->coders[0].left_dongel = &state->dongels[0];
		state->coders[0].right_dongel = &state->dongels[0];
		state->coders[0].state = state;
	}
	return (EXIT_SUCCESS);
}

int	init_mutexes_and_cond(t_state *state)
{
	if (pthread_mutex_init(&state->queue->lock, NULL) != 0)
		return (clean_and_print_err(MUTEX_QUEUE_ERROR, NULL, 1, state),
			EXIT_FAILURE);
	if (pthread_cond_init(&state->coder_wait_cond, NULL) != 0)
		return (clean_and_print_err(COND_QUEUE_ERROR, NULL, 1, state),
			EXIT_FAILURE);
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
