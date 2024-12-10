#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char	*ft_strdup(char *s)
{
	int	i;
	char	*res;

	i = 0;
	while (s[i] != '\0')
		i++;
	res = (char *)malloc(sizeof(char) * (i + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		res[i] = s[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

char	*expand_env(char *s)
{
	char	*value;
	char	*var_name; 
	char	*res;

	if (s[0] == '$')
	{
		var_name = s + 1;
		value = getenv(var_name);
		if (value)
			res = ft_strdup(value);
		else
			res = ft_strdup(" ");
	}
	else
		res = ft_strdup(s);
	return (res);
}


//echo $USER
