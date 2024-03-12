/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkazaz <lkazaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 17:21:16 by lkazaz            #+#    #+#             */
/*   Updated: 2024/03/11 17:43:35 by lkazaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void ft_echo(t_cmd *tmp)
{
    if (tmp && tmp->args)
    {
        for (int i = 0; tmp->args[i]; i++)
        {
            printf("%s ", tmp->args[i]);
        }
    }
    if (!tmp->flags)
    printf("\n");
}

char *t_getdata(t_env *envir, char *str)
{
    while (envir)
    {
        if(!ft_strcmp(envir->name , str))
            return(envir->value);
        envir = envir->next;
    }
    return (NULL);
}

void    ft_modifenv(t_env *envir, char *chr, char *new)
{
    while (envir)
    {
        if(!ft_strcmp(envir->name , chr))
            envir->value = ft_strdup(new);
        envir = envir->next;
        
    }
}

void ft_cd(t_cmd *tmp, t_env *envir)
{
    char *home;
    char oldpwd[256];
    char newpath[256];
    int   i;

    i = 0;
    getcwd(oldpwd, 256);
    home = t_getdata(envir, "HOME");
    if (tmp->args[0])
    {
       
            ft_putstr_fd("minishell: cd: ", 2);
            ft_putstr_fd(tmp->args[0], 2);
            ft_putstr_fd(": No such file or directory\n", 2);
            exit (1);
    }
    if (!tmp->files)
        tmp->files = ft_strdup(home);
    if (chdir(tmp->files) == -1)
    {
        perror("cd");
        return ;
    }
    else
    {
        i++;
        //pwd = ft_strdup(tmp->files);
    }
    if (i)
    {
        getcwd(newpath, 256);
        ft_modifenv(envir, "PWD", newpath);
        ft_modifenv(envir, "OLDPWD", oldpwd);
    }
}

void    ft_pwd(void)
{
    char s[256];
    getcwd(s, 256);
    printf("%s\n", s);
}

void    ft_affichenv(t_env *envir)
{
    char **env;

    env = ft_env(envir);
    while (env && *env)
    {
        printf("%s\n", *env);
        env++;
    }
}

void ft_unset(t_cmd *cmd, t_env **head_ref)
{ 
   t_env *temp = *head_ref, *temp1;
    if (temp != NULL && temp->name == cmd->args[0]) 
    { 
        *head_ref = temp->next; 
        free(temp);
        return; 
    } 
    while (temp != NULL && ft_strcmp(temp->name, cmd->args[0]))
    {
        temp1 = temp; 
        temp = temp->next; 
    }
    if (temp == NULL) 
       return; 
     temp1->next = temp->next; 
        free(temp);
}

t_env	*ft_lstlast(t_env *lst)
{
	if (lst == NULL)
		return (NULL);
	if (lst->next == NULL)
		return (lst);
	return (ft_lstlast(lst->next));
}

void	ft_lstadd_back(t_env **lst, t_env *new)
{
	t_env	*p;
    t_env   *tmp;
    
    p = *lst;
    while( p->next->next)
        p =p->next;
     tmp = p->next;
     p->next = new;
     p->next->next = tmp;
}

int ft_strnchr(const char *str, int a)
{
	char			*c;
	unsigned char	l;
    int             i;

	c = (char *)str;
    i = 0;
	l = (unsigned char )a;
	while (*c)
	{
		if (*c == l)
			return (i);
		c++;
        i++;
	}
	if (l != 0)
		return (0);
	return (0);
}

void    ft_export(t_cmd *cmd, t_env **envir)
{
    int len;
    t_env *last;
    t_env *new;
    int i;

    i = 0;
    last = ft_malloc(sizeof(t_env));
    if (!last)
        return ;
    last = ft_lstlast(*envir);
    while (cmd->args[i])
    {
        if (ft_strnchr(cmd->args[i], '=')>0)
        {
            new=ft_malloc(sizeof(t_env));
            if (new == NULL)
	        	return ;
            len = 0;
            while (cmd->args[i][len])
                 len++;
            new->name = ft_substr(cmd->args[i], 0, ft_strnchr(cmd->args[i], '='));
            new->value = ft_substr(cmd->args[i], ft_strnchr(cmd->args[i], '=') + 1, len);
	        new -> next = NULL;
            ft_lstadd_back(envir, new);
        }
        i++;
    }
}

int ft_builtins(t_cmd *tmp, t_env *envir)
{
    char *command;

    if (tmp && tmp->cmd) // Check if tmp and tmp->cmd are not NULL
    {
        command = ft_last_world(tmp->cmd);
        if (command) // Check if command is not NULL
        {
            if (ft_strcmp(command, "echo") == 0)
            {
                ft_echo(tmp);
                return (0);
            }
            else if (ft_strcmp(command, "cd") == 0)
            {
                ft_cd(tmp, envir);
                return (0);
            }
            else if (ft_strcmp(command, "pwd") == 0)
            {
                ft_pwd();
                return (0);
            }
            else if ((ft_strcmp(command, "env") == 0))
            {
                ft_affichenv(envir);
                return (0);
            }
            else if ((ft_strcmp(command, "unset") == 0))
            {
                ft_unset(tmp, &envir);
                return (0);
            }
            else if (ft_strcmp(command, "export")==0)
            {
                ft_export(tmp, &envir);
                return (0);   
            }
        }
        else
            return (1);
    }
        return (-1);
}
