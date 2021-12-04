/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:45:18 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/04 17:18:11 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include "philo.h"

static void	eats_counter(t_philo *philo, t_rule *rule)
{
	if (philo->eat_cnt < rule->must_eats)
	{
		philo->eat_cnt++;
		if (philo->eat_cnt >= rule->must_eats)
		{
			sem_wait(rule->finished_counter_sem);
			rule->finished_counter++;
			sem_post(rule->finished_counter_sem);
		}
	}
}

static void	*philo(void *args)
{
	t_philo	*philo;
	t_rule	*rule;

	philo = (t_philo *)args;
	rule = philo->rule;
	while (!rule->died)
	{
		if (philo->id % 2)
			usleep(1000);
		sem_wait(rule->forks);
		prn_action(philo->id, TAKE_FORK, rule);
		sem_wait(rule->forks);
		prn_action(philo->id, TAKE_FORK, rule);
		philo->last_meal = getcurrent();
		prn_action(philo->id, EATING, rule);
		precise_sleep(rule->time_to_eat, rule);
		eats_counter(philo, rule);
		sem_post(rule->forks);
		sem_post(rule->forks);
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
				prn_action(i, DIED, rule);
				break ;
			}
		}
		if (rule->must_eats > 0 && rule->finished_counter == rule->num)
			prn_action(0, SIM_ENDED, rule);
		usleep(500);
	}
}

static void	stop_simulation(t_rule *rule)
{
	int	i;

	sem_post(rule->forks);
	i = -1;
	while (++i < rule->num)
		pthread_join(rule->philo[i].tid, NULL);
	destroy_semaphore(rule);
	free(rule->philo);
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
