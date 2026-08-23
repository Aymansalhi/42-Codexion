/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority_queue.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molahrac <molahrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 12:14:00 by molahrac          #+#    #+#             */
/*   Updated: 2026/08/22 23:39:34 by molahrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

static void	sift_down(t_priority_queue *queue, int index)
{
	int	left;
	int	right;
	int	smallest;

	while (1)
	{
		left = (index * 2) + 1;
		right = left + 1;
		smallest = index;
		if (left < queue->size
			&& compare_coders(queue, queue->heap[left],
				queue->heap[smallest]) < 0)
			smallest = left;
		if (right < queue->size
			&& compare_coders(queue, queue->heap[right],
				queue->heap[smallest]) < 0)
			smallest = right;
		if (smallest == index)
			break ;
		swap_heap_nodes(queue, index, smallest);
		index = smallest;
	}
}

void	pop_coder_from_priority_queue(t_priority_queue *queue, t_coder *coder)
{
	int	index;
	int	last;
	int	parent;

	if (!queue || !coder)
		return ;
	pthread_mutex_lock(&queue->lock);
	index = coder->queue_index;
	if (index < 0 || index >= queue->size || queue->heap[index] != coder)
	{
		pthread_mutex_unlock(&queue->lock);
		return ;
	}
	last = queue->size - 1;
	coder->queue_index = -1;
	queue->size--;
	if (index != last)
	{
		queue->heap[index] = queue->heap[last];
		queue->heap[index]->queue_index = index;
		parent = (index - 1) / 2;
		if (index > 0
			&& compare_coders(queue, queue->heap[index],
				queue->heap[parent]) < 0)
			sift_up(queue, index);
		else
			sift_down(queue, index);
	}
	pthread_mutex_unlock(&queue->lock);
}

t_coder	*peek_priority_queue(t_priority_queue *queue)
{
	if (!queue || queue->size == 0)
		return (NULL);
	return (queue->heap[0]);
}
