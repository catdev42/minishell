#include <unistd.h>

char	*ft_strdup(char *source) // receive "hello"
{
	char *pointer;
	int len;

    pointer = NULL;
	len = ft_strlen(source) + 2;
	pointer = malloc(len * sizeof(char));
	if (!pointer)
        return NULL;
    ft_strncpy(pointer, source, len);
    return (pointer);
}
// 5 + 1 * 4 = 9
// (5 + 1) * 4 = 24

int	main(void)
{
	char *result_alloc;
	char *str = "hello";
	result_alloc = ft_strdup(str);
	printf("%s", result_alloc);
	free(result_alloc);
	return (0);
}