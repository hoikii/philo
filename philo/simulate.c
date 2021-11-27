/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:45:18 by kanlee            #+#    #+#             */
/*   Updated: 2021/11/27 17:08:38 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdlib.h>
#include "philo.h"

#include <stdio.h>
#include <unistd.h>

void	*philo(void *args)
{
	t_philo	*philo;
	t_rule	*rule;

	philo = (t_philo *)args;
	rule = philo->rule;
	while (!rule->died)
	{
		pthread_mutex_lock(&rule->forks[philo->fork1]);
		prn_action(philo->id, TAKE_FORK, rule);
		pthread_mutex_lock(&rule->forks[philo->fork2]);
		prn_action(philo->id, TAKE_FORK, rule);
		philo->last_meal = getcurrent();
		prn_action(philo->id, EATING, rule);
		precise_sleep(rule->time_to_eat, rule);
		pthread_mutex_unlock(&(rule->forks[philo->fork1]));
		pthread_mutex_unlock(&(rule->forks[philo->fork2]));
		prn_action(philo->id, SLEEPING, rule);
		precise_sleep(rule->time_to_sleep, rule);
		prn_action(philo->id, THINKING, rule);
	}
}

static void	monitor(t_rule *rule)
{
	int		i;
	t_philo	philo;

	while (!rule->died)
	{
		i = -1;
		while (++i < rule->num)
		{
			philo = rule->philo[i];
			if (philo.last_meal + rule->time_to_die < getcurrent())
			{
				prn_action(i, DIED, rule);
				rule->died = 1;
				break ;
			}
		}
		usleep(50);
	}
}

static void	stop_simulation(t_rule *rule)
{
	int	i;

	i = -1;
	while (++i < rule->num)
	{
		pthread_mutex_unlock(&rule->forks[i]);
		pthread_mutex_destroy(&rule->forks[i]);
		pthread_join(rule->philo[i].tid, NULL);
	}
	free(rule->philo);
	free(rule->forks);
}

int	simulate(t_rule *rule)
{
	int	i;

	i = -1;
	while (++i < rule->num)
	{
		pthread_create(&rule->philo[i].tid, NULL, &philo, &(rule->philo[i]));
	}
	monitor(rule);
	stop_simulation(rule);

	return 1;
}
