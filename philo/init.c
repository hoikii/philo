/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:45:18 by kanlee            #+#    #+#             */
/*   Updated: 2021/11/27 17:13:02 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdlib.h>
#include "philo.h"

#include <stdio.h>
#include <unistd.h>

static int	init_mutex(t_rule *rule)
{
	int	i;

	i = -1;
	while (++i < rule->num)
		pthread_mutex_init(&rule->forks[i], NULL);
	return SUCCESS;
}

static int	init_philosophers(t_rule *rule)
{
	int	i;

	i = -1;
	while (++i < rule->num)
	{
		rule->philo[i].id = i;
		rule->philo[i].fork1 = i;
		rule->philo[i].fork2 = (i + 1) % rule->num;
		if (i == 0)
		{
			rule->philo[i].fork1 = (i + 1) % rule->num;
			rule->philo[i].fork2 = i;
		}
		rule->philo[i].last_meal = rule->start_time;
		rule->philo[i].rule = rule;
	}
}

int	init(t_rule *rule)
{
	pthread_t	*threads;

	rule->philo = malloc(sizeof(t_philo) * rule->num);
	rule->forks = malloc(sizeof(pthread_mutex_t) * rule->num);
	if (!(rule->philo) || !(rule->forks))
		return (prn_error(MALLOC_FAIL));
	if (init_mutex(rule) == FAIL)
		return  3 ;
	rule->start_time = getcurrent();
	rule->died = 0;
	init_philosophers(rule);

	return (SUCCESS);
}
