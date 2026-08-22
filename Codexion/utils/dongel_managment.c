/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongel_managment.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 13:28:24 by mirr              #+#    #+#             */
/*   Updated: 2026/08/21 22:01:27 by mirr             ###   ########.fr       */
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

	if (!coder->state->simulation_running)
		return ;

	coder->left_dongel->available = 0;
	pthread_mutex_lock(&coder->state->print_lock);
	time_passed = get_time_in_ms() - coder->state->start_time;
	printf("%lld %d has taken a dongle\n", time_passed, coder->id + 1);
	if (coder->left_dongel != coder->right_dongel)
	{
		coder->right_dongel->available = 0;
		time_passed = get_time_in_ms() - coder->state->start_time;
		printf("%lld %d has taken a dongle\n", time_passed, coder->id + 1);
	}
	pthread_mutex_unlock(&coder->state->print_lock);
	unlock_dongles_in_order(coder);
}

void	release_dongles(t_coder *coder)
{
	if (!coder->state->simulation_running)
		return ;
	lock_dongles_in_order(coder);
	coder->left_dongel->available = 1;
	coder->left_dongel->last_released_ms = get_time_in_ms();
	if (coder->left_dongel != coder->right_dongel)
	{
		coder->right_dongel->available = 1;
		coder->right_dongel->last_released_ms = get_time_in_ms();
	}
	unlock_dongles_in_order(coder);
	if (strcmp(coder->state->cfg->scheduler, SCHEDULER_EDF) == 0)
		pop_from_edf_queue(coder);
	else
		pop_from_queue(coder);
	pthread_cond_broadcast(&coder->state->coder_wait_cond);
}
