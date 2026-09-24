/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molahrac <molahrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 22:33:41 by molahrac          #+#    #+#             */
/*   Updated: 2026/08/23 16:27:17 by molahrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

void	request_compile(t_coder *coder)
{
	if (strcmp(coder->state->cfg->scheduler, SCHEDULER_EDF) == 0)
		push_to_edf_queue(coder);
	else
		push_to_queue(coder);
}

void	compile(t_coder *coder)
{
	set_burnout(coder);
	printf("%lld %d is compiling\n",
		get_time_in_ms() - coder->state->start_time, coder->id + 1);
	pthread_mutex_unlock(&coder->state->print_lock);
	usleep(coder->state->cfg->time_to_compile * 1000);
	pthread_mutex_lock(&coder->state->state_lock);
	coder->compiles_done++;
	if (coder->compiles_done >= coder->state->cfg->number_of_compiles_required)
	{
		coder->is_finished = 1;
	}
	pthread_mutex_unlock(&coder->state->state_lock);
}

void	debug(t_coder *coder)
{
	pthread_mutex_lock(&coder->state->print_lock);
	printf("%lld %d is debugging\n",
		get_time_in_ms() - coder->state->start_time, coder->id + 1);
	pthread_mutex_unlock(&coder->state->print_lock);
	usleep(coder->state->cfg->time_to_debug * 1000);
}

void	refactor(t_coder *coder)
{
	pthread_mutex_lock(&coder->state->print_lock);
	printf("%lld %d is refactoring\n",
		get_time_in_ms() - coder->state->start_time, coder->id + 1);
	pthread_mutex_unlock(&coder->state->print_lock);
	usleep(coder->state->cfg->time_to_refactor * 1000);
}

void	*coder_thread_routine(void *arg)
{
	t_coder		*coder;

	coder = (t_coder *)arg;
	while (!coder_is_finished(coder) && simulation_is_running(coder->state))
	{
		request_compile(coder);
		wait_for_scheduler_allows_me_and_get_dongles(coder);
		if (!simulation_is_running(coder->state))
			return (NULL);
		take_dongels(coder);
		if (!simulation_is_running(coder->state))
		{
			pthread_mutex_unlock(&coder->state->print_lock);
			return (unlock_dongles_mutex_in_order(coder), NULL);
		}
		compile(coder);
		release_dongles(coder);
		if (!simulation_is_running(coder->state))
			return (NULL);
		debug(coder);
		if (!simulation_is_running(coder->state))
			return (NULL);
		refactor(coder);
	}
	return (NULL);
}
