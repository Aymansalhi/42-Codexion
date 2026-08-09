/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 19:03:48 by mirr              #+#    #+#             */
/*   Updated: 2026/08/09 00:56:54 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

int	create_coders_and_dongles(t_state *state)
{
	state->coders = malloc(sizeof(t_coder) * state->cfg->number_of_coders);
	if (!state->coders)
		return (EXIT_FAILURE);
	state->dongels = malloc(sizeof(t_dongel) * state->cfg->number_of_coders);
	if (!state->dongels)
		return (free(state->coders), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	create_and_init_coders_and_dongles(t_state *state)
{
	if (create_coders_and_dongles(state) == EXIT_FAILURE)
		return (EXIT_FAILURE);

	init_coders(state);
	init_dongles(state);
	return (EXIT_SUCCESS);
}
