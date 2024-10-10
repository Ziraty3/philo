/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalogne <gvalogne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:32:32 by gvalogne          #+#    #+#             */
/*   Updated: 2024/09/26 17:37:50 by gvalogne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int parse_int(const char *s)
{
	int	res;

	res = 0;
	while (*s == ' ' || *s == '\n' || *s == '\t' || *s == '\r')
		s++;
	if (*s == '+' || *s == '-')
		s++;
	if (!*s || *s < '0' || *s > '9')
		return (-1);
	while ('0' <= *s && *s <= '9')
	{
		res = res * 10 + (*s - '0');
		s++;
	}
	if (*s != '\0')
		return (-1);
	return (res);
}

int	read_and_verify_args(int argc, char **argv, t_params *params)
{
	params->num_philos = parse_int(argv[1]);
	params->time_to_die = parse_int(argv[2]);
	params->time_to_eat = parse_int(argv[3]);
	params->time_to_sleep = parse_int(argv[4]);
	if (argc == 6)
		params->max_meals = parse_int(argv[5]);
	else
		params->max_meals = -1;
	if (params->num_philos == -1 || params->time_to_die == -1
		|| params->time_to_eat == -1 || params->time_to_sleep == -1
		|| (argc == 6 && params->max_meals == -1))
	{
		printf("Error: All parameters must be integers.\n");
		return (0);
	}
	return (1);
}

int	parse_arguments(int argc, char **argv, t_params *params)
{
	if (argc < 5 || argc > 6)
	{
		printf("Usage: %s number_of_philos time_to_die time_to_eat time_to_sleep \
[number_of_times_each_philos_must_eat]\n", argv[0]);
		return (0);
	}
	if (!read_and_verify_args(argc, argv, params))
		return (0);
	params->stop = 0;
	return (1);
}