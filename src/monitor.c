/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ziratya <ziratya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:09:50 by gvalogne          #+#    #+#             */
/*   Updated: 2024/10/08 16:40:56 by ziratya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void monitor_philo(t_params *params, int i)
{
	long	current_time;

	pthread_mutex_lock(&params->death_mutex);
	pthread_mutex_lock(&params->meal_mutex);
	current_time = get_timestamp();
	if ((current_time - params->philos[i].last_meal) > params->time_to_die)
	{
		print_philosopher_status(&params->philos[i], DEAD);
		pthread_mutex_lock(&params->stop_mutex);
		params->stop = 1;
		pthread_mutex_unlock(&params->stop_mutex);
	}
	pthread_mutex_unlock(&params->death_mutex);
	pthread_mutex_unlock(&params->meal_mutex);
}

int	monitor_meals(t_params *params)
{
	int	all_philosophers_done;
	int	i;

	all_philosophers_done = 1;
	i = 0;
	while (i < params->num_philos)
	{
		monitor_philo(params, i);
		if (params->max_meals > 0)
		{
			pthread_mutex_lock(&params->meal_mutex);
			if (params->philos[i].meals_eaten < params->max_meals)
				all_philosophers_done = 0;
			pthread_mutex_unlock(&params->meal_mutex);
		}
		else
		{
			all_philosophers_done = 0;
		}
		if (params->stop)
			break ;
		i++;
	}
	return (all_philosophers_done);
}

void	check_meals(t_params *params)
{
	pthread_mutex_lock(&params->print_mutex);
	printf("All philosophers have eaten at least %d times.\n", \
	params->max_meals);
	pthread_mutex_lock(&params->stop_mutex);
	params->stop = 1;
	pthread_mutex_unlock(&params->stop_mutex);
	pthread_mutex_unlock(&params->print_mutex);
}

void	*monitor_routine(void *arg)
{
	t_params	*params;
	int			should_stop;

	params = (t_params *)arg;
	should_stop = 0;
	while (!should_stop)
	{
		if (monitor_meals(params))
			check_meals(params);
		pthread_mutex_lock(&params->stop_mutex);
		should_stop = params->stop;
		pthread_mutex_unlock(&params->stop_mutex);
		usleep(100);
	}
	return (NULL);
}