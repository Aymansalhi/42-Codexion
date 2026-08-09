/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 20:15:58 by mirr              #+#    #+#             */
/*   Updated: 2026/08/09 22:59:55 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

int	main(int argc, char **argv)
{
	t_state		state;

	(void)argc;
	state.cfg = ft_parsing_args(argc, argv);
	if (!state.cfg)
		return (EXIT_FAILURE);
	if (create_and_init_coders_and_dongles(&state) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	free(state.cfg);
	return (EXIT_SUCCESS);
}
