/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 19:16:54 by mirr              #+#    #+#             */
/*   Updated: 2026/08/20 02:51:47 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	push_to_queue(t_coder *coder)
{
	t_fifo_queue	*queue;

	queue = coder->state->queue;
	pthread_mutex_lock(&coder->state->queue->lock);
	coder->next = NULL;
	if (queue->size == 0)
	{
		queue->head = coder;
		queue->tail = coder;
	}
	else
	{
		queue->tail->next = coder;
		queue->tail = coder;
	}
	queue->size++;
	pthread_mutex_unlock(&coder->state->queue->lock);
}

void	pop_from_queue(t_coder *coder)
{
	t_fifo_queue	*queue;

	queue = coder->state->queue;
	if (queue->size == 0)
	{
		pthread_mutex_unlock(&queue->lock);
		return ;
	}
	queue->head = coder->next;
	queue->size--;
	if (queue->size == 0)
		queue->tail = NULL;
}
