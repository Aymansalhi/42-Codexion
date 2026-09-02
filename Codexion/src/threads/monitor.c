/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molahrac <molahrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 10:55:46 by molahrac          #+#    #+#             */
/*   Updated: 2026/08/22 02:09:36 by molahrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

static int	coder_burned_out(t_state *state, t_coder *coder, long long now)
{
	long long	last_start;

	pthread_mutex_lock(&coder->mutex_burnout);
	last_start = coder->last_compile_start;
	pthread_mutex_unlock(&coder->mutex_burnout);
	if (now - last_start >= state->cfg->time_to_burnout)
		return (1);
	return (0);
}

static int	monitor_check_coders(t_state *state, long long now)
{
	int	i;
	int	coder_finished;

	i = 0;
	coder_finished = 0;
	while (i < state->cfg->number_of_coders)
	{
		if (coder_burned_out(state, &state->coders[i], now))
		{
			pthread_mutex_lock(&state->print_lock);
			printf("%lld %d burned out\n", now - state->start_time,
				state->coders[i].id + 1);
			pthread_mutex_unlock(&state->print_lock);
			stop_simulation(state);
			pthread_cond_broadcast(&state->coder_wait_cond);
			return (1);
		}
		if (coder_is_finished(&state->coders[i]))
			coder_finished++;
		i++;
	}
	if (coder_finished == state->cfg->number_of_coders)
		stop_simulation(state);
	return (0);
}

void	*monitor(void *arg)
{
	t_state			*state;
	long long		now;

	state = (t_state *)arg;
	while (simulation_is_running(state))
	{
		now = get_time_in_ms();
		if (monitor_check_coders(state, now))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
