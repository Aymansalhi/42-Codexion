/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 13:34:07 by mirr              #+#    #+#             */
/*   Updated: 2026/07/22 16:34:50 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H

//@ -------------------------------------------- MACROS -------------
# define CODEXION_H
# define EXIT_FAILURE 1
# define EXIT_SUCCESS 0

//@ -------------------------------------------- INCLUDS ------------
# include <stdlib.h>
# include <string.h>
# include <stdio.h> // delete



//@ -------------------------------------------- STRUCTERS ---------
typedef struct s_config
{
	int		number_of_coders;

	int		time_to_burnout;

	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;

	int		number_of_compiles_required;

	int		dongle_cooldown;

	char	*scheduler;
}			t_config;

// typedef struct 


// @-------------------------------------------- PROTOTYPES ---------
t_config	*ft_parsing_args(int argc, char **argv);

#endif