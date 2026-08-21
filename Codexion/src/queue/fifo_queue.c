/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fifo_queue.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 19:16:54 by mirr              #+#    #+#             */
/*   Updated: 2026/08/20 03:22:02 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

int	init_fifo_queue(t_state *state)
{
	state->fifo_queue = malloc(sizeof(t_fifo_queue));
	if (!state->fifo_queue)
		return (clean_and_print_err(MALLOC_ERROR, NULL, 1, state),
			EXIT_FAILURE);
	state->fifo_queue->head = NULL;
	state->fifo_queue->tail = NULL;
	state->fifo_queue->size = 0;
	return (EXIT_SUCCESS);
}

void	push_to_queue(t_coder *coder)
{
	t_fifo_queue	*fifo_queue;

	fifo_queue = coder->state->fifo_queue;
	pthread_mutex_lock(&coder->state->fifo_queue->lock);
	coder->next = NULL;
	if (fifo_queue->size == 0)
	{
		fifo_queue->head = coder;
		fifo_queue->tail = coder;
	}
	else
	{
		fifo_queue->tail->next = coder;
		fifo_queue->tail = coder;
	}
	fifo_queue->size++;
	pthread_mutex_unlock(&coder->state->fifo_queue->lock);
}

void	pop_from_queue(t_coder *coder)
{
	t_fifo_queue	*fifo_queue;

	fifo_queue = coder->state->fifo_queue;
	if (fifo_queue->size == 0)
	{
		pthread_mutex_unlock(&fifo_queue->lock);
		return ;
	}
	fifo_queue->head = coder->next;
	fifo_queue->size--;
	if (fifo_queue->size == 0)
		fifo_queue->tail = NULL;
}
