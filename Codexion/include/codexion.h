/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 13:34:07 by mirr              #+#    #+#             */
/*   Updated: 2026/07/16 20:34:53 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

//@ -------------------------------------------- INCLUDS ------------
# include <stdlib.h>


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



#endif