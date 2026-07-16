/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 13:34:07 by mirr              #+#    #+#             */
/*   Updated: 2026/07/16 20:14:03 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

/*  TODO1: what datastruct or algo im gonna use for this project for shudeler?*/
/* back yp to fisrt of photo chat*/

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