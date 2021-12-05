/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:45:18 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/05 18:14:50 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
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
			sem_post(rule->finished_counter);
	}
}

static void	*monitor_all_ate(void *arg)
{
	t_rule	*rule;
	int		i;

	rule = (t_rule *)arg;
	i = -1;
	while (++i < rule->num)
		sem_wait(rule->finished_counter);
	if (!rule->died)
		prn_action(0, SIM_ENDED, rule);
	sem_post(rule->someone_dead);
	return (NULL);
}

static void	*monitor_each(void *arg)
{
	t_rule		*rule;
	t_philo		*philo;

	philo = (t_philo *)arg;
	rule = philo->rule;
	while (!rule->died)
	{
		if (philo->last_meal + rule->time_to_die < getcurrent())
		{
			prn_action(philo->id, DIED, rule);
			sem_post(rule->someone_dead);
			exit(0);
		}
		usleep(500);
	}
	exit(0);
}

static void	philo(t_philo *philo)
{
	t_rule	*rule;

	rule = philo->rule;
	if (pthread_create(&philo->tid, NULL, monitor_each, (void *)philo) != 0)
	{
		prn_error(THREAD_CREATE_FAIL);
		sem_post(rule->someone_dead);
		exit(0);
	}
	pthread_detach(philo->tid);
	while (1)
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
}

static void	stop_simulation(t_rule *rule)
{
	int	i;

	i = -1;
	while (++i < rule->num)
	{
		if (rule->philo[i].pid != 0)
			kill(rule->philo[i].pid, SIGTERM);
		sem_post(rule->finished_counter);
	}
	destroy_semaphore(rule);
	free(rule->philo);
}

int	simulate(t_rule *rule)
{
	int			i;
	pthread_t	monitor_tid;

	if (pthread_create(&monitor_tid, NULL, monitor_all_ate, rule) != 0)
	{
		prn_error(THREAD_CREATE_FAIL);
		stop_simulation(rule);
		return (FAIL);
	}
	pthread_detach(monitor_tid);
	i = -1;
	while (++i < rule->num)
	{
		rule->philo[i].pid = fork();
		if (rule->philo[i].pid < -1)
		{
			prn_error(PROCESS_FORK_FAIL);
			return (FAIL);
		}
		if (rule->philo[i].pid == 0)
			philo(&rule->philo[i]);
	}
	sem_wait(rule->someone_dead);
	rule->died = 1;
	stop_simulation(rule);
	return (SUCCESS);
}
