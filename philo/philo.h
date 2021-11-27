/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 15:28:03 by kanlee            #+#    #+#             */
/*   Updated: 2021/11/27 17:41:01 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# define FAIL 0
# define SUCCESS 1

enum e_errmsgs {
	WRONG_ARGS,
	NO_PHILO,
	NEGATIVE_ARGS,
	INVALID_OPT_ARG,
	MALLOC_FAIL
};

enum e_action_msg {
	EATING,
	SLEEPING,
	THINKING,
	DIED
};

typedef struct s_philo {
	int				id;
	struct s_rule	*rule;
}	t_philo;

typedef struct s_rule {
	int			num;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			must_eats;
	int			forks[100];
	t_philo		*philo;
	long long	start_time;
}	t_rule;

int			init(t_rule *rule);
int			prn_error(int err);

int			ft_atoi(const char *str);
long long	getcurrent(void);
void		prn_action(int id, int action, t_rule *rule);

#endif
