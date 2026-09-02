/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongel_managment.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molahrac <molahrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 13:28:24 by molahrac          #+#    #+#             */
/*   Updated: 2026/08/23 16:51:36 by molahrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

int	dongel_ready(t_dongel *dongel, t_state *state)
{
	long long	now;

	now = get_time_in_ms();
	if (dongel->available == 0)
		return (0);
	if (dongel->last_released_ms != -1
		&& now - dongel->last_released_ms < state->cfg->dongle_cooldown)
		return (0);
	return (1);
}

void	take_dongels(t_coder *coder)
{
	long long	time_passed;

	if (!simulation_is_running(coder->state))
		return ;
	pthread_mutex_lock(&coder->state->print_lock);
	time_passed = get_time_in_ms() - coder->state->start_time;
	printf("%lld %d has taken a dongle\n", time_passed, coder->id + 1);
	if (coder->left_dongel != coder->right_dongel)
	{
		time_passed = get_time_in_ms() - coder->state->start_time;
		printf("%lld %d has taken a dongle\n", time_passed, coder->id + 1);
	}
	pthread_mutex_unlock(&coder->state->print_lock);
}

void	release_dongles(t_coder *coder)
{
	coder->left_dongel->available = 1;
	coder->left_dongel->last_released_ms = get_time_in_ms();
	if (coder->left_dongel != coder->right_dongel)
	{
		coder->right_dongel->available = 1;
		coder->right_dongel->last_released_ms = get_time_in_ms();
	}
	unlock_dongles_mutex_in_order(coder);
	pthread_mutex_lock(&coder->state->priority_queue->lock);
	if (strcmp(coder->state->cfg->scheduler, SCHEDULER_EDF) == 0)
		pop_coder_from_priority_queue_locked(
			coder->state->priority_queue, coder);
	else
		pop_coder_from_priority_queue_locked(
			coder->state->priority_queue, coder);
	pthread_cond_broadcast(&coder->state->coder_wait_cond);
	pthread_mutex_unlock(&coder->state->priority_queue->lock);
}
