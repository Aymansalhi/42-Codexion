/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 00:21:13 by mirr              #+#    #+#             */
/*   Updated: 2026/08/21 10:54:10 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	init_coder_fields(t_coder *coder, int id,
		t_dongel *left, t_dongel *right)
{
	coder->id = id;
	coder->left_dongel = left;
	coder->right_dongel = right;
	coder->is_finished = 0;
	coder->compiles_done = 0;
	coder->last_compile_start = 0;
}

void	lock_dongles_in_order(t_coder *coder)
{
	t_dongel	*first;
	t_dongel	*second;

	if (coder->left_dongel == coder->right_dongel)
	{
		pthread_mutex_lock(&coder->left_dongel->lock);
		return ;
	}
	first = coder->left_dongel;
	second = coder->right_dongel;
	if (first->id > second->id)
	{
		first = coder->right_dongel;
		second = coder->left_dongel;
	}
	pthread_mutex_lock(&first->lock);
	pthread_mutex_lock(&second->lock);
}

void	unlock_dongles_in_order(t_coder *coder)
{
	if (coder->left_dongel == coder->right_dongel)
	{
		pthread_mutex_unlock(&coder->left_dongel->lock);
		return ;
	}
	pthread_mutex_unlock(&coder->left_dongel->lock);
	pthread_mutex_unlock(&coder->right_dongel->lock);
}

void	continue_until_dongles_are_avaliable(t_coder *coder)
{
	int			ready;

	while (coder->state->simulation_running)
	{
		lock_dongles_in_order(coder);
		ready = dongel_ready(coder->left_dongel, coder->state)
			&& dongel_ready(coder->right_dongel, coder->state);
		if (ready)
			break ;
		unlock_dongles_in_order(coder);
		usleep(1000);
	}
}

void	wait_until_scheduler_allows_me(t_coder *coder)
{
	if (strcmp(coder->state->cfg->scheduler, SCHEDULER_EDF) == 0)
		handle_edf_scheduler(coder);
	else
		handle_fifo_scheduler(coder);
	continue_until_dongles_are_avaliable(coder);
}
