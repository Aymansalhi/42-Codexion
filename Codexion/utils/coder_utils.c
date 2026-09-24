/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molahrac <molahrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 00:21:13 by molahrac          #+#    #+#             */
/*   Updated: 2026/08/23 19:26:19 by molahrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	init_coder_fields(t_coder *coder, int id,
		t_dongel *left, t_dongel *right)
{
	coder->id = id;
	coder->left_dongel = left;
	coder->right_dongel = right;
	coder->queue_index = -1;
	coder->queue_order = 0;
	coder->is_finished = 0;
	coder->compiles_done = 0;
	coder->last_compile_start = 0;
}

void	lock_dongles_mutex_in_order(t_coder *coder)
{
	t_dongel	*first;
	t_dongel	*second;

	if (coder->left_dongel == coder->right_dongel)
	{
		pthread_mutex_lock(&coder->left_dongel->lock);
		return ;
	}
	first = coder->left_dongel;
	second = coder->right_dongel;
	if (first->id > second->id)
	{
		first = coder->right_dongel;
		second = coder->left_dongel;
	}
	pthread_mutex_lock(&first->lock);
	pthread_mutex_lock(&second->lock);
}

void	unlock_dongles_mutex_in_order(t_coder *coder)
{
	if (coder->left_dongel == coder->right_dongel)
	{
		pthread_mutex_unlock(&coder->left_dongel->lock);
		return ;
	}
	pthread_mutex_unlock(&coder->left_dongel->lock);
	pthread_mutex_unlock(&coder->right_dongel->lock);
}

int	dongels_are_ready(t_coder *coder)
{
	int	ready;
	t_dongel	*first;
	t_dongel	*second;

	if (coder->left_dongel == coder->right_dongel)
		return (0);
	first = coder->left_dongel;
	second = coder->right_dongel;
	if (first->id > second->id)
	{
		first = coder->right_dongel;
		second = coder->left_dongel;
	}
	if (pthread_mutex_trylock(&first->lock) != 0)
		return (0);
	if (pthread_mutex_trylock(&second->lock) != 0)
	{
		pthread_mutex_unlock(&first->lock);
		return (0);
	}
	ready = dongel_ready(coder->left_dongel, coder->state)
		&& dongel_ready(coder->right_dongel, coder->state);
	pthread_mutex_unlock(&second->lock);
	pthread_mutex_unlock(&first->lock);
	return (ready);
}

int	coder_is_schedulable(t_coder *coder)
{
	t_priority_queue	*queue;
	t_coder				*best;
	int					i;

	queue = coder->state->priority_queue;
	best = NULL;
	i = 0;
	while (i < queue->size)
	{
		if (dongels_are_ready(queue->heap[i])
			&& (!best || compare_coders(queue, queue->heap[i], best) < 0))
			best = queue->heap[i];
		i++;
	}
	return (best == coder);
}

void	wait_for_scheduler_allows_me_and_get_dongles(t_coder *coder)
{
	t_priority_queue	*queue;

	queue = coder->state->priority_queue;
	pthread_mutex_lock(&queue->lock);
	while (simulation_is_running(coder->state)
		&& !coder_is_schedulable(coder))
	{
		pthread_cond_wait(
			&coder->state->coder_wait_cond, &queue->lock);
	}
	if (!simulation_is_running(coder->state))
	{
		pthread_mutex_unlock(&queue->lock);
		return ;
	}
	lock_dongles_mutex_in_order(coder);
	coder->left_dongel->available = 0;
	if (coder->left_dongel != coder->right_dongel)
		coder->right_dongel->available = 0;
	pthread_mutex_unlock(&queue->lock);
}
