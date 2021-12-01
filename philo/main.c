/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 15:03:58 by kanlee            #+#    #+#             */
/*   Updated: 2021/12/01 08:19:25 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "philo.h"

int	prn_error(int err)
{
	if (err == WRONG_ARGS)
		printf("philo n ms ms ms [n]\n");
	else if (err == NO_PHILO)
		printf("There should be at least one philosopher\n");
	else if (err == NEGATIVE_ARGS)
		printf("Incorrent time arguments\n");
	else if (err == INVALID_OPT_ARG)
		printf("Invalid optional argument\n");
	else if (err == MALLOC_FAIL)
		printf("Error while memory allocation\n");
	else if (err == MUTEX_INIT_FAIL)
		printf("Error while initializing mutex\n");
	else if (err == THREAD_CREATE_FAIL)
		printf("Error while creating thread\n");
	return (FAIL);
}

int	chk_args(int ac, char **av, t_rule *rule)
{
	if (ac != 5 && ac != 6)
		return (prn_error(WRONG_ARGS));
	rule->num = ft_atoi(av[1]);
	rule->time_to_die = ft_atoi(av[2]);
	rule->time_to_eat = ft_atoi(av[3]);
	rule->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
	{
		rule->must_eats = ft_atoi(av[5]);
		if (rule->must_eats <= 0)
			return (prn_error(INVALID_OPT_ARG));
	}
	else
		rule->must_eats = -1;
	if (rule->num <= 0)
		return (prn_error(NO_PHILO));
	if (rule->time_to_die <= 0 || rule->time_to_eat <= 0
		|| rule->time_to_sleep <= 0)
		return (prn_error(NEGATIVE_ARGS));
	return (SUCCESS);
}

int	main(int ac, char **av)
{
	t_rule	rule;

	memset(&rule, 0, sizeof(t_rule));
	if (chk_args(ac, av, &rule) == FAIL)
		return (1);
	if (init(&rule) == FAIL)
	{
		if (rule.philo)
			free(rule.philo);
		if (rule.forks)
			free(rule.forks);
		return (1);
	}
	if (simulate(&rule) == FAIL)
		return (1);
	return (0);
}
