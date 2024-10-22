#include "./include/minishell.h"
#include <fcntl.h>
#include <unistd.h>

static size_t	ft_length_long(long int n)
{
	size_t		len;
	long int	num;

	num = n;
	len = 1;
	if (num < 0)
	{
		num *= -1;
		len++;
	}
	while (num >= 10)
	{
		num = num / 10;
		len++;
	}
	return (len);
}
char	*ft_ltoa(long int n)
{
	char		*str;
	size_t		len;
	long int	num;

	num = n;
	len = ft_length_long(num);
	str = malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	if (num < 0)
	{
		str[0] = '-';
		num *= -1;
	}
	str[len] = 0;
	while (num >= 10)
	{
		str[len - 1] = num % 10 + 48;
		num /= 10;
		len--;
	}
	str[len - 1] = num + 48;
	return (str);
}
int	main(void)
{
	char	*ok;
	char	*ok2;
	int		fd;
	char	heredocs[MAXARGS][MAXARGS];

	name_heredoc(ok);
	fd = open(ft_ltoa((long int)ok), O_RDWR | O_CREAT | O_APPEND, 0644);
	write(fd, "hello", 5);
}

void	here_init(char heredocs[MAXARGS][MAXARGS]) // add tools
{
	int i;
	char *tempalloc;

	i = 0;
	while (i < MAXARGS)
	{
		tempalloc = NULL;
		tempalloc = ft_atoi(i);
		if (!tempalloc)
			exit(1); // add clean tools
		ft_strlcpy(heredocs[i], "heredoc", MAXARGS);
		ft_strlcat(heredocs[i], i, MAXARGS);
		free(tempalloc);
	}
	return ;
}

// name_heredoc(char *pointer, char heredocs[][])
// {
// 	int		i;
// 	char	*tempstralloc;

// 	while (heredoc[i][0])
// 		i++;
// 	tempstralloc = ft_atol((long int)pointer);
// 	ft_strlcpy(heredocs[i], )
// 	/*
// 	//  to delete a file
// 			if (unlink(filename) == 0) {
// 			printf("File deleted successfully.\n");
// 		} else {
// 			perror("Error deleting file");
// 		}
// 	*/
// }
