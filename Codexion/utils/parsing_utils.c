/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 12:10:39 by mirr              #+#    #+#             */
/*   Updated: 2026/08/08 15:36:49 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

int	ft_validate_field(long val, int field_id)
{
	if (field_id == 6 || field_id == 7)
		return (val >= 0);
	return (val > 0);
}

int	ft_accumulate(const char *str, size_t i, unsigned long long *rus)
{
	unsigned long long	prev;

	*rus = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (EXIT_FAILURE);
		prev = *rus;
		*rus = *rus * 10 + (str[i] - '0');
		if (*rus < prev)
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	ft_strict_int(const char *str, long *out)
{
	unsigned long long	rus;
	size_t				i;

	i = 0;
	if (!str || !str[0])
		return (EXIT_FAILURE);
	if (str[i] == '+')
		i++;
	if (!str[i])
		return (EXIT_FAILURE);
	if (ft_accumulate(str, i, &rus))
		return (EXIT_FAILURE);
	if (rus > (unsigned long long)INT_MAX)
		return (EXIT_FAILURE);
	*out = (long)rus;
	return (EXIT_SUCCESS);
}

void	ft_set_config_value(t_config *configs, long val, int i)
{
	if (i == 1)
		configs->number_of_coders = (int)val;
	else if (i == 2)
		configs->time_to_burnout = (int)val;
	else if (i == 3)
		configs->time_to_compile = (int)val;
	else if (i == 4)
		configs->time_to_debug = (int)val;
	else if (i == 5)
		configs->time_to_refactor = (int)val;
	else if (i == 6)
		configs->number_of_compiles_required = (int)val;
	else if (i == 7)
		configs->dongle_cooldown = (int)val;
}
