#include <stdio.h>
#include <string.h>
 
void main()
{
    char *s = "this has 3 words in it\n but now 9";
    int count = 0, i;
 
       for (i = 0;s[i] != '\0';i++)
    {
        if (s[i] == ' ' && s[i+1] != ' ')
            count++;    
    }
    printf("Number of words in given string are: %d\n", count + 1);
}