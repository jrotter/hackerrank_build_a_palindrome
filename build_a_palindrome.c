#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXSTRING 100001

void print_array(int array[],int size)
{
  int i;

  for (i=0;i<size;i++) {
    printf("%d ",array[i]);
  }
  printf("\n\n");
}


int max (int a, int b) 
{
  if (a > b)
    return a;
  else
    return b;
}


int palindrome(char *str, int length) {
  int a = 0;
  int b = length - 1;

  while (b > a) {
    if (str[a++] != str[b--]) {
      return 0;
    }
  }
  return 1;  
}


/* Find the length of all longest palindromes beginning at s[x] */
void find_palindromes_left(char *s, int length, int results[])
{
  char hold;
  int x, y;

  for (x=0;x<length;x++) {
    results[x] = 1;
  }
  for (x=0;x<length;x++) {
    for (y=length-1;y>x;y--) {
      if (s[y] == s[x]) {
        hold = s[y+1];
        s[y+1] = '\0'; /* Temporarily end the string after y */
        if (palindrome(&(s[x]),y-x+1)) {
          s[y+1] = hold; /* Undo the temporary end-of-string */
          results[x] = y-x+1;
          break;
        }
        else {
          s[y+1] = hold;
        }
      }
    }
  }
}


/* Find the length of the longest palindrome ending at s[x] */
void find_palindromes_right(char *s, int length, int results[])
{
  char hold;
  int x, y;

  for (x=0;x<length;x++) {
    results[x] = 1;
  }
  for (y=length-1;y>0;y--) {
    for (x=0;x<y;x++) {
      if (s[y] == s[x]) {
        hold = s[y+1];
        s[y+1] = '\0'; /* Temporarily end the string after y */
        if (palindrome(&(s[x]),y-x+1)) {
          s[y+1] = hold; /* Undo the temporary end-of-string */
          results[y] = y-x+1;
          break;
        }
        else {
          s[y+1] = hold;
        }
      }
    }
  }
}


/* Return the length of the maximal bookend ending at s1[x] that has a match in s2 */
int find_s1_bookend_length(int x, char *s1, char *s2, char *buffer, int startlength)
{
  char *searchstring = buffer;
  int i = x;
  int j = 0;

  while (j < startlength) {
    searchstring[j++] = s1[i--];
  }
  searchstring[j+1] = '\0';
  
  while (i >= 0) {
    searchstring[j] = s1[i];
    searchstring[j+1] = '\0';
    //printf("[S1] x == %d, i == %d, j == %d, last == %d, searchstring == %s\n",x,i,j,startlength,searchstring);
    if (strstr(s2,searchstring)) {
      j++;
      i--;
    }
    else {
      //printf("x == %d: returning %d\n",x,j);
      return j;
    }
  }
  //printf("x == %d: returning %d\n",x,j);
  return j;
}


/* Return the length of the maximal bookend starting at s2[x] that has a match in s1 */
int find_s2_bookend_length(int x, char *s1, char *s2, char *buffer, int startlength)
{
  char *searchstring = buffer;
  int i = x;
  int j = MAXSTRING;

  searchstring[j--] = '\0';
  while ((MAXSTRING - 1 - j) < startlength) {
    searchstring[j--] = s2[i++];
  }
  
  while (s2[i] != '\0') {
    searchstring[j] = s2[i];
    //printf("[S2] x == %d, i == %d, j == %d, last == %d, searchstring == %s\n",x,i,j,startlength,&(searchstring[j]));
    if (strstr(s1,&(searchstring[j]))) {
      j--;
      i++;
    }
    else {
      //printf("x == %d: returning %d\n",x,i-x);
      return i-x;
    }
  }
  //printf("x == %d: returning %d\n",x,i-x);
  return i-x;
}


