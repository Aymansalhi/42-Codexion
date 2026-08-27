/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_clean.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molahrac <molahrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:26:28 by molahrac          #+#    #+#             */
/*   Updated: 2026/08/23 17:06:56 by molahrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	clean_and_destroy_mutexes_2(t_state *state)
{
	if (state->priority_queue && state->priority_queue->lock_initialized)
		pthread_mutex_destroy(&state->priority_queue->lock);
	if (state->print_lock_initialized)
		pthread_mutex_destroy(&state->print_lock);
	if (state->state_lock_initialized)
	{
		pthread_mutex_destroy(&state->state_lock);
		state->state_lock_initialized = 0;
	}
}

void	clean_and_destroy_mutexes(t_state *state)
{
	int		i;

	i = 0;
	if (state->coders && state->cfg)
	{
		i = 0;
		while (i < state->coders_initialized)
		{
			pthread_mutex_destroy(&state->coders[i].mutex_burnout);
			i++;
		}
	}
	i = 0;
	if (state->dongels)
	{
		while (i < state->dongels_initialized)
		{
			pthread_mutex_destroy(&state->dongels[i].lock);
			i++;
		}
	}
	clean_and_destroy_mutexes_2(state);
}

void	clean_priority_queue(t_state *state)
{
	if (!state->priority_queue)
		return ;
	free(state->priority_queue->heap);
	state->priority_queue->heap = NULL;
	state->priority_queue->size = 0;
	state->priority_queue->capacity = 0;
	free(state->priority_queue);
}

void	clean_memory(t_state *state)
{
	if (state->coder_wait_cond_initialized)
		pthread_cond_destroy(&state->coder_wait_cond);
	clean_and_destroy_mutexes(state);
	if (state->coders)
		free(state->coders);
	if (state->dongels)
		free(state->dongels);
	if (state->cfg)
		free(state->cfg);
	if (state->priority_queue)
		clean_priority_queue(state);
	state->coders = NULL;
	state->dongels = NULL;
	state->cfg = NULL;
	state->priority_queue = NULL;
}

void	clean_and_print_err(char *err, char *details, int clean, t_state *state)
{
	fprintf(stderr, "ERROR: %s", err);
	if (details)
		fprintf(stderr, " (%s)", details);
	if (clean)
		clean_memory(state);
	fprintf(stderr, "\n");
}
