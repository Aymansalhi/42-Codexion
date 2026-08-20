/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 10:55:46 by mirr              #+#    #+#             */
/*   Updated: 2026/08/19 01:17:52 by mirr             ###   ########.fr       */
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

void	*monitor(void *arg)
{
	t_state	*state;
	int		i;
	int		coder_finished;
	long long	now;

	state = (t_state *)arg;
	while (state->simulation_running)
	{
		now = get_time_in_ms();
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
				state->simulation_running = 0;
				pthread_cond_broadcast(&state->coder_wait_cond);
				return (NULL);
			}
			if (state->coders[i].is_finished)
				coder_finished++;
			i++;
		}
		usleep(1000);
		if (coder_finished == state->cfg->number_of_coders)
			state->simulation_running = 0;
	}
	return (NULL);
}
