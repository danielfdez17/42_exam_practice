#include "get_next_line.h"


int	ft_strlen(char *str)
{
	int i = 0;
	if (!str)
		return (0);
	while (str[i])
		++i;
	return i;
}

char	*ft_find_endl(char *str)
{
	int i = 0;
	if (!str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\n')
			return (&str[i]);
		++i;
	}
	return (NULL);
}

char	*ft_substr(char *str, int start, int len)
{
	int str_len = ft_strlen(str);
	if (len > str_len - start)
		len = str_len - start;
	char *new_str = malloc(sizeof(char) * (len + 1));
	if (!new_str)
		return (NULL);
	int i = 0;
	while (str[start + i] && i < len)
	{
		new_str[i] = str[start + i];
		++i;
	}
	new_str[i] = '\0';
	return (new_str);
}

char	*ft_strjoin(char *s1, char *s2)
{
	int len = ft_strlen(s1) + ft_strlen(s2) + 1;
	char *join = malloc(sizeof(char *) * len);
	if (!join)
		return (NULL);
	int i = 0;
	while (s1 && s1[i])
	{
		join[i] = s1[i];
		++i;
	}
	int j = 0;
	while(s2 && s2[j])
		join[i++] = s2[j++];
	join[i] = '\0';
	free(s1);
	free(s2);
	return (join);
}
