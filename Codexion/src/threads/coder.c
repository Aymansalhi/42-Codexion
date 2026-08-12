/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 22:33:41 by mirr              #+#    #+#             */
/*   Updated: 2026/08/11 21:51:28 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

void	request_compile(t_coder *coder)
{
	push_to_queue(coder);
}

void	wait_until_scheduler_allows_me(t_coder *coder)
{
	t_queue	*queue;

	queue = coder->state->queue;
	pthread_mutex_lock(&queue->lock);
	while (queue->head != coder)
	{
		pthread_cond_wait(
			&coder->state->coder_wait_cond, &queue->lock);
	}
	printf("Coder %d is allowed to compile now.\n", coder->id);
	pop_from_queue(coder);
	pthread_cond_brodcast(&coder->state->coder_wait_cond);
	pthread_mutex_unlock(&queue->lock);
}


void	*coder_thread_routine(void *arg)
{
	t_coder		*coder;
	t_state		*state;

	coder = (t_coder *)arg;

	request_compile(coder);
	wait_until_scheduler_allows_me(coder);



	// coder's life

	// take dongels
	// compile
	// wait
	// debug
	// refactor
	// etc.
	// usleep(5000000); //* sleep for 5 seconds to simulate work
	printf("Coder %d is starting its routine.\n", coder->id);
	usleep(3000000); //* sleep for 5 seconds to simulate work


	return (NULL);
}

// @TODO: 1  i think befor start coding this i v counted a probleme how to know that im the coder to take the dongels so what ill do i ll start implemeint the fifo algo to start good
// @TODO: 2 go sreach how to know that im the self coder that scheduler choose me is he pass the acctual info struct as parameter ??



// @TODO: 3 i need to test all of this shit now what i did then continue in line 45 and 46 two func