#include "minishell.h"


void ft_putstr_fd(char *s, int fd)
{
    write(fd, s, ft_strlen(s));
}

int ft_count_quotes(char *str)
{
    int i = 0;
    int count = 0;
    while (str[i])
    {
        if (str[i] == '\"')
            count++;
        i++;
    }
    return count;
}

int ft_syntax_error(char *input)
{
    int i = 0;
    while (input[i])
    {
        if (ft_count_quotes(input) % 2 != 0)
        {
            ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
            return 1;
        }
        if (input[i] == '|')
        {
            if (input[i + 1] == '|')
            {
                ft_putstr_fd("minishell: syntax error near unexpected token `||'\n", 2);
                return 1;
            }
            else if (input[i + 1] == '\0')
            {
                ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
                return 1;
            }
        }
        if (input[i] == '>')
        {
            if (input [i + 1] == '>')
            {
                if (input[i + 2] == '>')
                {
                    ft_putstr_fd("minishell: syntax error near unexpected token `>>>'\n", 2);
                    return 1;
                }
                else if (input[i + 2] == '\0')
                {
                    ft_putstr_fd("minishell: syntax error near unexpected token `>>'\n", 2);
                    return 1;
                }
            }
            else if (input[i + 1] == '\0')
            {
                ft_putstr_fd("minishell: syntax error near unexpected token `>'\n", 2);
                return 1;
            }
        }
        if (input[i] == '<')
        {
            if (input[i + 1] == '<')
            {
                if (input[i + 2] == '<')
                {
                    ft_putstr_fd("minishell: syntax error near unexpected token `<<<'\n", 2);
                    return 1;
                }
                else if (input[i + 2] == '\0')
                {
                    ft_putstr_fd("minishell: syntax error near unexpected token `<<'\n", 2);
                    return 1;
                }
            }
            else if (input[i + 1] == '\0')
            {
                ft_putstr_fd("minishell: syntax error near unexpected token `<'\n", 2);
                return (1);
            }
        }
        if (input[i] == '<')
        {
            if (access(input, F_OK) == -1)
            {
                ft_putstr_fd("minishell: ", 2);
                perror(input);
                return (1);
            }
        }
        i++;
    }
    return (0);
}