// Find all bookend lengths
find_bookend_lengths(
  char s1[],
  int s1_length,
  int s1_bookend_length[],
  char s2[],
  int s2_length,
  int s2_bookend_length[],
  char tempbuffer[])
{
  int x, last;

  /* Find the lengths of all left bookends */
  last = 1;
  for (x=s1_length-1;x>=0;x--) {
    last = find_s1_bookend_length(x,s1,s2,tempbuffer,last-1);
    s1_bookend_length[x] = last;
  }
  //printf("S1 Bookends:    ");
  //print_array(s1_bookend_length,s1_length);

  /* Find the lengths of all right bookends */
  last = 1;
  for (x=0;x<s2_length;x++) {
    last = find_s2_bookend_length(x,s1,s2,tempbuffer,last-1);
    s2_bookend_length[x] = last;
  }
  //printf("S2 Bookends:    "); 
  //print_array(s2_bookend_length,s2_length); 
}


/* Return the length of the palindrome formed by bookends at the end of s1 and beginning of s2 */
int find_trivial_bookend_palindrome(char *s1, int s1_length, char *s2, int s2_length)
{
  int i = s1_length-1;
  int j = 0;

  while ((s1[i] == s2[j]) && (i >= 0) && (j < s2_length)) {
    i--;
    j++;
  }
  //printf("Trival palindrome length == %d\n",j*2);
  return j*2;
}


