/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 22:33:41 by mirr              #+#    #+#             */
/*   Updated: 2026/08/13 12:24:08 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

void	request_compile(t_coder *coder)
{
	printf("Coder %d is pushed to the queue waiting to compile.\n", coder->id);
	push_to_queue(coder);
}

void	wait_until_scheduler_allows_me(t_coder *coder)
{
	t_queue	*queue;

	queue = coder->state->queue;
	pthread_mutex_lock(&queue->lock);
	while (queue->head != coder)
	{
		printf("Coder %d is waiting for its turn to compile.\n", coder->id);
		pthread_cond_wait(
			&coder->state->coder_wait_cond, &queue->lock);
	}
	printf("Coder %d is allowed to compile now.\n", coder->id);
	pop_from_queue(coder); //@ TODO: move this to after coder finishes compiling, but for now it's here to avoid deadlock
	pthread_cond_broadcast(&coder->state->coder_wait_cond);
	pthread_mutex_unlock(&queue->lock);
}

// @TODO 2: implement Dongle cooldown is mandatory also
void	take_dongels(t_coder *coder)
{
	coder->left_dongel->available = 0;
	coder->right_dongel->available = 0;
	pri
}

void	*coder_thread_routine(void *arg)
{
	t_coder		*coder;

	coder = (t_coder *)arg;

	request_compile(coder);
	wait_until_scheduler_allows_me(coder);



	// coder's life

	take_dongels(coder);
	// compile
	// wait
	// debug
	// refactor
	// etc.
	// usleep(5000000); //* sleep for 5 seconds to simulate work
	printf("Coder %d is starting its routine.\n", coder->id);
	usleep(5000000); //* sleep for 5 seconds to simulate work


	return (NULL);
}

// @TODO 1: kepp continueing implementing the coder_thread_routine u can check the check list to continue
// @TODO 1-0: so manly i need to implement the dongd etc. i will do it in the next commitsel taking and releasing and the compiling and debugging and refactoring an