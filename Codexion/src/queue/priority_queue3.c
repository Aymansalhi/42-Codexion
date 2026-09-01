/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority_queue3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molahrac <molahrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 23:35:07 by molahrac          #+#    #+#             */
/*   Updated: 2026/08/22 23:39:54 by molahrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

long long	get_deadline(t_coder *coder)
{
	return (coder->last_compile_start + coder->state->cfg->time_to_burnout);
}

int	compare_by_id(t_coder *a, t_coder *b)
{
	if (a->id < b->id)
		return (-1);
	if (a->id > b->id)
		return (1);
	return (0);
}

void	swap_heap_nodes(t_priority_queue *queue, int a, int b)
{
	t_coder	*tmp;

	tmp = queue->heap[a];
	queue->heap[a] = queue->heap[b];
	queue->heap[b] = tmp;
	queue->heap[a]->queue_index = a;
	queue->heap[b]->queue_index = b;
}
