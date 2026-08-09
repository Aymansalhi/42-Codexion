/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 20:15:58 by mirr              #+#    #+#             */
/*   Updated: 2026/08/09 23:39:15 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	clean_memory(t_state *state)
{

	if (state->coders)
		free(state->coders);
	if (state->dongels)
		free(state->dongels);
	if (state->cfg)
		free(state->cfg);
}

int	main(int argc, char **argv)
{
	t_state		state;

	(void)argc;
	state.cfg = ft_parsing_args(argc, argv);
	if (!state.cfg)
		return (EXIT_FAILURE);
	// if (create_and_init_coders_and_dongles(&state) == EXIT_FAILURE)
	// 	return (EXIT_FAILURE);
	clean_memory(&state);
	return (EXIT_SUCCESS);
}