void save_if_best(char *candidate, char *best)
{
  /* Note that we assume here that candidate_length == best_length */
  if ((best[0] == '\0') || (strcmp(candidate,best) < 0))
  {
    //printf("A NEW BEST! \"%s\"\n",candidate);
    strcpy(best,candidate);
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


// Find the maximum palindrome length
int max_palindrome_length(
      int s1_length,
      int s1_bookend_length[],
      int s1_palindromes[],
      int s2_length,
      int s2_bookend_length[],
      int s2_palindromes[])
{
  int best_length;
  int x;

  // Prime it with the best length of the two bookends
  best_length = max(s1_bookend_length[s1_length-1],s2_bookend_length[0])*2;

  // Body from S1
  for (x=1;x<s1_length;x++) {
    if (s1_bookend_length[x-1]) {
      best_length = max(best_length,s1_bookend_length[x-1]*2+s1_palindromes[x]);
    }
  }

  // Body from S2
  for (x=0;x<s2_length-1;x++) {
    if (s2_bookend_length[x+1]) {
      best_length = max(best_length,s2_bookend_length[x+1]*2+s2_palindromes[x]);
    }
  }
  return(best_length);
}


void find_best_palindrome(
  char s1[],
  int s1_length,
  char s2[],
  int s2_length,
  int best_length,
  int s1_bookend_length[],
  int s2_bookend_length[],
  int s1_palindromes[],
  int s2_palindromes[],
  char best[],
  char teststring[])
{
  int x;
  int bookend_length;
  int total_bookend_length;
  int body_length;

  // First deal with the bookends-only scenario
  if (find_trivial_bookend_palindrome(s1,s1_length,s2,s2_length) == best_length) {
    bookend_length = s1_bookend_length[s1_length-1];
    strncpy(teststring,&(s1[s1_length-bookend_length]),bookend_length);
    reverse_string(&(teststring[bookend_length]),&(s1[s1_length-bookend_length]),bookend_length);
    //printf("Considering teststring (bookends only): \"%s\"\n",teststring);
    save_if_best(teststring,best);
  }

  // Next look at strings including a body from s1
  for (x=1;x<s1_length;x++) {
    if (s1_bookend_length[x-1]*2+s1_palindromes[x] == best_length) {
      bookend_length = s1_bookend_length[x-1];
      body_length = s1_palindromes[x];
      strncpy(teststring,&(s1[x-bookend_length]),bookend_length+body_length);
      reverse_string(&(teststring[bookend_length+body_length]),&(s1[x-bookend_length]),bookend_length);
      //printf("Considering teststring (body from s1, x=%2d): \"%s\"\n",x,teststring);
      save_if_best(teststring,best);
    }
  }
  // Next look at strings including a body from s2
  for (x=0;x<s2_length-1;x++) {
    if (s2_bookend_length[x+1]*2+s2_palindromes[x] == best_length) {
      bookend_length = s2_bookend_length[x+1];
      body_length = s2_palindromes[x];
      reverse_string(teststring,&(s2[x+1]),bookend_length);
      strncpy(&(teststring[bookend_length]),&(s2[x+1-body_length]),body_length+bookend_length);
      teststring[best_length] = '\0';
      //printf("Considering teststring (body from s2, x=%2d): \"%s\"\n",x,teststring);
      save_if_best(teststring,best);
    }
  }
}


int main() {
  int params;
  char s1[MAXSTRING];
  char s2[MAXSTRING];
  int s1_palindromes[MAXSTRING];
  int s2_palindromes[MAXSTRING];
  int s1_length;
  int s2_length;
  char tempbuffer[MAXSTRING+MAXSTRING];

  int s1_bookend_length[MAXSTRING];
  int s2_bookend_length[MAXSTRING];

  char best[MAXSTRING+MAXSTRING];
  int best_length;
  char *teststring = tempbuffer;
  int teststring_length;

  int i; /* Temporary variable */
  int n, x; /* Loop counters */
  int last;

  /////////////////////////////////////////////////////////////////////////////
  // Read in the number of pairs we'll consider
  /////////////////////////////////////////////////////////////////////////////

  i = scanf("%i",&params);

  for (n=0;n<params;n++)
  {
    ///////////////////////////////////////////////////////////////////////////
    // Read in the pair of strings
    ///////////////////////////////////////////////////////////////////////////

    i = scanf("%s",s1);
    i = scanf("%s",s2);
    s1_length = strlen(s1);
    s2_length = strlen(s2);
    //printf("s1 == \"%s\"\n",s1);
    //printf("s1 length == \"%d\"\n",s1_length);
    //printf("s2 == \"%s\"\n",s2);
    //printf("s2 length == \"%d\"\n",s2_length);

    /* Initialize our best result */
    best[0] = '\0';
    best_length = 0;

    ///////////////////////////////////////////////////////////////////////////
    // Find all palindromes in s1 (left-indexed) and s2 (right-indexed)
    ///////////////////////////////////////////////////////////////////////////

    find_palindromes_left(s1,s1_length,s1_palindromes);
    //printf("S1 Palindromes: ");
    //print_array(s1_palindromes,strlen(s1));
    find_palindromes_right(s2,s2_length,s2_palindromes);
    //printf("S2 Palindromes: ");
    //print_array(s2_palindromes,strlen(s2));

    ///////////////////////////////////////////////////////////////////////////
    // Find the lengths of all bookends
    ///////////////////////////////////////////////////////////////////////////

    find_bookend_lengths(
      s1,
      s1_length,
      s1_bookend_length,
      s2,
      s2_length,
      s2_bookend_length,
      tempbuffer);

    ///////////////////////////////////////////////////////////////////////////
    // Determine the maximum palindrome length
    ///////////////////////////////////////////////////////////////////////////

    best_length = max_palindrome_length(
      s1_length,
      s1_bookend_length,
      s1_palindromes,
      s2_length,
      s2_bookend_length,
      s2_palindromes);
    //printf("best_length == %d\n",best_length);

    ///////////////////////////////////////////////////////////////////////////
    // We know the maximum length: Now find the best palindrome
    ///////////////////////////////////////////////////////////////////////////

    if (best_length < 2) {
      printf("-1\n");
    }
    else {
      find_best_palindrome(
        s1,
        s1_length,
        s2,
        s2_length,
        best_length,
        s1_bookend_length,
        s2_bookend_length,
        s1_palindromes,
        s2_palindromes,
        best,
        teststring);
      printf("%s\n",best);
    }
  }
  exit(0);
}

