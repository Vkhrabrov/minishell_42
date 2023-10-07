/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 20:54:40 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/07/21 00:51:01 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

int	ft_lstsize(t_list *lst)
{
	int		i;
	t_list	*check;

	i = 0;
	if (!lst)
		return (0);
	check = lst;
	while ((*check).next != NULL)
	{
		i++;
		check = (*check).next;
	}
	if (check->next == NULL)
		i++;
	return (i);
}

/*int   main(void)
{
    char *str = "Hello World!";
    t_list *b = ft_lstnew(str);
	int z = ft_lstsize(b);
    printf("1: %s\n", b->content);
    printf("2: %p\n", &(b->content));
	printf("3: %d\n", z);
    return(0);
}*/
