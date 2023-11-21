/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 11:36:02 by ccarrace          #+#    #+#             */
/*   Updated: 2023/11/19 20:32:02 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* find_char_index() called in
	- env_list_creation()
	- export_builtin()
*/

/* find_max_len() called in
	- builtins_execute.c
	- exit_builtin.c
	- export_builtin.c
*/

/* ft_list_size() called in
	- export_builtin.c
	- exit_builtin.c
*/

/* get_env_var_value() called in
	- echo_builtin.c
	- cd_builtin.c
*/
int	ft_list_size(token *args_lst)
{
	int	i;

	i = 0;
	while (args_lst)
	{
		i++;
		args_lst = args_lst->next;
	}
	return (i);
}

void	free_args_list(token *args_lst)
{
    token *current;

    while (args_lst != NULL)
	{
        current = args_lst;
        free(current->content);
		free(current);
        args_lst = args_lst->next;
    }	
}

char	*get_env_var_value(t_env_lst *env_lst, char *str)
{
	while (env_lst != NULL)
	{
		if (ft_strncmp(env_lst->var_name, str, find_max_len(env_lst->var_name, str)) == 0)
			return(env_lst->var_value);
		env_lst = env_lst->next;
	}
	return (str);
}

void	update_env_var_value(t_env_lst *env_lst, char *sought_name, char *new_value) 
{
	t_env_lst	*temp_ptr;

	temp_ptr = env_lst;
	while (temp_ptr)
	{
		if (ft_strncmp(temp_ptr->var_name, sought_name, find_max_len(temp_ptr->var_name, sought_name)) == 0)
		{
			free(temp_ptr->var_value);
			temp_ptr->var_value = strdup(new_value);
			return ;
		}
		temp_ptr = temp_ptr->next;
	}
	free(temp_ptr);
}

long	ft_atol(const char *str)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	while (*str == 32 || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = (result * 10) + (*str - '0');
		str++;
	}
	return (result * sign);
}
/*	Start of ft_ltoa (long to char)	and complementary functions */
static long	ft_long_length(long n)
{
	long	len;

	if (n <= 0)
		len = 1;
	else
		len = 0;
	while (n != 0)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

static char	*ft_long_min(void)
{
	char	*str;
	char	*tmp;
	int		i;

	i = 0;
	str = malloc(sizeof(char) * 21);
	if (!str)
		return (NULL);
	tmp = "-9223372036854775808";
	while (i < 20)
	{
		str[i] = tmp[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_ltoa(long n)
{
	long	len;
	char	*str;

	len = ft_long_length(n);
	// LL suffix ensures number is interpreted as a long integer literal.
	if (n == -9223372036854775807LL - 1)
		return (ft_long_min());
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (n == 0)
		str[0] = n + '0';
	if (n < 0)
	{
		str[0] = '-';
		n = n * (-1);
	}
	while (n > 0)
	{
		str[len - 1] = (n % 10) + '0';
		n = n / 10;
		len--;
	}
	return (str);
}
/*	End of ft_ltoa	*/