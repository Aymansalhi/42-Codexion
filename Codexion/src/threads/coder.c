/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 22:33:41 by mirr              #+#    #+#             */
/*   Updated: 2026/08/14 01:06:22 by mirr             ###   ########.fr       */
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
	pop_from_queue(coder); //@ TODO: move this to after coder releases dongels, but for now it's here to avoid deadlock
	pthread_cond_broadcast(&coder->state->coder_wait_cond);
	pthread_mutex_unlock(&queue->lock);
}

// @TODO 2: implement Dongle cooldown is mandatory also
void	take_dongels(t_coder *coder)
{
	long long	time_passed;

	if (coder->left_dongel->available == 0
		|| coder->right_dongel->available == 0)
	{
		printf("WTF no supose to happen, left_dongel:%d right_dongel:%d\n",
			coder->left_dongel->available, coder->right_dongel->available);
	}
	time_passed = get_time_in_ms() - coder->state->start_time;
	coder->left_dongel->available = 0;
	printf("%lld %d has taken a dongle\n", time_passed, coder->id);
	coder->right_dongel->available = 0;
	printf("%lld %d has taken a dongle\n", time_passed, coder->id);
}

void	compile(t_coder *coder)
{
	set_burnout(coder);
	printf("%lld Coder %d is compiling for %d ms.\n",
		get_time_in_ms() - coder->state->start_time, coder->id,
		coder->state->cfg->time_to_compile);
	usleep(coder->state->cfg->time_to_compile * 1000);
	coder->compiles_done++;
	if (coder->compiles_done >= coder->state->cfg->number_of_compiles_required)
	{
		printf("%lld Coder %d has finished compiling the required number of times.\n",
			get_time_in_ms() - coder->state->start_time, coder->id);
		coder->is_finished = 1;
	}
	release_dongles(coder); //@ implement this function in the next commit
	// @TODO 3: implement the compile time and the burnout time check and add 1 to compiles_done
}

void	*coder_thread_routine(void *arg)
{
	t_coder		*coder;

	coder = (t_coder *)arg;

	request_compile(coder);
	wait_until_scheduler_allows_me(coder);

	// coder's life
	take_dongels(coder);
	compile(coder);
	// wait
	// release dongels
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