

#include "minishell.h"

static int	ft_strrch(char *str, char c)
{
	int	len;

	len = ft_strlen(str);
	while (len > 0)
	{
		if (str[len] == c)
			return (len);
		len--;
	}
	return (0);
}
static char *ft_substrr(char *str, int i)
{
	char	*retl;
	int		len;
	int		j;

	len = 0;
	j = 0; 
	len = ft_strlen(str);
	retl = ft_malloc((len - i + 1) * sizeof(char));
	if (!retl)
		return (NULL);
	while (i < len)
	{
		retl[j] = str[i];
		i++;
		j++;
	}
	retl[j] = '\0';
	return (retl);

}

char **ft_env(t_env *envir)
{
	char **env;
	t_env *tmp;
	int	size;
	int	i;

	tmp = envir;
	size=0;
	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}
	env = ft_malloc((size + 1)*sizeof(char *));
	if (!env)
		return (NULL);
	i = 0;
	tmp = envir;
	while (tmp)
	{
		env[i] = ft_malloc(ft_strlen(tmp->name)+ft_strlen(tmp->value) + 1);
		if(!env[i])
			return (NULL);
		tmp = tmp->next;
		i++;
	}
	i = 0;
	while (envir)
    {
		env[i] = ft_strdup(envir->name);
        env[i] = ft_strjoin(env[i], "=");
        env[i] = ft_strjoin(env[i], envir->value);
        envir = envir->next;
		i++;
    }
	env[i] = NULL;
	return (env);
}

 t_redirect	*ft_lstlastred(t_redirect *lst)
{
	if (lst == NULL)
		return (NULL);
	if (lst->next == NULL)
		return (lst);
	return (ft_lstlastred(lst->next));
}
void ft_red_doc(t_redirect *tmp)
{
	char *str;

	if (pipe(tmp->pip) == -1)
	{
		perror("pipe");
		exit(1);
	}
		// printf("%s\n", tmp->file);
	while (1)
	{
		str = readline("> ");
		// printf("%s\n", str);
		if (!str || !ft_strcmp(tmp->file , str))
			break ;
		ft_putstr_fd(str, tmp->pip[1]);
		write(tmp->pip[1], "\n", 1);
		free(str);
	}
}

void ft_exec(t_cmd *tmp, char **env)
{
	char **cmdd;
	t_redirect *red;
	char	*str;
	int		i;
	// int		fd;

	i = 0;
	red = tmp->redirect;
	if (red)
	{
		while (red)
		{
			if (red->type == 1)
			{
				red->fd  = open(red->file , O_CREAT | O_WRONLY | O_TRUNC, 0644);
				if (red->fd == -1)
				{
					perror(red->file);
					exit(1);
				}
				dup2(red->fd, 1);
				close (red->fd);
			}
			if (red->type == 2)
			{
				if (access(red->file, R_OK) == -1)
				{
					perror(red->file);
					exit(1);
				}
				red->fd = open(red->file, O_RDONLY);
				if (red->fd == -1)
				{
					perror(red->file);
					exit(1);
				}
				dup2(red->fd, 0);
				close(red->fd);
			}
			if (red->type == 3)
			{
				red->fd = open(red->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
				if (red->fd == -1)
				{
					perror(red->file);
					exit(1);
				}
				dup2(red->fd, 1);
				close (red->fd);
			}
			if(red->type == 4)
			{
				ft_red_doc(red);
				if (ft_lstlastred(red) == red)
{				dup2(ft_lstlastred(red)->pip[0], 0);
				close(ft_lstlastred(red)->pip[0]);
				close(ft_lstlastred(red)->pip[1]);
			}			}
		red = red->next;
		}
		// if (ft_lstlastred(tmp->redirect)->type == 1 || ft_lstlastred(tmp->redirect)->type == 3)
		// {	fd = ft_lstlastred(tmp->redirect)->fd;
		// 	dup2(fd, 1);
		// 	close (fd);}
		while (tmp->redirect)
		{
			close (tmp->redirect->fd);
			tmp->redirect = tmp->redirect->next;
		}
	}
	i = ft_strrch(tmp->cmd , '/');
	str = ft_substrr(tmp->cmd, i);
    str = ft_strjoin(str, " ");
    str = ft_strjoin(str, tmp->flags);
    str = ft_strjoin(str, " ");
    str = ft_strjoin(str, tmp->files);
	str = ft_strjoin(str , " ");

	for (int i = 0; tmp->args[i]  ; i++)
	{
		str = ft_strjoin(str, tmp->args[i]);
		str = ft_strjoin(str, " ");
	}

	cmdd = ft_split(str, ' ');
	if (execve(tmp->cmd, cmdd, env) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(tmp->cmd, 2);
		ft_putstr_fd(" command not found\n", 2);
		exit(127);
	}
}


void	new_pipe(t_cmd *data)
{
	int	i;
    int ac;

    ac = 0;
	t_cmd *tmp3 = data; 
    while (tmp3->next)
    {
        ac++;
		tmp3 = tmp3->next;
    }
	data->fd = malloc((ac) * sizeof(int *));
	if (!data->fd)
		return ;
	i = 0;
	while (i < ac)
	{
		data->fd[i] = malloc(2 * sizeof(int));
		i++;
	}
	i = 0;
	while (i < ac)
	{
		if (pipe(data->fd[i]) == -1)
		{
			perror("pipe");
			exit(1);
		}
		i++;
	}
	int **f = data->fd;
	tmp3 = data;
	while (tmp3)
    {
        tmp3->fd = f; 
		tmp3 = tmp3->next;
    }
}

void	ft_close(t_cmd *data, int	i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		close (data->fd[j][1]);
		close (data->fd[j][0]);
		j++;
	}
} 