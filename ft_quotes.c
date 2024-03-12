 #include "minishell.h"

static int index_of_next_quote(char *str, int start, char quote)
{
    int len = ft_strlen(str);
    int i = start;
    while (i < len)
    {
        if (str[i] == quote)
            return i;
        i++;
    }
    return -1;
}

static char *remove_quotes(char *str, char quote)
{
    int len = ft_strlen(str);
    int i = 0;
    int j = 0;
    char *new = NULL;
    char *last = NULL;
    while (str[i] && i < len)
    {
        if (str[i] == quote)
        {
            i++;
        }
        else
        {
            j = index_of_next_quote(str, i, quote);
            if (j == -1)
            {
                new = ft_substr(str, i, len - i);
                last = ft_strjoin(last, new);
                free(new);
                break;
            }
            else
            {
                new = ft_substr(str, i, j - i);
                last = ft_strjoin(last, new);
                free(new);
                i = j + 1;
            }
        }
    }
    if (last == NULL)
        return str;
    return last;
}

int check_unbalanced_quotes(char *str)
{
    int single_quotes = 0;
    int double_quotes = 0;
    for (int i = 0; str[i]; i++)
    {
        if (str[i] == '\'') single_quotes++;
        if (str[i] == '\"') double_quotes++;
    }
    
    return single_quotes % 2 != 0 || double_quotes % 2 != 0;
}

char *remove_all_quotes(char *str)
{
    if (str == NULL)
        return NULL;
    if (check_unbalanced_quotes(str)) {
        printf("Syntax error: unbalanced quotes\n");
        return NULL;
    }
    char *new = NULL;
    int flag = 0;
    if (str[0] == '\"' || str[0] == '\'')
    {
        if (str[0] == '\"')
        {
            flag = 1;
            new = remove_quotes(str, str[0]);
        }
        else if (str[0] == '\'')
        {
            new = remove_quotes(str, str[0]);
        }
        else if (str[0] == '\'' && flag == 1)
        {
            new = remove_quotes(str, str[0]);
        }
    }
    else
        return str;
    return new;
}
