/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:45:18 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/05 18:31:04 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include "philo.h"

void	*monitor_all_ate(void *arg)
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

void	*monitor_each(void *arg)
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
