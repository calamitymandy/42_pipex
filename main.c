/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amdemuyn <amdemuyn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 16:16:21 by amdemuyn          #+#    #+#             */
/*   Updated: 2023/06/28 16:17:06 by amdemuyn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>

char    *find_path(char **envp)
{
    int i;

    i = 0;
    while (envp[i])
    {
        if (envp[i][0] == 'P' && envp[i][1] == 'A' && envp[i][2] == 'T' && envp[i][3] == 'H')
        {
            //printf("%c", envp[i][5]);
            return (&envp[i][5]);
        }
        i++;
    }
    return (0);
}

int main(int argc, char **argv, char **envp)
{
    char    *path;

    if (argc == 2)
    {
        path = find_path(envp);
        printf("%s", path);
    }
    else
        printf("fuck you");
    return (0);
}