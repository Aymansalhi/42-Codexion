/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edf_queue_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 22:19:30 by mirr              #+#    #+#             */
/*   Updated: 2026/08/21 22:20:50 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	push_when_queue_is_empty(t_edf_queue *edf_queue, t_edf_node *new_node)
{
	edf_queue->head = new_node;
	edf_queue->tail = new_node;
}
