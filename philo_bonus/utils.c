/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 15:44:57 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/04 15:46:30 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include "philo.h"

static int	is_space(char ch)
{
	return (ch == '\t' || ch == '\n' || ch == ' '
		|| ch == '\v' || ch == '\r' || ch == '\f');
}

int	ft_atoi(const char *nptr)
{
	int			sign;
	long long	result;

	while (is_space(*nptr))
		nptr++;
	sign = 1;
	if (*nptr == '-')
		sign = -1;
	if (*nptr == '-' || *nptr == '+')
		nptr++;
	result = 0;
	while ('0' <= *nptr && *nptr <= '9')
	{
		result = result * 10 + (*nptr++ - '0');
		if (sign * result > INT_MAX)
			return (-1);
		else if (sign * result < INT_MIN)
			return (0);
	}
	return (sign * result);
}

long long	getcurrent(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	prn_action(int id, int action, t_rule *rule)
{
	long long	current;

	sem_wait(rule->writing);
	current = getcurrent() - rule->start_time;
	if (rule->died)
		;
	else if (action == TAKE_FORK)
		printf("%lld %d has taken a fork\n", current, id + 1);
	else if (action == EATING)
		printf("%lld %d is eating\n", current, id + 1);
	else if (action == SLEEPING)
		printf("%lld %d is sleeping\n", current, id + 1);
	else if (action == THINKING)
		printf("%lld %d is thinking\n", current, id + 1);
	else if (action == DIED || action == SIM_ENDED)
	{
		rule->died = 1;
		if (action == DIED)
			printf("%lld %d is died\n", current, id + 1);
		else
			printf("%lld All philosophers ate at least %d times.\n",
				current, rule->must_eats);
	}
	sem_post(rule->writing);
	return ;
}

void	precise_sleep(int duration, t_rule *rule)
{
	long long	start;

	start = getcurrent();
	while (!rule->died && getcurrent() - start < duration)
		usleep(100);
	return ;
}
