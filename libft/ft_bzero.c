/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhrabro <vkhrabro@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 20:41:15 by vkhrabro          #+#    #+#             */
/*   Updated: 2023/01/21 23:05:09 by vkhrabro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	*ft_bzero(void *ptr, unsigned long n)
{
	unsigned char	*p;

	p = (unsigned char *)ptr;
	while (n--)
	{
		*p = '\0';
		p++;
	}
	return (p);
}
