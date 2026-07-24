/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 11:52:17 by mirr              #+#    #+#             */
/*   Updated: 2026/07/23 12:10:06 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/codexion.h"

/* Converts the initial portion of the string 'str' to an integer */
void	skip_spacese(const char *str, size_t *i)
{
	while ((str[*i] >= 9 && str[*i] <= 13) || str[*i] == 32)
	{
		(*i)++;
	}
}

void	check_sign(const char *str, size_t *i, int *xsign)
{
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-')
			*xsign = -1;
		(*i)++;
	}
}

int	ft_atoi(const char *str)
{
	size_t		i;
	long long	rus;
	int			xsign;

	i = 0;
	rus = 0;
	xsign = 1;
	skip_spacese(str, &i);
	check_sign(str, &i, &xsign);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (rus * 10 + (str[i] - '0') < rus)
		{
			if (xsign == 1)
				return (-1);
			if (xsign == -1)
				return (0);
		}
		rus *= 10;
		rus += (str[i] - '0');
		i++;
	}
	rus = rus * xsign;
	return (rus);
}
