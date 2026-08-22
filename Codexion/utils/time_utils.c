/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 12:35:04 by mirr              #+#    #+#             */
/*   Updated: 2026/08/21 10:24:40 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../include/codexion.h"

/**
 * @brief Get the current time in milliseconds since the epoch.
 * 
 * @return long long The current time in milliseconds.
 */

long long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	set_burnout(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex_burnout);
	coder->last_compile_start = get_time_in_ms();
	pthread_mutex_unlock(&coder->mutex_burnout);
}