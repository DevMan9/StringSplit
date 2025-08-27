#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../source/splitstring.x"

int main()
{
    char test_string1[] = "There once was an ugly barnacle. He was so ugly that everyone died. The End.";
    char test_delimiter1[] = " ";
    char *expected[] = {"There", "once", "was", "an", "ugly", "barnacle.", "He", "was", "so", "ugly", "that", "everyone", "died.", "The", "End."};

    printf("Splitting string.\n");
    char **splits = NULL;
    size_t count = 0;
    SplitString(test_string1, test_delimiter1, &splits, &count);

    printf("Testing count.\n");
    assert(count == 15);

    printf("Testing splits.\n");
    size_t index = 0;
    for (char **word = splits; *word != NULL; word++, index++)
    {
        assert(strcmp(*word, expected[index]) == 0);
        free(*word);
    }

    printf("Testing index.\n");
    assert(index == 15);
    free(splits);

    printf("No delimiter in string test.\n");
    char test_delimiter2[] = "qqq";
    SplitString(test_string1, test_delimiter2, &splits, &count);
    assert(count == 1);
    assert(strcmp(splits[0], test_string1) == 0);
    assert(splits[1] == NULL);
    free(splits[0]);
    free(splits);

    printf("string == delimiter test.\n");
    char test_string2[] = "cat";
    char test_delimiter3[] = "cat";
    SplitString(test_string2, test_delimiter3, &splits, &count);
    assert(count == 2);
    assert(strcmp(splits[0], "") == 0);
    assert(strcmp(splits[1], "") == 0);
    for (char **word = splits; *word != NULL; word++)
    {
        free(*word);
    }
    free(splits);

    printf("Empty delimiter test.\n");
    char test_string3[] = "Empty delimiter string";
    char test_delimiter4[] = "";
    SplitString(test_string3, test_delimiter4, &splits, &count);
    assert(count == 1);
    assert(strcmp(splits[0], test_string3) == 0);
    free(splits[0]);
    free(splits);

    printf("Empty string test.\n");
    char test_string4[] = "";
    char test_delimiter5[] = "Empty string";
    SplitString(test_string4, test_delimiter5, &splits, &count);
    assert(count == 1);
    assert(strcmp(splits[0], test_string4) == 0);
    free(splits[0]);
    free(splits);

    printf("Empty empty test.\n");
    char test_string5[] = "";
    char test_delimiter6[] = "";
    SplitString(test_string5, test_delimiter6, &splits, &count);
    assert(count == 1);
    assert(strcmp(splits[0], test_string5) == 0);
    free(splits[0]);
    free(splits);

    printf("All tests passed!.\n");
    return 0;
}