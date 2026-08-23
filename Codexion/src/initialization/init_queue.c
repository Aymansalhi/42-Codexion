/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_queue.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molahrac <molahrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 23:37:23 by molahrac          #+#    #+#             */
/*   Updated: 2026/08/22 23:38:34 by molahrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

int	init_priority_queue(t_state *state, t_queue_mode mode)
{
	state->priority_queue = malloc(sizeof(t_priority_queue));
	if (!state->priority_queue)
		return (clean_and_print_err(MALLOC_ERROR, NULL, 1, state),
			EXIT_FAILURE);
	state->priority_queue->heap = malloc(sizeof(t_coder *)
			* state->cfg->number_of_coders);
	if (!state->priority_queue->heap)
		return (clean_and_print_err(MALLOC_ERROR, NULL, 1, state),
			EXIT_FAILURE);
	state->priority_queue->size = 0;
	state->priority_queue->capacity = state->cfg->number_of_coders;
	state->priority_queue->next_order = 0;
	state->priority_queue->mode = mode;
	if (pthread_mutex_init(&state->priority_queue->lock, NULL) != 0)
		return (clean_and_print_err(MUTEX_QUEUE_ERROR, NULL, 1, state),
			EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
