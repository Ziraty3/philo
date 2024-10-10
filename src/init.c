/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalogne <gvalogne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 17:09:36 by gvalogne          #+#    #+#             */
/*   Updated: 2024/09/26 17:49:47 by gvalogne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int init_mutex(t_params *params)
{
	if (pthread_mutex_init(&params->print_mutex, NULL) != 0
	|| pthread_mutex_init(&params->death_mutex, NULL) != 0
	|| pthread_mutex_init(&params->meal_mutex, NULL) != 0
	|| pthread_mutex_init(&params->stop_mutex, NULL) != 0)
	{
		handle_error("Failed to initialize mutex");
		return (0);
	}
	return (1);
}

int init_forks(t_params *params)
{
	int	i;

	params->forks = malloc(params->num_philos * sizeof(pthread_mutex_t));
	if (!params->forks)
	{
		handle_error("Failed to allocate memory for forks");
		return (0);
	}
	i = 0;
	while (i < params->num_philos)
	{
		if (pthread_mutex_init(&params->forks[i], NULL) != 0)
		{
			handle_error("Failed to init fork mutex");
			return (0);
		}
		i++;
	}
	return (1);
}

int init_philo(t_philo *philo, int id, t_params *params)
{
	philo->id = id;
	philo->l_fork = &params->forks[id];
	philo->r_fork = &params->forks[(id + 1) % params->num_philos];
	philo->meals_eaten = 0;
	philo->params = params;
	if (pthread_create(&philo->thread, NULL, philo_routine, philo) != 0)
	{
		handle_error("Failed to allocate memory for forks");
		return (0);
	}
	return (1);
}

int init_philos(t_params *params)
{
	int	i;

	params->philos = malloc(params->num_philos * sizeof(t_philo));
	if (!params->philos)
	{
		handle_error("Failed to allocate memory for philosopers");
		return (0);
	}
	i = 0;
	while (i < params->num_philos)
	{
		if (!init_philo(&params->philos[i], i, params))
			return (0);
		i++;
	}
	return (1);
}

int init_params(t_params *params, int ac, char **av)
{
	if (!parse_arguments(ac, av, params))
		return (0);
	params->start_time = get_timestamp();
	if (!init_forks(params) || !init_mutex(params) || !init_philos(params))
		return (0);
	return (1);
}