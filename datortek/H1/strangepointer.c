/* strangepointer.c - Test pointers
 *
 * Copyright (C) 2009 J Wennlund and F Lundevall
 *
 * This program assumes 32-bit integers, so it is likely to fail
 * on 64-bit machines such as Itanium, Athlon64 and others.
 */

#include <stdio.h>
#include <stdlib.h>

/* memory allocated at compile time, in global area */
char * ptr1; /* ptr1 is now a pointer to char */
int val1;
void * mp;
void * vp;
char array1[ 17 ]; /* array1 is now an array of char */

int main ()         /* called as a method/function/subroutine */
{
  int val2;
  int * ptr2;

  val1 = 4711;
  val2 = 222222;
  ptr1 = &val1;

  array1[ 0 ] = 11;
  array1[ 1 ] = 17;
  array1[ 16 ] = 53; /* last element */
  
  printf ("Message SP.01 from strangepointer.c: Hello World!\n");

  printf ("SP.02: val1: stored at %08x (hex); value is %d (dec), %08x (hex)\n",
          (int) &val1, val1, val1);
  printf ("SP.03: val2: stored at %08x (hex); value is %d (dec), %08x (hex)\n",
          (int) &val2, val2, val2);

  printf ("SP.04: ptr1: stored at %08x (hex); value is %d (dec), %08x (hex)\n",
          (int) &ptr1, (int) ptr1, (int) ptr1);
  printf ("SP.05: Dereference pointer ptr1 and we find: %d \n", *ptr1);

  *ptr1 = 1234; /* dereference pointer ptr1 and modify */
  printf ("\nSP.06: Executed *ptr1 = 1234; \n");
  printf ("SP.07: ptr1: stored at %08x (hex); value is %d (dec), %08x (hex)\n",
          (int) &ptr1, (int) ptr1, (int) ptr1);
  printf ("SP.08: Dereference pointer ptr1 again, we find: %d \n", *ptr1);
  printf ("SP.09: val1: stored at %08x (hex); value is %d (dec), %08x (hex)\n",
          (int) &val1, val1, val1);

  ptr1 = &val2; /* assign new value to pointer variable ptr1 */
  printf ("\nSP.10: Executed ptr1 = &val2; \n");
  printf ("SP.11: ptr1: stored at %08x (hex); value is %d (dec), %08x (hex)\n",
          (int) &ptr1, (int) ptr1, (int) ptr1);
  printf ("SP.12: Dereference pointer ptr1 once more to find: %d \n", *ptr1);

  printf("SP.13: array1[0]: stored at %08x (hex); value is %d (dec), %08x (hex)\n",
          (int) &array1[0], array1[0], array1[0]);
  printf("SP.14: array1[1]: stored at %08x (hex); value is %d (dec), %08x (hex)\n",
          (int) &array1[1], array1[1], array1[1]);

  ptr1 = array1; /* assign new value to pointer variable ptr1 */
  printf ("\nSP.15: Executed ptr1 = array1; \n");

  printf ("SP.16: ptr1: stored at %08x (hex); value is %d (dec), %08x (hex)\n",
          (int) &ptr1, (int) ptr1, (int) ptr1);
  printf ("SP.17: Dereference pointer ptr1 and we find: %d \n", *ptr1);

  ptr1 = ptr1 + 1; /* add 1 to char pointer */
  printf ("\nSP.18: Executed ptr1 = ptr1 + 1; \n");

  printf ("SP.19: ptr1: stored at %08x (hex); value is %d (dec), %08x (hex)\n",
          (int) &ptr1, (int) ptr1, (int) ptr1);
  printf ("SP.20: Dereference pointer ptr1 and we find: %d \n", *ptr1);
  
  ptr2 = array1; /* assign new value to pointer variable ptr2 */
  printf ("\nSP.21: Executed ptr2 = array1; \n");
  
  printf ("SP.22: ptr2: stored at %08x (hex); value is %d (dec), %08x (hex)\n",
          (int) &ptr2, (int) ptr2, (int) ptr2);
  printf ("SP.23: Dereference pointer ptr2 and we find: %d \n", *ptr2);
  
  ptr2 = ptr2 + 1; /* add 1 to int pointer */
  printf ("\nSP.24: Executed ptr2 = ptr2 + 1; \n");
  
  printf ("SP.25: ptr2: stored at %08x (hex); value is %d (dec), %08x (hex)\n",
          (int) &ptr2, (int) ptr2, (int) ptr2);
  printf ("SP.26: Dereference pointer ptr2 and we find: %d \n", *ptr2);
}
