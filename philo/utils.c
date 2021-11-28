/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 15:44:57 by kanlee            #+#    #+#             */
/*   Updated: 2021/11/28 19:44:20 by kanlee           ###   ########.fr       */
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

	pthread_mutex_lock(&rule->writing);
	if (rule->died && action != DIED)
	{
		pthread_mutex_unlock(&rule->writing);
		return ;
	}
	current = getcurrent() - rule->start_time;
	if (action == TAKE_FORK)
		printf("%lld %d %s\n", current, id + 1, "has taken a fork");
	else if (action == EATING)
		printf("%lld %d %s\n", current, id + 1, "is eating");
	else if (action == SLEEPING)
		printf("%lld %d %s\n", current, id + 1, "is sleeping");
	else if (action == THINKING)
		printf("%lld %d %s\n", current, id + 1, "is thinking");
	pthread_mutex_unlock(&rule->writing);
	/*
	else if (action == DIED)
		printf("%lld %d %s\n", current, id + 1, "died");
		*/
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
