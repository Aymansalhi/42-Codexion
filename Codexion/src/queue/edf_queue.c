/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edf_queue.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 03:18:49 by mirr              #+#    #+#             */
/*   Updated: 2026/08/20 03:57:01 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

int	init_edf_queue(t_state *state)
{
	state->edf_queue = malloc(sizeof(t_edf_queue));
	if (!state->edf_queue)
		return (clean_and_print_err(MALLOC_ERROR, NULL, 1, state),
			EXIT_FAILURE);
	state->edf_queue->head = NULL;
	state->edf_queue->tail = NULL;
	state->edf_queue->size = 0;
	return (EXIT_SUCCESS);
}

void	push_to_edf_queue(t_coder *coder)
{
	t_edf_queue	*edf_queue;
	t_edf_node	*new_node;

	edf_queue = coder->state->edf_queue;
	new_node = malloc(sizeof(t_edf_node));
	if (!new_node)
		return (clean_and_print_err(MALLOC_ERROR, NULL, 1, coder->state));
	new_node->coder = coder;
	new_node->prev = NULL;
	new_node->next = NULL;
	pthread_mutex_lock(&coder->state->edf_queue->lock);
	if (edf_queue->size == 0)
	{
		edf_queue->head = new_node;
		edf_queue->tail = new_node;
	}
	else
	{
		edf_queue->tail->next = new_node;
		edf_queue->tail = new_node;
	}
	edf_queue->size++;
	pthread_mutex_unlock(&coder->state->edf_queue->lock);
}

// @TODO: implement EDF queue pop function