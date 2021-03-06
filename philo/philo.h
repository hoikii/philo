/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 15:28:03 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/02 08:53:37 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>

# define FAIL 0
# define SUCCESS 1

enum e_errmsgs {
	WRONG_ARGS,
	NO_PHILO,
	NEGATIVE_ARGS,
	INVALID_OPT_ARG,
	MALLOC_FAIL,
	MUTEX_INIT_FAIL,
	THREAD_CREATE_FAIL
};

enum e_action_msg {
	TAKE_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIED,
	SIM_ENDED
};

typedef struct s_philo {
	pthread_t		tid;
	int				id;
	int				fork1;
	int				fork2;
	long long		last_meal;
	int				eat_cnt;
	struct s_rule	*rule;
}	t_philo;

typedef struct s_rule {
	int				num;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eats;
	pthread_mutex_t	*forks;
	t_philo			*philo;
	long long		start_time;
	int				died;
	pthread_mutex_t	writing;
	int				finished_counter;
	pthread_mutex_t	finished_counter_mutex;
}	t_rule;

int			init(t_rule *rule);
void		destroy_mutex(t_rule *rule);
int			simulate(t_rule *rule);
int			prn_error(int err);

int			ft_atoi(const char *str);
long long	getcurrent(void);
void		prn_action(int id, int action, t_rule *rule);
void		precise_sleep(int duration, t_rule *rule);

#endif
