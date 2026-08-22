/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 10:32:12 by mirr              #+#    #+#             */
/*   Updated: 2026/08/21 22:06:24 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	handle_fifo_scheduler(t_coder *coder)
{
	t_fifo_queue	*fifo_queue;

	fifo_queue = coder->state->fifo_queue;
	if (!fifo_queue)
		return ;
	pthread_mutex_lock(&fifo_queue->lock);
	while (coder->state->simulation_running && fifo_queue->head != coder)
		pthread_cond_wait(
			&coder->state->coder_wait_cond, &fifo_queue->lock);
	pthread_mutex_unlock(&fifo_queue->lock);
}

void	handle_edf_scheduler(t_coder *coder)
{
	t_edf_queue	*edf_queue;
	t_edf_node	*earliest;

	edf_queue = coder->state->edf_queue;
	if (!edf_queue)
		return ;
	pthread_mutex_lock(&edf_queue->lock);
	while (coder->state->simulation_running)
	{
		earliest = find_coder_closest_to_burnout(edf_queue);
		if (earliest && earliest->coder == coder)
			break ;
		pthread_cond_wait(
			&coder->state->coder_wait_cond, &edf_queue->lock);
	}
	pthread_mutex_unlock(&edf_queue->lock);
}
