/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:45:18 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/01 08:02:59 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdlib.h>
#include "philo.h"

#include <stdio.h>
#include <unistd.h>

static void	eats_counter(t_philo *philo, t_rule *rule)
{
	if (philo->eat_cnt < rule->must_eats)
	{
		philo->eat_cnt++;
		if (philo->eat_cnt >= rule->must_eats)
		{
			pthread_mutex_lock(&rule->finished_counter_mutex);
			rule->finished_counter++;
			pthread_mutex_unlock(&rule->finished_counter_mutex);
		}
	}
}

void	*philo(void *args)
{
	t_philo	*philo;
	t_rule	*rule;

	philo = (t_philo *)args;
	rule = philo->rule;
	while (!rule->died)
	{
		if (philo->id % 2)
			usleep(1000);
		pthread_mutex_lock(&rule->forks[philo->fork1]);
		prn_action(philo->id, TAKE_FORK, rule);
		pthread_mutex_lock(&rule->forks[philo->fork2]);
		prn_action(philo->id, TAKE_FORK, rule);
		philo->last_meal = getcurrent();
		prn_action(philo->id, EATING, rule);
		precise_sleep(rule->time_to_eat, rule);
		eats_counter(philo, rule);
		pthread_mutex_unlock(&(rule->forks[philo->fork1]));
		pthread_mutex_unlock(&(rule->forks[philo->fork2]));
		prn_action(philo->id, SLEEPING, rule);
		precise_sleep(rule->time_to_sleep, rule);
		prn_action(philo->id, THINKING, rule);
	}
	return (NULL);
}

static void	monitor(t_rule *rule)
{
	int			i;
	t_philo		philo;

	while (!rule->died)
	{
		i = -1;
		while (++i < rule->num)
		{
			philo = rule->philo[i];
			if (philo.last_meal + rule->time_to_die < getcurrent())
			{
				pthread_mutex_lock(&rule->writing);
				printf("%lld %d %s\n", getcurrent() - rule->start_time, i + 1, "died");
				rule->died = 1;
				pthread_mutex_unlock(&rule->writing);
				break ;
			}
			if (rule->must_eats > 0 && rule->finished_counter == rule->num)
			{
				pthread_mutex_lock(&rule->writing);
				printf("%lld All philosophers ate at least %d.\n", getcurrent() - rule->start_time, rule->must_eats);
				rule->died = 1;
				pthread_mutex_unlock(&rule->writing);
				break ;
			}
		}
		usleep(500);
	}
}

static void	stop_simulation(t_rule *rule)
{
	int	i;

	destroy_mutex(rule);
	i = -1;
	while (++i < rule->num)
		pthread_join(rule->philo[i].tid, NULL);
	free(rule->philo);
	free(rule->forks);
}

int	simulate(t_rule *rule)
{
	int	i;

	i = -1;
	while (++i < rule->num)
	{
		if (pthread_create(&rule->philo[i].tid, NULL, &philo, &(rule->philo[i]))
			!= 0)
		{
			rule->died = 1;
			prn_error(THREAD_CREATE_FAIL);
			stop_simulation(rule);
			return (FAIL);
		}
	}
	monitor(rule);
	stop_simulation(rule);
	return (SUCCESS);
}
