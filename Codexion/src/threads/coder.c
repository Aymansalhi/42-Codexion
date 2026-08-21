/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 22:33:41 by mirr              #+#    #+#             */
/*   Updated: 2026/08/20 03:39:58 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

void	request_compile(t_coder *coder)
{
	if (coder->state->cfg->scheduler == SCHEDULER_EDF)
		push_to_edf_queue(coder);
	else
		push_to_queue(coder);
}

void	compile(t_coder *coder)
{
	set_burnout(coder);
	pthread_mutex_lock(&coder->state->print_lock);
	printf("%lld %d is compiling\n",
		get_time_in_ms() - coder->state->start_time, coder->id + 1);
	pthread_mutex_unlock(&coder->state->print_lock);
	usleep(coder->state->cfg->time_to_compile * 1000);
	coder->compiles_done++;
	if (coder->compiles_done >= coder->state->cfg->number_of_compiles_required)
	{
		coder->is_finished = 1;
	}
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

	while (!coder->is_finished && coder->state->simulation_running)
	{
		request_compile(coder);
		wait_until_scheduler_allows_me(coder);
		if (!coder->state->simulation_running)
			break ;
		take_dongels(coder);
		if (!coder->state->simulation_running)
			break ;
		compile(coder);
		release_dongles(coder);
		if (!coder->state->simulation_running)
			break ;
		debug(coder);
		if (!coder->state->simulation_running)
			break ;
		refactor(coder);
	}
	return (NULL);
}
