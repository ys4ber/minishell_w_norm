#include "minishell.h"


#define WCONTINUED 8

void ft_execute(t_cmd *cmd , t_env *envir)
{
	t_cmd   *tmp=cmd;
    char    **env;
    int     pip=0;
    int     *pid;
    int     status;
    env = ft_env(envir);
    
    while (tmp->next)
    {
        pip++;
        tmp = tmp->next;
    }
    t_cmd *cmd1 = cmd;
    t_cmd *cmd2 = cmd;
    new_pipe(cmd1);
    
	int i = 0;
    pid = ft_malloc((pip + 1)*sizeof(int));
	while (cmd)
	{
        pid[i] = fork();
        if (pid[i] == -1)
		{
			perror("fork");
			exit(1);
		}
        if (pid[i] == 0)
        {
            if (pip > 0)
            {
                if (!ft_builtins(cmd, envir))
                    exit(0) ;
                if (i == 0)
                    (dup2(cmd->fd[0][1], 1),ft_close(cmd, pip),ft_exec(cmd, env));
                if (i != pip)
                    (dup2(cmd->fd[i - 1][0], 0),dup2(cmd->fd[i][1], 1),ft_close(cmd, pip),ft_exec(cmd, env));
                if (i == pip)
                    (dup2(cmd->fd[i -1][0], 0),ft_close(cmd, pip),ft_exec(cmd, env));
            }
            else
            {
                 if (!ft_builtins(cmd, envir))
                    return ;
                ft_exec(cmd, env);
            }
        }
		cmd = cmd->next;
		i++;	
	}
    ft_close(cmd2, pip);
    int k=0;
    while(k < i)
    {
        waitpid(pid[k], &status, WCONTINUED);
        k++;
    }

}

t_cmd	*ft_set_arguments(char **args, t_cmd *cmd)
{
    int	i = 0;
    int	j;

    cmd->cmd = NULL;
    cmd->flags = NULL;
    cmd->files = NULL;
    if (args[0] == NULL)
        return (NULL);
    while (args[i])
    {  
        if (ft_strcmp(args[i], "<") == 0)
        {
            int fd = open(args[i + 1], O_RDONLY);
            if (fd == -1)
            {
                perror(args[i + 1]);
                return NULL;
            }
            if (args[i + 2])
            {
                close(fd);
            }
            else
                break;
            i += 2;
        }
        else
            break;
    }
    if (args[i] && ft_strcmp(args[i], "<") != 0 && ft_strcmp(args[i], ">") != 0 
        && ft_strcmp(args[i], ">>") != 0 && ft_strcmp(args[i], "<<") != 0)
    {
        cmd->cmd = args[i];
        i++;
    }
    cmd->args = (char **)ft_malloc(sizeof(char *) * (ft_args_len(args) + 1)); // Allocate memory for args
    j = 0;
    while (args[i])
    {
        if (args[i][0] == '-')
        {
            if (cmd->flags == NULL)
                cmd->flags = args[i];
            else
                cmd->flags = ft_strjoin2(cmd->flags, " ", args[i]);
        }
        else if (access(args[i], F_OK) == 0)
        {
            if (cmd->files == NULL)
                cmd->files = args[i];
            else
                cmd->files = ft_strjoin2(cmd->files, " ", args[i]);
        }
        else if (strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0 || strcmp(args[i], "<") == 0 || strcmp(args[i], "<<") == 0)
        {
            if (args[i + 1])
            {
                i += 2;
                continue;
            }
            else
            {
                printf("Error: No file specified for redirection\n");
                return NULL;
            }
        }
        else
        {
            cmd->args[j] = args[i];
            j++;
        }
        i++;
    }
    cmd->args[j] = NULL;
    return (cmd);
}

