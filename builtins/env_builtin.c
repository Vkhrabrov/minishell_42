/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarrace <ccarrace@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 21:00:11 by ccarrace          #+#    #+#             */
/*   Updated: 2023/10/15 11:56:29 by ccarrace         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

/*
    'envp' is derived from "environment pointer".
    The 'envp' argument is automatically provided by the operating system.
*/
void env(char *envp[]) 
{
    int i;

    i = 0;
    while (envp[i] != NULL)
    {
        printf("%s\n", envp[i]); 
        i++;       
    }
}

int main(int argc, char *argv[], char *envp[]) 
{
    (void)argc;
    (void)argv;

    env(envp);
    return (0);
}
