/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 20:15:58 by mirr              #+#    #+#             */
/*   Updated: 2026/07/23 13:34:01 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/codexion.h"


void	print_config(t_config *configs)
{
	printf("number_of_coders: %d\n", configs->number_of_coders);
	printf("time_to_burnout: %d\n", configs->time_to_burnout);
	printf("time_to_compile: %d\n", configs->time_to_compile);
	printf("time_to_debug: %d\n", configs->time_to_debug);
	printf("time_to_refactor: %d\n", configs->time_to_refactor);
	printf(
		"number_of_compiles_required: %d\n",
		configs->number_of_compiles_required
		);
	printf("dongle_cooldown: %d\n", configs->dongle_cooldown);
	printf("scheduler: %s\n", configs->scheduler);
}

int	main(int argc, char **argv)
{
	t_config	*configs;

	(void)argc;
	configs = ft_parsing_args(argc, argv);
	if (!configs)
		return (EXIT_FAILURE);
	print_config(configs);

	free(configs);
	return (EXIT_SUCCESS);
}
