/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:45:18 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/05 18:41:38 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <fcntl.h>
#include "philo.h"

void	destroy_semaphore(t_rule *rule)
{
	sem_close(rule->writing);
	sem_close(rule->forks);
	sem_close(rule->finished_counter);
	sem_close(rule->someone_dead);
	sem_unlink("/philo_writing");
	sem_unlink("/philo_forks");
	sem_unlink("/philo_finished_counter");
	sem_unlink("/philo_someone_dead");
	return ;
}

static int	init_semaphore(t_rule *rule)
{
	sem_unlink("/philo_writing");
	sem_unlink("/philo_forks");
	sem_unlink("/philo_finished_counter");
	sem_unlink("/philo_someone_dead");
	rule->writing = sem_open("/philo_writing", O_CREAT, S_IRWXU, 1);
	rule->forks = sem_open("/philo_forks", O_CREAT, S_IRWXU, rule->num);
	rule->finished_counter = sem_open("/philo_finished_counter",
			O_CREAT, S_IRWXU, 0);
	rule->someone_dead = sem_open("/philo_someone_dead", O_CREAT, S_IRWXU, 0);
	if (!rule->writing || !rule->forks || !rule->finished_counter
		|| !rule->someone_dead)
		return (FAIL);
	return (SUCCESS);
}

static void	init_philosophers(t_rule *rule)
{
	int	i;

	i = -1;
	while (++i < rule->num)
	{
		rule->philo[i].id = i;
		rule->philo[i].last_meal = rule->start_time;
		rule->philo[i].eat_cnt = 0;
		rule->philo[i].rule = rule;
	}
}

int	init(t_rule *rule)
{
	rule->philo = malloc(sizeof(t_philo) * rule->num);
	if (!rule->philo)
		return (prn_error(MALLOC_FAIL));
	if (init_semaphore(rule) == FAIL)
	{
		destroy_semaphore(rule);
		return (prn_error(SEM_INIT_FAIL));
	}
	rule->start_time = getcurrent();
	rule->died = 0;
	init_philosophers(rule);
	return (SUCCESS);
}
