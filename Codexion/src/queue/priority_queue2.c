/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority_queue2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molahrac <molahrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 13:28:35 by molahrac          #+#    #+#             */
/*   Updated: 2026/09/20 16:33:53 by molahrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

static int	compare_fifo_order(t_coder *a, t_coder *b)
{
	if (a->queue_order < b->queue_order)
		return (-1);
	if (a->queue_order > b->queue_order)
		return (1);
	return (compare_by_id(a, b));
}

static int	compare_edf_deadline(t_coder *a, t_coder *b)
{
	long long	deadline_a;
	long long	deadline_b;

	deadline_a = get_deadline(a);
	deadline_b = get_deadline(b);
	if (deadline_a < deadline_b)
		return (-1);
	if (deadline_a > deadline_b)
		return (1);
	return (compare_fifo_order(a, b));
}

int	compare_coders(t_priority_queue *queue, t_coder *a, t_coder *b)
{
	if (queue->mode == QUEUE_FIFO)
		return (compare_fifo_order(a, b));
	if (queue->mode == QUEUE_EDF)
		return (compare_edf_deadline(a, b));
	return (0);
}

/**
 * *sift_up() - restore the tree so it's valid again: if the (element[index])
 * is smaller than its parent, swap them, and keep going up until
 * the root or until is the min
 *
 * @queue: the priority queue whose heap array is being fixed
 * @index: the array position of the element to sift upward
 *         (normally the last slot, right after a push)
 */
void	sift_up(t_priority_queue *queue, int index)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (compare_coders(queue, queue->heap[index], queue->heap[parent]) >= 0)
			break ;
		swap_heap_nodes(queue, index, parent);
		index = parent;
	}
}

void	push_to_priority_queue(t_priority_queue *queue, t_coder *coder)
{
	if (!queue)
		return ;
	pthread_mutex_lock(&queue->lock);
	if (queue->size >= queue->capacity || coder->queue_index != -1)
	{
		pthread_mutex_unlock(&queue->lock);
		return ;
	}
	coder->queue_order = queue->next_order++;
	queue->heap[queue->size] = coder;
	coder->queue_index = queue->size;
	queue->size++;
	sift_up(queue, coder->queue_index);
	pthread_mutex_unlock(&queue->lock);
}
