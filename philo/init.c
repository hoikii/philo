/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:45:18 by kanlee            #+#    #+#             */
/*   Updated: 2021/11/27 18:52:21 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdlib.h>
#include "philo.h"

#include <stdio.h>
#include <unistd.h>

void	philo_eats(t_philo philo)
{
	//take fork
	while (philo->last_meal + philo->rule->time_to_die < getcurrent())

void	*philo(void *args)
{
	t_philo	*philo;
	t_rule	*rule;

	philo = (t_philo *)args;
	rule = philo->rule;
	while (1)
	{
		prn_action(philo->id, TAKE_FORK, rule);
		prn_action(philo->id, EATING, rule);
		precise_sleep(rule->time_to_eat);
		prn_action(philo->id, SLEEPING, rule);
		precise_sleep(rule->time_to_sleep);
		prn_action(philo->id, THINKING, rule);
	}
	pthread_exit(0);
}

int	init(t_rule *rule)
{
	pthread_t	*threads;
	int			i;

	rule->philo = malloc(sizeof(t_philo) * rule->num);
	threads = malloc(sizeof(pthread_t) * rule->num);
	if (!(rule->philo) || !(threads))
		return (prn_error(MALLOC_FAIL));
	rule->start_time = getcurrent();
	i = -1;
	while (++i < rule->num)
	{
		rule->philo[i].id = i;
		rule->philo[i].fork1 = i;
		rule->philo[i].fork2 = (i + 1) % rule->num;
		rule->philo[i].last_meal = rule->start_time;
		rule->philo[i].rule = rule;
		pthread_create(&threads[i], NULL, &philo, &(rule->philo[i]));
	}
	while (--i >= 0)
		pthread_join(threads[i], NULL);
	free(rule->philo);
	free(threads);
	return (SUCCESS);
}
