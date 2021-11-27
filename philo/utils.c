/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 15:44:57 by kanlee            #+#    #+#             */
/*   Updated: 2021/11/27 17:54:56 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <sys/time.h>
#include <stdio.h>
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

	current = getcurrent() - rule->start_time;
	if (action == SLEEPING)
		printf("%lld %d %s\n", current, id + 1, "is sleeping");
	return ;
}
