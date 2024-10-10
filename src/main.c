/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvalogne <gvalogne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 17:04:03 by gvalogne          #+#    #+#             */
/*   Updated: 2024/09/26 17:32:13 by gvalogne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int main(int ac, char **av)
{
	t_params	params;
	pthread_t	monitor_thread;

	if (!init_params(&params, ac, av))
		return (EXIT_FAILURE);
	if (pthread_create(&monitor_thread, NULL, monitor_routine, &params) != 0)
	{
		handle_error("Failed to create monitor thread");
		cleanup(&params);
		return (EXIT_FAILURE);
	}
	pthread_join(monitor_thread, NULL);
	cleanup(&params);
	return (EXIT_SUCCESS);
}