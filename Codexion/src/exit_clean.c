/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_clean.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 00:26:28 by mirr              #+#    #+#             */
/*   Updated: 2026/08/10 01:40:48 by mirr             ###   ########.fr       */
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
	state->coders = NULL;
	state->dongels = NULL;
	state->cfg = NULL;
}

void	clean_and_print_err(char *err, char *details, int clean, t_state *state)
{
	fprintf(stderr, "ERROR: %s", err);
	if (details)
		fprintf(stderr, " (%s)", details);
	if (clean)
		clean_memory(state);
	fprintf(stderr, "\n");
}
