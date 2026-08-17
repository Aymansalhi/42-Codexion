/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 10:55:46 by mirr              #+#    #+#             */
/*   Updated: 2026/08/16 14:25:58 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

void	*monitor(void *arg)
{
	t_state	*state;
	int		i;
	int		delete_me;

	state = (t_state *)arg;
	while (state->simulation_running)
	{
		i = 0;
		while (i < state->cfg->number_of_coders)
		{
			// printf("monitor: coder %d is working\n", state->coders[i].id);
			i++;
		}
		usleep(100000);
		if (delete_me == 6)
			state->simulation_running = 0;
		delete_me++;
	}
	return (NULL);
}
