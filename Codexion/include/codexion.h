/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 13:34:07 by mirr              #+#    #+#             */
/*   Updated: 2026/08/10 01:24:29 by mirr             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H

//@ -------------------------------------------- MACROS -------------
# define CODEXION_H
# define EXIT_FAILURE 1
# define EXIT_SUCCESS 0
# define MALLOC_ERROR "Memory allocation failed."
# define INVALID_ARG "Invalid argument."
# define INVALD_ARGS "Invalid number of arguments."
# define INVALID_SCHEDULER "Invalid scheduler. Must be 'fifo' or 'edf'."
# define MUTEX_DONGEL_ERROR "Failed to initialize mutex for dongel."

//@ -------------------------------------------- INCLUDS ------------
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <limits.h>
# include <pthread.h>

//@ -------------------------------------------- STRUCTERS ---------
typedef struct s_state		t_state;
typedef struct s_config		t_config;
typedef struct s_coder		t_coder;
typedef struct s_dongel		t_dongel;

typedef struct s_queue		t_queue;

struct s_state
{
	t_config			*cfg;
	t_coder				*coders;
	t_dongel			*dongels;
	t_queue				*queue;
	pthread_mutex_t		state_lock;
};

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

struct s_dongel
{
	int					id;
	int					available;
	pthread_mutex_t		lock;
};

struct s_coder
{
	int				id;
	pthread_t		thread;
	t_dongel		right_dongel;
	t_dongel		left_dongel;
	t_coder			*next;
};

struct s_queue
{
	t_coder	*head;
	t_coder	*tail;
	int		size;
};


// @-------------------------------------------- PROTOTYPES ---------
int			ft_parsing_args(int argc, char **argv, t_state *state);
int			create_and_init_coders_and_dongles(t_state *state);

// @-------------------------------------------- PROTOTYPES UTILS -----
int			ft_strict_int(const char *str, long *out);
void		ft_set_config_value(t_config *configs, long val, int i);
int			ft_validate_field(long val, int field_id);

// @-------------------------------------------- PROTOTYPES EXIT_CLEAN -----
void		clean_memory(t_state *state);
void		clean_and_print_err(
				char *err,
				char *details,
				int clean,
				t_state *state);

#endif