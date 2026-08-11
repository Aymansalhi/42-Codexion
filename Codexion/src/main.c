/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 20:15:58 by mirr              #+#    #+#             */
/*   Updated: 2026/08/11 10:48:06 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"



int	main(int argc, char **argv)
{
	t_state		state;

	(void)argc;
	if (ft_parsing_args(argc, argv, &state))
		return (clean_memory(&state), EXIT_FAILURE);
	if (init_coders_and_dongles(&state) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (start_simulation(&state) == EXIT_FAILURE) //@TODO: 3 implement the create_threads func
		return (EXIT_FAILURE);

	clean_memory(&state);
	printf("All memory cleaned successfully. Exiting program.\n");
	return (EXIT_SUCCESS);
}
