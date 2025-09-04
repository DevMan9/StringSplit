#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "StringSplit-StandardContract/splitstring.x"

int DescendingStringCompare(const void *a, const void *b)
{
    char *a_s = *((char **)a);
    char *b_s = *((char **)b);
    return strcmp(b_s, a_s);
}

static inline void _CountDelimiters(char **delimiters, size_t *ret)
{
    (*ret) = 0;
    for (char **delimiter = delimiters; *delimiter != NULL; delimiter++)
    {
        (*ret)++;
    }
}

static inline void _SortDelimiters(char **delimiters, size_t delimiter_count)
{
    qsort(delimiters, delimiter_count, sizeof(*delimiters), DescendingStringCompare);
}

void _DEBUGPrintTokens(char **tokens)
{
    for (char **token = tokens; *token != NULL; token++)
    {
        printf("%s\n", *token);
    }
}

static inline void _FindNextDelimiter(char *string, char **delimiters, char **ret_location, char **ret_matched_delimiter)
{
    char *earliest_delimiter = NULL;
    char *matched_delimiter = NULL;

    // Iterate the list of delimiters.
    for (char **delimiter = delimiters; *delimiter != NULL; delimiter++)
    {
        // Ignore "" delimiters.
        if ((*delimiter)[0] == '\0')
        {
            continue;
        }
        // Find the next occurance of this delimiter.
        char *found_delimiter = strstr(string, *delimiter);
        if (found_delimiter == NULL)
        {
            // Delimiter not found.
            continue;
        }
        // Record this delimiter and where it was found.
        if (earliest_delimiter == NULL)
        {
            earliest_delimiter = found_delimiter;
            matched_delimiter = *delimiter;
        }
        else
        {
            // We only care about the earliest found delimiter.
            if (found_delimiter < earliest_delimiter)
            {
                earliest_delimiter = found_delimiter;
                matched_delimiter = *delimiter;
            }
        }
    }

    // Set return values.
    (*ret_location) = earliest_delimiter;
    (*ret_matched_delimiter) = matched_delimiter;
}

static inline void _RipString(char *string, char *delimiter_location, char **ripped_string)
{
    size_t ripped_string_length = delimiter_location - string;
    (*ripped_string) = malloc(1 + ripped_string_length);
    memcpy((*ripped_string), string, ripped_string_length);
    (*ripped_string)[ripped_string_length] = '\0';
}

static inline void _Grow(char ***tokens, size_t *token_count, size_t *tokens_size)
{
    if ((*token_count) >= (*tokens_size))
    {
        (*tokens_size) <<= 1;
        (*tokens) = realloc((*tokens), (*tokens_size) * sizeof(**tokens));
    }
}

static inline void _FilterEmpties(char ***tokens, size_t *token_count)
{
    // Create a new string array the same size as tokens.
    char **filtered_tokens = malloc((1 + (*token_count)) * sizeof(*filtered_tokens));
    size_t filtered_token_count = 0;

    // Iterate tokens.
    for (char **token = *tokens; *token != NULL; token++)
    {
        if ((*token)[0] != '\0')
        {
            // Only keep non-empty strings.
            filtered_tokens[filtered_token_count] = *token;
            filtered_token_count++;
        }
        else
        {
            // free empty strings.
            free(*token);
        }
    }

    // Resize to fit data.
    filtered_tokens = realloc(filtered_tokens, (1 + filtered_token_count) * sizeof(*filtered_tokens));
    // Null terminate
    filtered_tokens[filtered_token_count] = NULL;
    // Free the original tokens.
    free(*tokens);
    // Set return values.
    *tokens = filtered_tokens;
    *token_count = filtered_token_count;
}

void SplitString(char *string, char **delimiters, int flags, char ***ret_tokens, size_t *ret_token_count)
{
    // ret_token_count is nullable
    // prepare a backup.
    size_t token_count_fallback = 0;
    if (ret_token_count == NULL)
    {
        ret_token_count = &token_count_fallback;
    }

    // Initialize return variables
    (*ret_token_count) = 0;
    size_t tokens_size = 16;
    (*ret_tokens) = malloc(tokens_size * sizeof(**ret_tokens));

    // Count delimiters
    size_t delimiter_count = 0;
    _CountDelimiters(delimiters, &delimiter_count);

    if (flags & LONGEST_FIRST)
    {
        // Sorting the delimiters in descending order ensures that ties are broken in favor of longer delimiters.
        _SortDelimiters(delimiters, delimiter_count);
    }

    // Loop throught the given string, cutting off pieces as delimiters are found.
    char *remaining_string = string;
    while (1)
    {
        // Find the next delimiter.
        char *next_delimiter_location = NULL;
        char *next_delimiter = NULL;
        _FindNextDelimiter(remaining_string, delimiters, &next_delimiter_location, &next_delimiter);
        if (next_delimiter_location == NULL)
        {
            break;
        }

        // Rip a copy of the delimited string from the give string.
        char *ripped_string = NULL;
        _RipString(remaining_string, next_delimiter_location, &ripped_string);

        // Store the copy of the substring in the ret_tokens.
        (*ret_tokens)[(*ret_token_count)] = ripped_string;
        (*ret_token_count)++;
        // If we need more space, grow ret_tokens.
        _Grow(ret_tokens, ret_token_count, &tokens_size);

        // Move past the current delimiter.
        size_t delimiter_length = strlen(next_delimiter);
        remaining_string = next_delimiter_location + delimiter_length;
    }

    // Rip the last substring
    (*ret_tokens)[(*ret_token_count)] = strdup(remaining_string);
    (*ret_token_count)++;
    // If we need more space, grow ret_tokens.
    _Grow(ret_tokens, ret_token_count, &tokens_size);

    // Null terminate ret_tokens.
    (*ret_tokens)[(*ret_token_count)] = NULL;
    // Size ret_tokens to fit the data.
    (*ret_tokens) = realloc((*ret_tokens), (1 + (*ret_token_count)) * sizeof(**ret_tokens));

    if (flags & FILTER_EMPTIES)
    {
        _FilterEmpties(ret_tokens, ret_token_count);
    }
}

void DestroyReturnTokens(char ***ret_tokens)
{
    for (char **token = *ret_tokens; *token != NULL; token++)
    {
        free(*token);
    }
    free(*ret_tokens);
    *ret_tokens = NULL;
}