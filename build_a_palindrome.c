#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXSTRING 10001

void print_array(int array[],int size)
{
  int i;

  for (i=0;i<size;i++)
  {
    printf("%d ",array[i]);
  }
  printf("\n\n");
}


int palindrome(char *str, int length) {
  int a = 0;
  int b = length - 1;

  while (b > a) 
  {
    if (str[a++] != str[b--])
    {
      return 0;
    }
  }
  return 1;  
}


void find_palindromes_left(char *s, int length, int results[])
{
  char hold;
  int x, y;
  char *searchptr = s;
  char *nextmatch;

  for (x=0;x<length;x++)
  {
    results[x] = 1;
  }
  for (x=0;x<length;x++)
  {
    for (y=length-1;y>x;y--)
    {
      /* printf("(x,y) == (%d,%d)\n",x,y); */
      if (s[y] == s[x]) 
      {
        hold = s[y+1];
        s[y+1] = '\0'; /* Temporarily end the string after y */
        /* printf("  Testing \"%s\"\n",&(s[x])); */
        if (palindrome(&(s[x]),y-x+1))
        {
          /* printf("  %s is a palindrome\n",&(s[x])); */
          s[y+1] = hold; /* Undo the temporary end-of-string */
          results[x] = y-x+1;
          break;
        }
        else
        {
          s[y+1] = hold;
        }
      }
    }
  }
}


void find_palindromes_right(char *s, int length, int results[])
{
  char hold;
  int x, y;
  char *searchptr = s;
  char *nextmatch;

  for (x=0;x<length;x++)
  {
    results[x] = 1;
  }
  for (y=length-1;y>0;y--)
  {
    for (x=0;x<y;x++)
    {
      /* printf("(x,y) == (%d,%d)\n",x,y); */
      if (s[y] == s[x]) 
      {
        hold = s[y+1];
        s[y+1] = '\0'; /* Temporarily end the string after y */
        /* printf("  Testing \"%s\"\n",&(s[x])); */
        if (palindrome(&(s[x]),y-x+1))
        {
          /* printf("  %s is a palindrome\n",&(s[x])); */
          s[y+1] = hold; /* Undo the temporary end-of-string */
          results[y] = y-x+1;
          break;
        }
        else
        {
          s[y+1] = hold;
        }
      }
    }
  }
}


void main() {
  int params;
  char s1[MAXSTRING];
  char s2[MAXSTRING];
  int s1_palindromes[MAXSTRING];
  int s2_palindromes[MAXSTRING];
  char s1_substring[MAXSTRING];
  int s1_length;
  char s2_substring[MAXSTRING];
  int s2_length;
  char left_bookend[MAXSTRING];
  char right_bookend[MAXSTRING];
  char body[MAXSTRING];

  char best[MAXSTRING+MAXSTRING];
  int bestlength;

  int n, x, y, a, b; /* Loop counters */

  /* Read in the number of pairs we'll consider */
  scanf("%i",&params);

  for (n=0;n<params;n++)
  {
    /* Read in the pair */
    scanf("%s",s1);
    scanf("%s",s2);
    s1_length = strlen(s1);
    s2_length = strlen(s2);

    /* Initialize our best result */
    best[0] = '\0';
    bestlength = 0;

    /* Find all palindromes in s1 (left-indexed) and s2 (right-indexed) */
    find_palindromes_left(s1,s1_length,s1_palindromes); 
    print_array(s1_palindromes,strlen(s1));
    find_palindromes_right(s2,s2_length,s2_palindromes); 
    print_array(s2_palindromes,strlen(s2));

    for (x=0;x<s1_length;x++)
    {
      if (bestlength <= (s1_length - x + s2_length))
      {
        for (y=x;y<s1_length;y++)
        {
          strncpy(left_bookend,&(s1[x]),y-x+1);
          left_bookend[y-x+1] = '\0';
          printf("Left bookend (%d,%d) = \"%s\"\n",x,y,left_bookend);
        }
      }
    }
  }
}

#ifdef EXTRA
    if test_string.length > @@length ||
      ((test_string.length == @@length) && (test_string < @@value))
      @@value = test_string
      @@length = test_string.length
    end
#endif
