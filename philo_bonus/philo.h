/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 15:28:03 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/05 18:23:12 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <semaphore.h>

# define FAIL 0
# define SUCCESS 1

enum e_errmsgs {
	WRONG_ARGS,
	NO_PHILO,
	NEGATIVE_ARGS,
	INVALID_OPT_ARG,
	MALLOC_FAIL,
	SEM_INIT_FAIL,
	THREAD_CREATE_FAIL,
	PROCESS_FORK_FAIL
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
	pid_t			pid;
	pthread_t		tid;
	int				id;
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
	sem_t			*forks;
	t_philo			*philo;
	long long		start_time;
	int				died;
	sem_t			*writing;
	sem_t			*finished_counter;
	sem_t			*someone_dead;
}	t_rule;

int			init(t_rule *rule);
void		destroy_semaphore(t_rule *rule);
int			simulate(t_rule *rule);
int			prn_error(int err);

void		*monitor_all_ate(void *arg);
void		*monitor_each(void *arg);

int			ft_atoi(const char *str);
long long	getcurrent(void);
void		prn_action(int id, int action, t_rule *rule);
void		precise_sleep(int duration, t_rule *rule);

#endif
