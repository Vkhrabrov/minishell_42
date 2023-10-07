/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 23:14:19 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/07/21 00:50:33 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

t_list	*ft_lstmap(t_list *lst, void *(*f)(int), void (*del)(int))
{
	t_list	*a;
	t_list	*b;
	t_list	*temp;

	if (!lst || !f)
		return (NULL);
	a = 0;
	while (lst)
	{
		temp = f(lst->content);
		b = ft_lstnew(temp);
		if (!b)
		{
			ft_lstclear(&a, del);
			free(temp);
			return (NULL);
		}
		ft_lstadd_back(&a, b);
		lst = lst -> next;
	}
	return (a);
}
