/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 20:15:58 by mirr              #+#    #+#             */
/*   Updated: 2026/07/16 20:39:59 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "include/codexion.h"



int	main(int argc, char **argv)
{
	t_config	*configs;

	configs = ft_parsing_args(argc, argv);

	free(configs);
}
