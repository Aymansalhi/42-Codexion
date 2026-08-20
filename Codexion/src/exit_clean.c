/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_clean.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:26:28 by mirr              #+#    #+#             */
/*   Updated: 2026/08/12 11:34:46 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"


void	clean_and_destroy_mutexes(t_state *state)
{
	int		i;

	i = 0;
	if (state->dongels)
	{
		while (i < state->dongels_initialized)
		{
			pthread_mutex_destroy(&state->dongels[i].lock);
			i++;
		}
	}
	if (state->queue)
		pthread_mutex_destroy(&state->queue->lock);
	pthread_mutex_destroy(&state->print_lock);
}

void	clean_memory(t_state *state)
{
	int		i;

	pthread_cond_destroy(&state->coder_wait_cond);
	if (state->coders && state->cfg)
	{
		i = 0;
		while (i < state->cfg->number_of_coders)
		{
			pthread_mutex_destroy(&state->coders[i].mutex_burnout);
			i++;
		}
	}
	clean_and_destroy_mutexes(state);
	if (state->coders)
		free(state->coders);
	if (state->dongels)
		free(state->dongels);
	if (state->cfg)
		free(state->cfg);
	if (state->queue)
		free(state->queue);
	state->coders = NULL;
	state->dongels = NULL;
	state->cfg = NULL;
	state->queue = NULL;
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
