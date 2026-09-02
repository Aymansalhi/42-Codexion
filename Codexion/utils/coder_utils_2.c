/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molahrac <molahrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 16:47:10 by molahrac          #+#    #+#             */
/*   Updated: 2026/08/23 17:11:28 by molahrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

int	simulation_is_running(t_state *state)
{
	int	running;

	pthread_mutex_lock(&state->state_lock);
	running = state->simulation_running;
	pthread_mutex_unlock(&state->state_lock);
	return (running);
}

void	stop_simulation(t_state *state)
{
	pthread_mutex_lock(&state->state_lock);
	state->simulation_running = 0;
	pthread_mutex_unlock(&state->state_lock);
}

int	coder_is_finished(t_coder *coder)
{
	int	finished;

	pthread_mutex_lock(&coder->state->state_lock);
	finished = coder->is_finished;
	pthread_mutex_unlock(&coder->state->state_lock);
	return (finished);
}

int	handle_single_coder_case(t_state *state)
{
	init_coder_fields(&state->coders[0], 0, &state->dongels[0],
		&state->dongels[0]);
	state->coders[0].state = state;
	if (pthread_mutex_init(&state->coders[0].mutex_burnout, NULL) != 0)
		return (clean_and_print_err("Failed to init coder mutex",
				NULL, 1, state), EXIT_FAILURE);
	state->coders_initialized = 1;
	return (EXIT_SUCCESS);
}
