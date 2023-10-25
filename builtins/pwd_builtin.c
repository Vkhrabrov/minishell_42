/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 19:20:45 by ccarrace          #+#    #+#             */
/*   Updated: 2023/10/11 19:52:26 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int pwd(void)
{
    char buffer[PATH_MAX];
    char 
    curr_work_dir = getcwd(buffer, sizeof(buffer));
    
    if (curr_work_dir)
        printf("%s\n", buffer);
    else
        perror("getcwd");
    return 0;
}
