/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 17:15:14 by myakoven          #+#    #+#             */
/*   Updated: 2024/11/11 00:22:35 by myakoven         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

/* 	Allocates, checks the memory allocation,
		and returns the pointer to allocation.
	Calls error_exit in case of memory error  */
char	*safe_calloc(size_t nmemb, size_t size, t_tools *tools)
{
	char	*str;

	str = ft_calloc(nmemb, size);
	if (tools && !str)
	{
		clean_tools(tools);
		error_exit_main(tools, errno); //CHECK
	}
	return (str);
}

/* exits the program if exiting*/
int	record_exit(int exit_num, t_tools *tools)
{
	global_signal = 0;
	tools->exit_code = exit_num;
	if (tools->exit_string)
	{
		free(tools->exit_string);
		tools->exit_string = NULL;
	}
	tools->exit_string = ft_ltoa(exit_num);
	if (!tools->exit_string)
		error_exit_main(tools, errno);
	return (exit_num);
}

/*
Can initialize 2 ints and 2 string pointers
If something isnt necessary, pass in NULL
Used by clean_line()!
*/
void	init_zero(size_t *i, size_t *j, char **str1, char **str2)
{
	if (i)
		*i = 0;
	if (j)
		*j = 0;
	if (str1)
		*str1 = NULL;
	if (str2)
		*str2 = NULL;
}

// reallocation: dest not provided
char	*ft_join_one(char const *s1, char const *delim, char const *s2)
{
	size_t	len;
	size_t	i;
	size_t	j;
	char	*fullstr;

	i = 0;
	j = 0;
	len = ft_strlen(s1) + ft_strlen(delim) + ft_strlen(s2);
	fullstr = ft_calloc(len + 2, sizeof(char));
	if (!fullstr)
		return (NULL);
	j = 0;
	while (s1[j])
		fullstr[i++] = s1[j++];
	j = 0;
	while (delim[j])
		fullstr[i++] = delim[j++];
	j = 0;
	while (s2[j])
		fullstr[i++] = s2[j++];
	fullstr[i] = 0;
	return (fullstr);
}
