#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXSTRING 10001

void print_array(int array[],int size)
{
  int i;

  for (i=0;i<size;i++)
  {
    //printf("%d ",array[i]);
  }
  //printf("\n\n");
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

  for (x=0;x<length;x++)
  {
    results[x] = 1;
  }
  for (x=0;x<length;x++)
  {
    for (y=length-1;y>x;y--)
    {
      if (s[y] == s[x]) 
      {
        hold = s[y+1];
        s[y+1] = '\0'; /* Temporarily end the string after y */
        if (palindrome(&(s[x]),y-x+1))
        {
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

  for (x=0;x<length;x++)
  {
    results[x] = 1;
  }
  for (y=length-1;y>0;y--)
  {
    for (x=0;x<y;x++)
    {
      if (s[y] == s[x]) 
      {
        hold = s[y+1];
        s[y+1] = '\0'; /* Temporarily end the string after y */
        if (palindrome(&(s[x]),y-x+1))
        {
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


void save_if_best(
  char *candidate, 
  int candidate_length, 
  char *best, 
  int *best_length)
{
  /* Note that we assume here that candidate_length >= (*best_length) */
  if ((candidate_length > (*best_length)) || 
      (strcmp(candidate,best) < 0))
  {
    //printf("    A NEW BEST!\n");
    strcpy(best,candidate);
    (*best_length) = candidate_length;
  }
}


void reverse_string(char *target, char *source, int length)
{
  int i; /* loop counter */

  for(i=0;i<length;i++)
  {
    target[length-i-1] = source[i];
  }
  target[length] = '\0';
}


int main() {
  int params;
  char s1[MAXSTRING];
  char s2[MAXSTRING];
  int s1_palindromes[MAXSTRING];
  int s2_palindromes[MAXSTRING];
  int s1_length;
  int s2_length;

  char left_bookend[MAXSTRING];
  char right_bookend[MAXSTRING];
  int bookend_length;
  int total_bookend_length;
  int body_length;

  char best[MAXSTRING+MAXSTRING];
  int best_length;
  char teststring[MAXSTRING+MAXSTRING];
  int teststring_length;

  int i, n, x, y, a, b; /* Loop counters */

  /* Read in the number of pairs we'll consider */
  i = scanf("%i",&params);

  for (n=0;n<params;n++)
  {
    /* Read in the pair */
    i = scanf("%s",s1);
    i = scanf("%s",s2);
    s1_length = strlen(s1);
    s2_length = strlen(s2);
    printf("s1 == \"%s\"\n",s1);
    printf("s2 == \"%s\"\n",s2);

    /* Initialize our best result */
    best[0] = '\0';
    best_length = 0;

    /* Find all palindromes in s1 (left-indexed) and s2 (right-indexed) */
    find_palindromes_left(s1,s1_length,s1_palindromes); 
    print_array(s1_palindromes,strlen(s1));
    find_palindromes_right(s2,s2_length,s2_palindromes); 
    print_array(s2_palindromes,strlen(s2));

    for (x=0;x<s1_length;x++)
    {
      if (best_length <= (s1_length - x + s2_length))
      {
        for (y=x;y<s1_length;y++)
        {
          /* Set up the bookend data */
          bookend_length = y-x+1;
          total_bookend_length = bookend_length * 2;
          strncpy(left_bookend,&(s1[x]),bookend_length);
          left_bookend[bookend_length] = '\0';
          //printf("Left bookend (%d,%d) = \"%s\"\n",x,y,left_bookend);
          reverse_string(right_bookend,left_bookend,bookend_length);

          /* See if any right bookends are found */
          if (strstr(s2,right_bookend))
          {
            //printf("  Right bookend = \"%s\"\n",right_bookend);

            /* Try the body from s1 */
            if (y != (s1_length-1))
            {
              body_length = s1_palindromes[y+1];
  
              /* Ignore this body if the resulting string would be too small */
              if ((total_bookend_length + body_length) >= best_length)
              {
                /* Build the test string */
                teststring_length = total_bookend_length + body_length;
                strcpy(teststring,left_bookend);
                strncpy(&(teststring[bookend_length]),&(s1[y+1]),body_length);
                strncpy(&(teststring[bookend_length+body_length]),right_bookend,bookend_length);
                teststring[teststring_length] = '\0';
                //printf("    Teststring (body from s1): \"%s\"\n",teststring);
  
                /* Compare test string with the best we've seen */
                save_if_best(teststring,teststring_length,best,&best_length);
              }
            }
            else /* No body available from s1 - try just the bookends */
            {
              /* Ignore this body if the resulting string would be too small */
              if (total_bookend_length >= best_length)
              {
                /* Build the test string */
                teststring_length = total_bookend_length;
                strcpy(teststring,left_bookend);
                strncpy(&(teststring[bookend_length]),right_bookend,bookend_length);
                teststring[teststring_length] = '\0';
                //printf("    Teststring (no body): \"%s\"\n",teststring);
  
                /* Compare test string with the best we've seen */
                save_if_best(teststring,teststring_length,best,&best_length);
              }
            }
          }
          else /* No right bookend was found */
          {
            /* If a right bookend was not found for x at a given length
               nothing will be found with a greater length */
            break;
          }
        }
      }
    }  /* for */

    for (b=s2_length-1;b>=0;b--)
    {
      if (best_length <= (s1_length + b + 1))
      {
        for (a=b;a>=0;a--)
        {
          /* Set up the bookend data */
          bookend_length = b-a+1;
          total_bookend_length = bookend_length * 2;
          strncpy(right_bookend,&(s2[a]),bookend_length);
          right_bookend[bookend_length] = '\0';
          //printf("Right bookend (%d,%d) = \"%s\"\n",a,b,right_bookend);
          reverse_string(left_bookend,right_bookend,bookend_length);

          /* See if any left bookends are found */
          if (strstr(s1,left_bookend))
          {
            //printf("  Left bookend = \"%s\"\n",left_bookend);

            /* Try the body from s2 */
            if (a != 0)
            {
              body_length = s2_palindromes[a-1];
  
              /* Ignore this body if the resulting string would be too small */
              if ((total_bookend_length + body_length) >= best_length)
              {
                /* Build the test string */
                teststring_length = total_bookend_length + body_length;
                strcpy(teststring,left_bookend);
                strncpy(&(teststring[bookend_length]),&(s2[a-body_length]),body_length);
                strncpy(&(teststring[bookend_length+body_length]),right_bookend,bookend_length);
                teststring[teststring_length] = '\0';
                //printf("    Teststring (body from s2): \"%s\"\n",teststring);
  
                /* Compare test string with the best we've seen */
                save_if_best(teststring,teststring_length,best,&best_length);
              }
            }
            else /* No body available from s2 - try just the bookends */
            {
              /* Ignore this body if the resulting string would be too small */
              if (total_bookend_length >= best_length)
              {
                /* Build the test string */
                teststring_length = total_bookend_length;
                strcpy(teststring,left_bookend);
                strncpy(&(teststring[bookend_length]),right_bookend,bookend_length);
                teststring[teststring_length] = '\0';
                //printf("    Teststring (no body): \"%s\"\n",teststring);
  
                /* Compare test string with the best we've seen */
                save_if_best(teststring,teststring_length,best,&best_length);
              }
            }
          }
          else /* No left bookend was found */
          {
            /* If a left bookend was not found for b at a given length
               nothing will be found with a greater length */
            break;
          }
        }
      }
    }  /* for */
    if (best_length)
      printf("%s\n",best);
    else
      printf("-1\n");
  }
}

