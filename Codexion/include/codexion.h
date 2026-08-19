/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mirr <mirr@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 13:34:07 by mirr              #+#    #+#             */
/*   Updated: 2026/08/19 01:41:05 by mirr             ###   ########.fr       */
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
# define THREAD_CREATION_ERROR "Failed to create thread."
# define THREAD_JOIN_ERROR "Failed to join thread."
# define MUTEX_QUEUE_ERROR "Failed to initialize mutex for queue."
# define COND_QUEUE_ERROR "Failed to initialize condition variable for queue."

//@ -------------------------------------------- INCLUDS ------------
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <limits.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>


//@ -------------------------------------------- STRUCTERS ---------
typedef struct s_state		t_state;
typedef struct s_config		t_config;
typedef struct s_coder		t_coder;
typedef struct s_dongel		t_dongel;

typedef struct s_queue		t_queue;
typedef struct s_tmp		t_tmp;

struct s_state
{
	int					simulation_running;
	pthread_t			monitor_thread;
	t_config			*cfg;
	t_coder				*coders;
	t_dongel			*dongels;
	t_queue				*queue;
	pthread_cond_t		coder_wait_cond;
	int					dongels_initialized;
	long long			start_time;
};

struct s_config
{
	int				number_of_coders; //? DONE
	int				time_to_burnout; //? DONE
	int				time_to_compile; //? DONE
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
	long long			last_released_ms;
	pthread_mutex_t		lock;
};

struct s_coder
{
	int				id;
	pthread_t		thread;
	t_dongel		*right_dongel;
	t_dongel		*left_dongel;
	t_coder			*next;
	t_state			*state;

	pthread_mutex_t	mutex_burnout;
	long long		last_compile_start;
	int				compiles_done;
	int				is_finished;
};

struct s_queue
{
	t_coder			*head;
	t_coder			*tail;
	int				size;
	pthread_mutex_t	lock;
};

struct s_tmp
{
	t_coder	*coder;
	t_state	*state;
};


// @-------------------------------------------- PROTOTYPES ---------
int			ft_parsing_args(int argc, char **argv, t_state *state);
int			init_and_setup_all(t_state *state);

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

// @-------------------------------------------- PROTOTYPES THREADS UTILS -----
int			start_simulation(t_state *state);
int			create_threads(t_state *state);
void		join_threads(t_state *state);

// @-------------------------------------------- PROTOTYPES THREADS -----
void		*coder_thread_routine(void *arg);
void		*monitor(void *arg);

// @-------------------------------------------- PROTOTYPES QUEUE -----
void		push_to_queue(t_coder *coder);
void		pop_from_queue(t_coder *coder);

// @-------------------------------------------- PROTOTYPES TIME -----
long long	get_time_in_ms(void);

// @-------------------------------------------- PROTOTYPES MUTEX -----
void		init_coder_fields(
				t_coder *coder,
				int id,
				t_dongel *left,
				t_dongel *right);
void		set_burnout(t_coder *coder);
void		lock_dongles_in_order(t_coder *coder);
void		unlock_dongles_in_order(t_coder *coder);

// @----------------------------------- PROTOTYPES CODER ROUTINE TASKS -----
void		take_dongels(t_coder *coder);
void		release_dongles(t_coder *coder);
void		wait_until_scheduler_allows_me(t_coder *coder);


#endif