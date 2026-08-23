/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edf_queue.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molahrac <molahrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 03:18:49 by molahrac          #+#    #+#             */
/*   Updated: 2026/08/22 11:58:50 by molahrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/codexion.h"

int	init_edf_queue(t_state *state)
{
	return (init_priority_queue(state, QUEUE_EDF));
}

void	push_to_edf_queue(t_coder *coder)
{
	push_to_priority_queue(coder->state->priority_queue, coder);
}

void	pop_from_edf_queue(t_coder *coder)
{
	pop_coder_from_priority_queue(coder->state->priority_queue, coder);
}
