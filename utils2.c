/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myakoven <myakoven@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 17:15:14 by myakoven          #+#    #+#             */
/*   Updated: 2024/10/26 18:39:28 by myakoven         ###   ########.fr       */
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
		// error_exit(tools, 1);
		// cant do this cause have to free things possibly in calling function
		return (NULL);
	return (str);
}

/* exits the program if exiting*/
int	record_exit(int exit_num, t_tools *tools)
{
	tools->exit_code = exit_num;
	if (tools->exit_string)
	{
		free(tools->exit_string);
		tools->exit_string = NULL;
	}
	tools->exit_string = ft_ltoa(exit_num);
	if (!tools->exit_string)
		error_exit(tools, errno);
	return (exit_num);
}

// void	strip_quotes_final(char *start)
// {
// 	int		i;
// 	char	quotechar;
// 	char	*firstquote;

// 	firstquote = 0;
// 	i = 0;
// 	quotechar = 0;
// 	while (start[i])
// 	{
// 		quotechar = 0;
// 		if (isquote(start[i]))
// 		{
// 			quotechar = start[i];
// 			firstquote = &start[i];
// 			i = skip_quotes(start, i);
// 			if (start[i] && start[i] == quotechar)
// 				i = i - remove_two(firstquote, &start[i]);
// 		}
// 		i++;
// 	}
// }

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
