/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:45:18 by kanlee            #+#    #+#             */
/*   Updated: 2021/11/28 15:06:11 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdlib.h>
#include "philo.h"

void	destroy_mutex(t_rule *rule)
{
	int	i;

	i = -1;
	while (++i < rule->num)
	{
		pthread_mutex_unlock(&rule->forks[i]);
		pthread_mutex_destroy(&rule->forks[i]);
	}
	pthread_mutex_unlock(&rule->writing);
	pthread_mutex_destroy(&rule->writing);
	return ;
}

static int	init_mutex(t_rule *rule)
{
	int	i;

	if (pthread_mutex_init(&rule->writing, NULL) != 0)
		return (FAIL);
	i = -1;
	while (++i < rule->num)
	{
		if (pthread_mutex_init(&rule->forks[i], NULL) != 0)
			return (FAIL);
	}
	return (SUCCESS);
}

static void	init_philosophers(t_rule *rule)
{
	int	i;

	i = -1;
	while (++i < rule->num)
	{
		rule->philo[i].id = i;
		rule->philo[i].fork1 = i;
		rule->philo[i].fork2 = (i + 1) % rule->num;
/*
		if (i == 0)
		{
			rule->philo[i].fork1 = (i + 1) % rule->num;
			rule->philo[i].fork2 = i;
		}
*/
		rule->philo[i].last_meal = rule->start_time;
		rule->philo[i].rule = rule;
	}
}

int	init(t_rule *rule)
{
	rule->philo = malloc(sizeof(t_philo) * rule->num);
	rule->forks = malloc(sizeof(pthread_mutex_t) * rule->num);
	if (!rule->philo || !rule->forks)
		return (prn_error(MALLOC_FAIL));
	if (init_mutex(rule) == FAIL)
	{
		destroy_mutex(rule);
		return (prn_error(MUTEX_INIT_FAIL));
	}
	rule->start_time = getcurrent();
	rule->died = 0;
	init_philosophers(rule);
	return (SUCCESS);
}
