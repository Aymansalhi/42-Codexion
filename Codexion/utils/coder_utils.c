/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 00:21:13 by mirr              #+#    #+#             */
/*   Updated: 2026/08/19 01:40:52 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	init_coder_fields(t_coder *coder, int id,
		t_dongel *left, t_dongel *right)
{
	coder->id = id;
	coder->left_dongel = left;
	coder->right_dongel = right;
	coder->is_finished = 0;
	coder->compiles_done = 0;
	coder->last_compile_start = 0;
}

void	set_burnout(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex_burnout);
	coder->last_compile_start = get_time_in_ms();
	pthread_mutex_unlock(&coder->mutex_burnout);
}

void	lock_dongles_in_order(t_coder *coder)
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

void	unlock_dongles_in_order(t_coder *coder)
{
	if (coder->left_dongel == coder->right_dongel)
	{
		pthread_mutex_unlock(&coder->left_dongel->lock);
		return ;
	}
	pthread_mutex_unlock(&coder->left_dongel->lock);
	pthread_mutex_unlock(&coder->right_dongel->lock);
}

void	wait_until_scheduler_allows_me(t_coder *coder)
{
	t_queue	*queue;

	queue = coder->state->queue;
	pthread_mutex_lock(&queue->lock);
	while (coder->state->simulation_running && queue->head != coder)
	{
		printf("Coder %d is (FREEZED) until it get UNFREEZED.\n", coder->id);
		pthread_cond_wait(
			&coder->state->coder_wait_cond, &queue->lock);
	}
	if (!coder->state->simulation_running)
	{
		pthread_mutex_unlock(&queue->lock);
		return ;
	}
	printf("Coder %d is allowed to compile now.\n", coder->id);
	pthread_mutex_unlock(&queue->lock);
}
