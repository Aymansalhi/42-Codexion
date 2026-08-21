/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_clean.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:26:28 by mirr              #+#    #+#             */
/*   Updated: 2026/08/20 03:47:44 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"


void	clean_and_destroy_mutexes(t_state *state)
{
	int		i;

	i = 0;

	if (state->coders && state->cfg)
	{
		i = 0;
		while (i < state->cfg->number_of_coders)
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
	if (state->fifo_queue)
		pthread_mutex_destroy(&state->fifo_queue->lock);
	pthread_mutex_destroy(&state->print_lock);
}

void	clean_edf_queue(t_state *state)
{
	t_edf_node	*current;
	t_edf_node	*next;

	if (!state->edf_queue)
		return ;
	current = state->edf_queue->head;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	state->edf_queue->head = NULL;
	state->edf_queue->tail = NULL;
	state->edf_queue->size = 0;
	free(state->edf_queue);
}

void	clean_memory(t_state *state)
{
	pthread_cond_destroy(&state->coder_wait_cond);
	clean_and_destroy_mutexes(state);
	if (state->coders)
		free(state->coders);
	if (state->dongels)
		free(state->dongels);
	if (state->cfg)
		free(state->cfg);
	if (state->fifo_queue)
		free(state->fifo_queue);
	if (state->edf_queue)
		clean_edf_queue(state);
	state->coders = NULL;
	state->dongels = NULL;
	state->cfg = NULL;
	state->fifo_queue = NULL;
	state->edf_queue = NULL;
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
