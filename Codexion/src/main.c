/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 20:15:58 by mirr              #+#    #+#             */
/*   Updated: 2026/08/10 01:59:07 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"



int	main(int argc, char **argv)
{
	t_state		state;

	(void)argc;
	if (ft_parsing_args(argc, argv, &state))
	{
		clean_memory(&state);
		return (EXIT_FAILURE);
	}
	if (create_and_init_coders_and_dongles(&state) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	clean_memory(&state);
	printf("All memory cleaned successfully. Exiting program.\n");
	return (EXIT_SUCCESS);
}
