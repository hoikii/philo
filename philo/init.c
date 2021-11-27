/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:45:18 by kanlee            #+#    #+#             */
/*   Updated: 2021/11/27 17:50:32 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdlib.h>
#include "philo.h"

#include <stdio.h>

void	*philo(void *args)
{
	t_philo	*philo;
	t_rule	*rule;

	philo = (t_philo *)args;
	rule = philo->rule;
	printf("hi from %d\n", philo->id + 1);
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
		rule->philo[i].rule = rule;
		pthread_create(&threads[i], NULL, &philo, &(rule->philo[i]));
	}
	while (--i >= 0)
		pthread_join(threads[i], NULL);
	return (SUCCESS);
}
