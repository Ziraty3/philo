/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalogne <gvalogne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 16:02:40 by gvalogne          #+#    #+#             */
/*   Updated: 2024/09/26 16:36:12 by gvalogne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void select_forks(t_philo *philo, pthread_mutex_t **first_fork,\
pthread_mutex_t **second_fork)
{
	if (philo->params->num_philos == 1)
	{
		print_philosopher_status(philo, TAKING_FORK);
		custom_usleep(philo->params->time_to_die);
		print_philosopher_status(philo, DEAD);
		pthread_mutex_lock(&philo->params->stop_mutex);
		*first_fork = NULL;
		*second_fork = NULL;
		return ;
	}
	if (philo->id % 2 == 0)
	{
		*first_fork = philo->r_fork;
		*second_fork = philo->l_fork;
	}
	else
	{
		*first_fork = philo->l_fork;
		*second_fork = philo->r_fork;
	}
}

void philosopher_take_forks(t_philo * philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	select_forks(philo, &first_fork, &second_fork);
	if (first_fork && second_fork)
	{
		pthread_mutex_lock(first_fork);
		print_philosopher_status(philo, TAKING_FORK);
		pthread_mutex_lock(second_fork);
		print_philosopher_status(philo, TAKING_FORK);
	}
}

void philo_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->params->death_mutex);
	pthread_mutex_lock(&philo->params->meal_mutex);
	philo->last_meal = get_timestamp();
	print_philosopher_status(philo, EATING);
	pthread_mutex_unlock(&philo->params->death_mutex);
	pthread_mutex_unlock(&philo->params->meal_mutex);
	custom_usleep(philo->params->time_to_eat);
	pthread_mutex_lock(&philo->params->meal_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->params->meal_mutex);
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
}

void philo_sleep(t_philo *philo)
{
	print_philosopher_status(philo, SLEEPING);
	custom_usleep(philo->params->time_to_sleep);
}

void *philo_routine(void *arg)
{
	t_philo	*philo;
	int		should_stop;

	philo = (t_philo *)arg;
	should_stop = 0;
	while (!should_stop)
	{
		philosopher_take_forks(philo);
		if (philo->l_fork == philo->r_fork)
			break ;
		philo_eat(philo);
		philo_sleep(philo);
		print_philosopher_status(philo, THINKING);
		pthread_mutex_lock(&philo->params->stop_mutex);
		should_stop = philo->params->stop;
		pthread_mutex_unlock(&philo->params->stop_mutex);
	}
	return (NULL);
}