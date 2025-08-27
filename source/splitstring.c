#include <string.h>
#include <stdlib.h>

#include "splitstring.x"

void SplitString(char *string, char *delimiter, char ***ret_tokens, size_t *ret_token_count)
{
    size_t delimiter_length = strlen(delimiter);

    size_t tokens_size = 16;
    (*ret_token_count) = 0;
    (*ret_tokens) = malloc(tokens_size * sizeof(**ret_tokens));

    char *current_position = string;
    if (delimiter_length == 0)
    {
        goto no_more_delimiters;
    }
    while (1)
    {
        char *next_delimiter = strstr(current_position, delimiter);

        if (next_delimiter == NULL)
        {
            goto no_more_delimiters;
        }

        size_t token_length = next_delimiter - current_position;
        (*ret_tokens)[(*ret_token_count)] = malloc((1 + token_length) * sizeof(*current_position));
        memcpy((*ret_tokens)[(*ret_token_count)], current_position, token_length);
        (*ret_tokens)[(*ret_token_count)][token_length] = '\0';
        (*ret_token_count)++;

        if ((*ret_token_count) >= tokens_size)
        {
            tokens_size <<= 1;
            (*ret_tokens) = realloc((*ret_tokens), tokens_size * sizeof(**ret_tokens));
        }

        current_position = next_delimiter + delimiter_length;
    }

no_more_delimiters:
    (*ret_tokens)[*ret_token_count] = strdup(current_position);
    (*ret_token_count)++;

    (*ret_tokens) = realloc((*ret_tokens), (1 + (*ret_token_count)) * sizeof(**ret_tokens));
    (*ret_tokens)[(*ret_token_count)] = NULL;
}