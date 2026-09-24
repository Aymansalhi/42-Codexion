/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molahrac <molahrac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 13:34:07 by molahrac          #+#    #+#             */
/*   Updated: 2026/08/23 19:17:22 by molahrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H

//@ -------------------------------------------- MACROS -------------
# define CODEXION_H
# define EXIT_FAILURE 1
# define EXIT_SUCCESS 0
# define SCHEDULER_FIFO "fifo"
# define SCHEDULER_EDF "edf"
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
typedef struct s_state			t_state;
typedef struct s_config			t_config;
typedef struct s_coder			t_coder;
typedef struct s_dongel			t_dongel;

typedef struct s_priority_queue	t_priority_queue;

typedef enum e_queue_mode
{
	QUEUE_FIFO,
	QUEUE_EDF
}	t_queue_mode;

struct s_state
{
	int					simulation_running;
	pthread_t			monitor_thread;
	t_config			*cfg;
	t_coder				*coders;
	t_dongel			*dongels;
	t_priority_queue	*priority_queue;
	pthread_cond_t		coder_wait_cond;
	int					coder_wait_cond_initialized;
	int					dongels_initialized;
	int					coders_initialized;
	long long			start_time;
	pthread_mutex_t		print_lock;
	int					print_lock_initialized;
	pthread_mutex_t		state_lock;
	int					state_lock_initialized;
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
	long long			last_released_ms;
	pthread_mutex_t		lock;
};

struct s_coder
{
	int					id;
	pthread_t			thread;
	t_dongel			*right_dongel;
	t_dongel			*left_dongel;
	t_coder				*next;
	int					queue_index;
	unsigned long long	queue_order;
	t_state				*state;

	pthread_mutex_t		mutex_burnout;
	long long			last_compile_start;
	int					compiles_done;
	int					is_finished;
};

struct s_priority_queue
{
	t_coder				**heap;
	int					size;
	int					capacity;
	unsigned long long	next_order;
	t_queue_mode		mode;
	pthread_mutex_t		lock;
	int					lock_initialized;
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
int			init_fifo_queue(t_state *state);
void		push_to_queue(t_coder *coder);
void		pop_from_queue(t_coder *coder);

int			init_edf_queue(t_state *state);
void		push_to_edf_queue(t_coder *coder);
void		pop_from_edf_queue(t_coder *coder);
t_coder		*peek_priority_queue(t_priority_queue *queue);
int			init_priority_queue(t_state *state, t_queue_mode mode);
void		push_to_priority_queue(t_priority_queue *queue, t_coder *coder);
void		pop_coder_from_priority_queue(t_priority_queue *queue,
				t_coder *coder);
	void		pop_coder_from_priority_queue_locked(t_priority_queue *queue,
				 t_coder *coder);
int			compare_coders(t_priority_queue *queue, t_coder *a, t_coder *b);
void		sift_up(t_priority_queue *queue, int index);
void		swap_heap_nodes(t_priority_queue *queue, int a, int b);
int			compare_by_id(t_coder *a, t_coder *b);
long long	get_deadline(t_coder *coder);

// @-------------------------------------------- PROTOTYPES TIME -----
long long	get_time_in_ms(void);

// @-------------------------------------------- PROTOTYPES MUTEX -----
void		init_coder_fields(
				t_coder *coder,
				int id,
				t_dongel *left,
				t_dongel *right);
void		set_burnout(t_coder *coder);
int			simulation_is_running(t_state *state);
void		stop_simulation(t_state *state);
int			coder_is_finished(t_coder *coder);
void		lock_dongles_mutex_in_order(t_coder *coder);
void		unlock_dongles_mutex_in_order(t_coder *coder);

// @----------------------------------- PROTOTYPES CODER ROUTINE TASKS && utils 
void		take_dongels(t_coder *coder);
void		release_dongles(t_coder *coder);
void		wait_for_scheduler_allows_me_and_get_dongles(t_coder *coder);
int		coder_is_schedulable(t_coder *coder);
int			dongel_ready(t_dongel *dongel, t_state *state);
int			handle_single_coder_case(t_state *state);

#endif