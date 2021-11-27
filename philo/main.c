/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanlee <kanlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 15:03:58 by kanlee            #+#    #+#             */
/*   Updated: 2021/11/27 17:51:35 by kanlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "philo.h"
#include <sys/time.h>

int	prn_error(int err)
{
	if (err == WRONG_ARGS)
		printf("Wrong arguments\n");
	else if (err == NO_PHILO)
		printf("There should be at least one philosopher\n");
	else if (err == NEGATIVE_ARGS)
		printf("Incorrent time arguments\n");
	else if (err == INVALID_OPT_ARG)
		printf("Invalid optional argument\n");
	else if (err == MALLOC_FAIL)
		printf("Error while memory allocation\n");
	else
		printf("some error\n");
	return (FAIL);
}

int	chk_args(int ac, char **av, t_rule *rule)
{
	rule->must_eats = 999;

	if (ac != 5 && ac != 6)
		return (prn_error(WRONG_ARGS));
	rule->num = ft_atoi(av[1]);
	rule->time_to_die = ft_atoi(av[2]);
	rule->time_to_eat = ft_atoi(av[3]);
	rule->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		rule->must_eats = ft_atoi(av[5]);
	if (rule->num <= 0)
		return (prn_error(NO_PHILO));
	if (rule->time_to_die <= 0 || rule->time_to_eat <= 0 || rule->time_to_sleep <= 0)
		return (prn_error(NEGATIVE_ARGS));
	if (rule->must_eats <= 0)
		return (prn_error(INVALID_OPT_ARG));
	return (SUCCESS);
}

int	main(int ac, char **av)
{
	t_rule	rule;

	if (chk_args(ac, av, &rule) == FAIL)
		return (1);
	printf("hello\n");

	if (init(&rule) == FAIL)
		return (prn_error(3));
	return (0);
}
