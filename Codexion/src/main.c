/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 20:15:58 by mirr              #+#    #+#             */
/*   Updated: 2026/08/18 13:26:22 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"



int	main(int argc, char **argv)
{
	t_state		state;

	(void)argc;
	memset(&state, 0, sizeof(t_state));
	state.simulation_running = 1;
	if (ft_parsing_args(argc, argv, &state))
		return (clean_memory(&state), EXIT_FAILURE);
	if (init_and_setup_all(&state) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (start_simulation(&state) == EXIT_FAILURE)
		return (EXIT_FAILURE);

	state.simulation_running = 0;
	clean_memory(&state);
	printf("All memory cleaned successfully. Exiting program.\n");
	return (EXIT_SUCCESS);
}
