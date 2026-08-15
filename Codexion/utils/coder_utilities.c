/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utilities.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 00:21:13 by mirr              #+#    #+#             */
/*   Updated: 2026/08/14 00:27:26 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	set_burnout(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex_burnout);
	coder->time_bournout = get_time_ms();
	pthread_mutex_unlock(&coder->mutex_burnout);
}
