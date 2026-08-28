/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molahrac <molahrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 20:15:58 by molahrac          #+#    #+#             */
/*   Updated: 2026/08/22 02:06:18 by molahrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

int	main(int argc, char **argv)
{
	t_state		state;

	(void)argc;
	memset(&state, 0, sizeof(t_state));
	state.simulation_running = 1;
	if (pthread_mutex_init(&state.state_lock, NULL) != 0)
		return (EXIT_FAILURE);
	state.state_lock_initialized = 1;
	if (ft_parsing_args(argc, argv, &state))
		return (clean_memory(&state), EXIT_FAILURE);
	if (init_and_setup_all(&state) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (start_simulation(&state) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	stop_simulation(&state);
	clean_memory(&state);
	return (EXIT_SUCCESS);
}
