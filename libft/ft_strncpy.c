#include "libft.h"

char	*ft_strncpy(const char *src, size_t n)
{
	char	*dst;
	size_t	i;

	if (!src)
		return (NULL);
	dst = malloc(n + 1);
	if (!dst)
		return (NULL);
	i = 0;
	while (i < n && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	while (i < n)
		dst[i++] = '\0';
	dst[n] = '\0';
	return (dst);
}
