/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 13:34:07 by mirr              #+#    #+#             */
/*   Updated: 2026/08/08 18:52:48 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H

//@ -------------------------------------------- MACROS -------------
# define CODEXION_H
# define EXIT_FAILURE 1
# define EXIT_SUCCESS 0

//@ -------------------------------------------- INCLUDS ------------
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <limits.h>
# include <pthread.h>

//@ -------------------------------------------- STRUCTERS ---------
typedef struct s_config		t_config;
typedef struct s_coder		t_coder;
typedef struct s_dongel		t_dongel;

typedef struct s_queue		t_queue;

struct s_config
{
	int				number_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	char			*scheduler;
};

struct s_coder
{
	int				id;
	pthread_t		thread;
	t_dongel		right_dongel;
	t_dongel		left_dongel;
	t_coder			*next;
};

struct s_dongel
{
	int					id;
	int					available;
	pthread_mutex_t		lock;
};

struct s_queue
{
	t_coder	*head;
	t_coder	*tail;
	int		size;
};


// @-------------------------------------------- PROTOTYPES ---------
t_config	*ft_parsing_args(int argc, char **argv);

// @-------------------------------------------- PROTOTYPES UTILS -----
int			ft_strict_int(const char *str, long *out);
void		ft_set_config_value(t_config *configs, long val, int i);
int			ft_validate_field(long val, int field_id);

#endif