/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fifo_queue.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molahrac <molahrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 19:16:54 by molahrac          #+#    #+#             */
/*   Updated: 2026/08/22 13:32:44 by molahrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

int	init_fifo_queue(t_state *state)
{
	return (init_priority_queue(state, QUEUE_FIFO));
}

void	push_to_queue(t_coder *coder)
{
	push_to_priority_queue(coder->state->priority_queue, coder);
}

void	pop_from_queue(t_coder *coder)
{
	pop_coder_from_priority_queue(coder->state->priority_queue, coder);
}
