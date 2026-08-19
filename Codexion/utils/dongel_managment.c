/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine_tasks.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 13:28:24 by mirr              #+#    #+#             */
/*   Updated: 2026/08/18 11:45:56 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

static int	dongel_ready(t_dongel *dongel, t_state *state)
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
	int			ready;

	while (coder->state->simulation_running)
	{
		lock_dongles_in_order(coder);
		ready = dongel_ready(coder->left_dongel, coder->state)
			&& dongel_ready(coder->right_dongel, coder->state);
		if (ready)
			break ;
		unlock_dongles_in_order(coder);
		usleep(1000);
	}
	if (!coder->state->simulation_running)
		return ;
	time_passed = get_time_in_ms() - coder->state->start_time;
	coder->left_dongel->available = 0;
	printf("%lld coder %d has taken a dongle\n", time_passed, coder->id);
	if (coder->left_dongel != coder->right_dongel)
	{
		coder->right_dongel->available = 0;
		printf("%lld coder %d has taken a dongle\n", time_passed, coder->id);
	}
	unlock_dongles_in_order(coder);
}

void	release_dongles(t_coder *coder)
{
	long long	time_passed;

	if (!coder->state->simulation_running)
		return ;
	lock_dongles_in_order(coder);
	time_passed = get_time_in_ms() - coder->state->start_time;
	coder->left_dongel->available = 1;
	coder->left_dongel->last_released_ms = get_time_in_ms();
	printf("%lld %d has released a dongle\n", time_passed, coder->id);
	if (coder->left_dongel != coder->right_dongel)
	{
		coder->right_dongel->available = 1;
		coder->right_dongel->last_released_ms = get_time_in_ms();
		printf("%lld %d has released a dongle\n", time_passed, coder->id);
	}
	unlock_dongles_in_order(coder);
	pop_from_queue(coder);
	pthread_cond_broadcast(&coder->state->coder_wait_cond);
}