char **ft_expand(char **args, t_env *envir)
{
    int i;
    int j;
    char **new_args;
    //char *tmp;
    char *value;

    i = 0;
    j = 0;
    new_args = (char **)ft_malloc(sizeof(char *) * (ft_args_len(args) + 1));
    while (args[i])
    {
        if (args[i][0] == '$')
        {
            value = ft_get_env_value(&args[i][1], envir);
            if (value != NULL)
            {
                new_args[j] = value;
                j++;
            }
        }
        // if (args[i][0] == '$' && args[i][1] == '?')
        // {
        //     new_args[j] = ft_itoa(exit_status);
        //     j++;
        // }
        else
        {
            new_args[j] = args[i];
            j++;
        }
        i++;
    }
    new_args[j] = NULL;
    return (new_args);
}


void ft_minishell(t_node *node, t_env *envir, char **env)
{
    t_cmd			*cmd;
    t_cmd			*current_cmd;
    t_cmd			*cmd_path;
    t_cmd *new_cmd = NULL;
    char			**args;
    (void)			env;

    cmd = ft_malloc(sizeof(t_cmd));
    if (node->args == NULL)
        return;
    current_cmd = cmd;
    t_redirect *first_redirect = NULL;
    t_redirect *current_redirect = NULL;
    for (t_node *tmp1 = node; tmp1; tmp1 = tmp1->next)
    {
        char *strtrim = ft_strtrim(tmp1->args, " ");
        args = ft_split2(strtrim, ' ');

        if (!args[0])
            return ;

        for (int k = 0; args[k]; k++)
        {
            args[k] = remove_all_quotes(args[k]);
        }

        args = ft_expand(args, envir);
        t_redirect *new_redirect = ft_redirection(args); // all the redirection are set here
        if (new_redirect != NULL)
        {
            if (first_redirect == NULL)
            {
                first_redirect = new_redirect;
                current_redirect = new_redirect;
            }
            else
            {
                current_redirect->next = new_redirect;
                current_redirect = new_redirect;
            }
        }
        new_cmd = ft_set_arguments(args, current_cmd);
        if (new_cmd == NULL)
            return ;
        current_cmd = new_cmd;
        current_cmd->redirect = first_redirect;
        first_redirect = NULL;
        if (tmp1->next)
        {
            current_cmd->next = ft_malloc(sizeof(t_cmd));
            current_cmd = current_cmd->next;
        }
        else
            current_cmd->next = NULL;
    }
    cmd_path = ft_get_path(cmd, envir);
    ft_execute(cmd_path, envir);
}


void ft_parse_input(char *input, t_env *envir, char **env)
{
    if (input == NULL || ft_strlen(input) == 0)
        return ;
    t_data *data = ft_set_in_pipe(input);
    // if (ft_syntax_error(data->str) == 1)
    //     return ;
    char *str = ft_set_spaces(data->str);
    char **pipe = ft_split(str, '|');
    for (int i = 0; pipe[i]; i++)
    {
        for (int j = 0; pipe[i][j]; j++)
        {
            if (pipe[i][j] == SPECIAL_CHAR)
                pipe[i][j] = '|';
        }
    }
    t_node *node = ft_malloc(sizeof(t_node));
    t_node *tmp = node;
    for (int i = 0; pipe[i]; i++)
    {
        tmp->args = ft_strdup(pipe[i]);

        if (pipe[i + 1])
        {
            tmp->next = ft_malloc(sizeof(t_node));
            tmp = tmp->next;
        }
        else
            tmp->next = NULL;
    }

    ft_minishell(node, envir, env);
}

int	main(int ac, char **av, char **env)
{
	char	*input;
	t_env	*envir;

	(void)ac;
	(void)av;
	envir = ft_get_env(env);

    // for(t_env *tmp = envir; tmp; tmp = tmp->next)
    // {
    //     printf("%s=%s\n", tmp->name, tmp->value);
    // }   
	
	while (1)
	{
		input = readline("minishell\n|___>$");
		if (!input)
			printf("\n");
		else
			ft_parse_input(input, envir, env);
		add_history(input);
		if (ft_strncmp(input, "exit", 4) == 0)
		{
			free(input);
			break ;
		}
        free(input);
	}
    ft_free_all();
}

