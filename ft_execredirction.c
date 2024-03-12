/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execredirction.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkazaz <lkazaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 22:47:32 by lkazaz            #+#    #+#             */
/*   Updated: 2024/03/08 23:17:50 by lkazaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    ft_execred(t_cmd *tmp, t_env *envir)
{
    (void)envir;
    if (tmp->redirect->file)
    {
        if (tmp->redirect->type == 1)
        {
            printf(">\n");
            exit(1);
        }
        else if (tmp->redirect->type == 3)
        {
            printf(">>\n");
            exit(1);
        }
    }   
}
