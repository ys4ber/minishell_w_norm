/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_store.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaber <ysaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 17:47:26 by ysaber            #+#    #+#             */
/*   Updated: 2024/03/12 17:59:03 by ysaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*ft_store_environment(char *env)
{
	t_env	*environment;
	int		i;

	i = 0;
	while (env[i] != '=')
		i++;
	environment = (t_env *)ft_malloc(sizeof(t_env));
	environment->name = ft_substr(env, 0, i);
	environment->value = ft_substr(env, i + 1, ft_strlen(env) - i);
	environment->next = NULL;
	return (environment);
}

t_env *ft_create_env(char *name, char *value)
{
	t_env *env;

	env = (t_env *)ft_malloc(sizeof(t_env));
	env->name = name;
	env->value = value;
	env->next = NULL;
	return (env);
}

t_env *ft_add_env(t_env *env, char *name, char *value)
{
	t_env *tmp;
	t_env *new;

	tmp = env;
	new = ft_create_env(name, value);
	if (!env)
		return (new);
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (env);
}



t_env	*ft_get_env(char **env)
{
	t_env	*environment;
	t_env	*head;
	int		i;
	if (!env || !*env)
	{
	    environment = ft_create_env("PWD", getcwd(NULL, 0));
	    head = environment;
	    environment->next = ft_create_env("SHLVL", "1");
	    environment = environment->next;
	    environment->next = ft_create_env("OLDPWD", getcwd(NULL, 0));
	    environment = environment->next;
	    //creating "_" variable
	    environment->next = ft_create_env("_", "minishell");
	    environment = environment->next; // Move to the "_" environment variable
	    environment->next = NULL; // Set the next field of "_" to NULL
	    return (head);
	}
	i = 0;
	while (env[i])
	{
		if (i == 0)
		{
			environment = ft_store_environment(env[i]);
			head = environment;
		}
		else
		{
			environment->next = ft_store_environment(env[i]);
			environment = environment->next;
		}
		i++;
	}
	return (head);
}

char	*ft_get_env_value(char *name, t_env *envir)
{
	t_env	*tmp;

	tmp = envir;
	while (tmp)
	{
		if (ft_strncmp(tmp->name, name, ft_strlen(name)) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

static char	*find_cmd_path(char **path_split, char *cmd)
{
	int		i;
	char	*cmd_path;

	i = 0;
	if(!path_split || !*path_split)
	return (NULL);
	while (path_split[i])
	{
		cmd_path = ft_strjoin2(path_split[i], "/", cmd);
		if (access(cmd_path, F_OK | X_OK) == 0)
		{
			return (cmd_path);
		}
		i++;
	}
	return (NULL);
}

t_cmd	*ft_get_path(t_cmd *cmd, t_env *envir)
{
	t_cmd	*tmp;
	char	*path;
	char	**path_split;
	char	*cmd_path;

	tmp = cmd;
	path = ft_get_env_value("PATH", envir);
	path_split = ft_split(path, ':');
	while (tmp)
	{
		if (access(tmp->cmd, F_OK | X_OK) != 0)
		{
			cmd_path = find_cmd_path(path_split, tmp->cmd);
			if (cmd_path)
				tmp->cmd = cmd_path;
		}
		tmp = tmp->next;
	}
	return (cmd);
}
