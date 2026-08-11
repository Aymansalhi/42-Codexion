/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 22:33:41 by mirr              #+#    #+#             */
/*   Updated: 2026/08/11 10:42:30 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

// void	take_dongels()
// {

// }

void	*coder_thread_routine(void *arg)
{
	t_coder		*coder;

	coder = (t_coder *)arg;

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
