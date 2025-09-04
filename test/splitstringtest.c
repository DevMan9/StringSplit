#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "StringSplit-StandardContract/splitstring.x"

__attribute__((unused)) static void PrintStrings(char **strings)
{
    printf("{");
    for (char **string = strings; *string != NULL; string++)
    {
        printf("\"%s\"", *string);
        if (*(string + 1) != NULL)
        {
            printf(", ");
        }
    }
    printf("}\n");
}

static void _CountStrings(char **strings, size_t *ret)
{
    size_t count = 0;
    for (char **string = strings; *string != NULL; string++)
    {
        count++;
    }
    (*ret) = count;
}

int Test(char **output, char **expected)
{
    size_t output_count = 0;
    size_t expected_count = 0;
    _CountStrings(output, &output_count);
    _CountStrings(expected, &expected_count);
    if (output_count != expected_count)
    {
        return 0;
    }

    for (size_t i = 0; i < output_count; i++)
    {
        if (strcmp(output[i], expected[i]) != 0)
        {
            return 0;
        }
    }

    return 1;
}

int main()
{
    char *string_1 = "This.astring.abhas.abcmultiple.adelimiters.";
    char *delimiters_1[] = {".a", ".ab", ".abc", NULL};

    char **tokens = NULL;
    size_t token_count;
    SplitString(string_1, delimiters_1, NONE, &tokens, &token_count);
    char *expected_1_1[] = {"This", "string", "bhas", "bcmultiple", "delimiters.", NULL};
    assert(Test(tokens, expected_1_1));
    assert(token_count == 5);
    DestroyReturnTokens(&tokens);

    SplitString(string_1, delimiters_1, LONGEST_FIRST, &tokens, &token_count);
    char *expected_1_2[] = {"This", "string", "has", "multiple", "delimiters.", NULL};
    assert(Test(tokens, expected_1_2));
    assert(token_count == 5);
    DestroyReturnTokens(&tokens);

    char *string_2 = "This string  is    testing the    filter  feature.    ";
    char *delimiters_2[] = {" ", NULL};
    SplitString(string_2, delimiters_2, NONE, &tokens, &token_count);
    char *expected_2_1[] = {"This", "string", "", "is", "", "", "", "testing", "the", "", "", "", "filter", "", "feature.", "", "", "", "", NULL};
    assert(Test(tokens, expected_2_1));
    assert(token_count == 19);
    DestroyReturnTokens(&tokens);

    SplitString(string_2, delimiters_2, FILTER_EMPTIES, &tokens, &token_count);
    char *expected_2_2[] = {"This", "string", "is", "testing", "the", "filter", "feature.", NULL};
    assert(Test(tokens, expected_2_2));
    assert(token_count == 7);
    DestroyReturnTokens(&tokens);

    char *string_3 = "This string   \t is \n\n tasked  \t\n\t with   \t\t removing \n\n\n ALL    \t  this whitespace.";
    char *delimiters_3[] = {" ", "\t", "\n", NULL};
    SplitString(string_3, delimiters_3, NONE, &tokens, NULL);
    char *expected_3_1[] = {"This", "string", "", "", "", "", "is", "", "", "", "tasked", "", "", "", "", "", "with", "", "", "", "", "", "removing", "", "", "", "", "ALL", "", "", "", "", "", "", "this", "whitespace.", NULL};
    assert(Test(tokens, expected_3_1));
    DestroyReturnTokens(&tokens);

    SplitString(string_3, delimiters_3, FILTER_EMPTIES, &tokens, NULL);
    char *expected_3_2[] = {"This", "string", "is", "tasked", "with", "removing", "ALL", "this", "whitespace.", NULL};
    assert(Test(tokens, expected_3_2));
    DestroyReturnTokens(&tokens);

    char *string_4 = "";
    char *delimiters_4[] = {" ", "\t", "\n", NULL};
    SplitString(string_4, delimiters_4, NONE, &tokens, &token_count);
    char *expected_4_1[] = {"", NULL};
    assert(Test(tokens, expected_4_1));
    assert(token_count == 1);
    DestroyReturnTokens(&tokens);

    SplitString(string_4, delimiters_4, FILTER_EMPTIES, &tokens, &token_count);
    char *expected_4_2[] = {NULL};
    assert(Test(tokens, expected_4_2));
    assert(token_count == 0);
    DestroyReturnTokens(&tokens);

    char *string_5 = "No delimiters.";
    char *delimiters_5[] = {NULL};
    SplitString(string_5, delimiters_5, NONE, &tokens, &token_count);
    char *expected_5_1[] = {"No delimiters.", NULL};
    assert(Test(tokens, expected_5_1));
    assert(token_count == 1);
    DestroyReturnTokens(&tokens);

    printf("All tests passed!.\n");
    return 0;
}