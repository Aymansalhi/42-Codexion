/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 22:33:41 by mirr              #+#    #+#             */
/*   Updated: 2026/08/19 01:40:58 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

void	request_compile(t_coder *coder)
{
	printf("Coder %d is pushed to the queue waiting to compile.\n", coder->id);
	push_to_queue(coder);
}

void	compile(t_coder *coder)
{
	set_burnout(coder);
	printf("%lld Coder %d is compiling for %d ms.\n",
		get_time_in_ms() - coder->state->start_time, coder->id,
		coder->state->cfg->time_to_compile);
	usleep(coder->state->cfg->time_to_compile * 1000);
	coder->compiles_done++;
	if (coder->compiles_done >= coder->state->cfg->number_of_compiles_required)
	{
		printf("%lld Coder %d finished compiling the required number of times\
			.\n", get_time_in_ms() - coder->state->start_time, coder->id);
		coder->is_finished = 1;
	}
}

void	debug(t_coder *coder)
{
	printf("%lld Coder %d is debugging for %d ms.\n",
		get_time_in_ms() - coder->state->start_time, coder->id,
		coder->state->cfg->time_to_debug);
	usleep(coder->state->cfg->time_to_debug * 1000);
}

void	refactor(t_coder *coder)
{
	printf("%lld Coder %d is refactoring for %d ms.\n",
		get_time_in_ms() - coder->state->start_time, coder->id,
		coder->state->cfg->time_to_refactor);
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
		printf("Coder %d is Ending its routine.\n", coder->id);
		printf("_____++++++Coder %d has done %d compiles.\n", coder->id, coder->compiles_done);
	}
	printf("Coder %d has finished its routine-----------------------------.\n", coder->id);
	return (NULL);
}
