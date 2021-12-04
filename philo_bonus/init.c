/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:45:18 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/04 17:22:44 by kanlee           ###   ########.fr       */
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
	sem_close(rule->finished_counter_sem);
	sem_unlink("/philo_writing");
	sem_unlink("/philo_forks");
	sem_unlink("/philo_finished_counter_sem");
	return ;
}

static int	init_semaphore(t_rule *rule)
{
	sem_unlink("/philo_writing");
	sem_unlink("/philo_forks");
	sem_unlink("/philo_finished_counter_sem");
	rule->writing = sem_open("/philo_writing", O_CREAT, S_IRWXU, 1);
	rule->forks = sem_open("/philo_forks", O_CREAT, S_IRWXU, rule->num);
	rule->finished_counter_sem = sem_open("/philo_finished_counter_sem",
		O_CREAT, S_IRWXU, 1);
	if (!rule->writing || !rule->forks || !rule->finished_counter_sem)
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
		rule->philo[i].fork1 = i;
		rule->philo[i].fork2 = (i + 1) % rule->num;
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
