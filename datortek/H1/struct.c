/*
 * struct.c - Program to test struct
 * Written 2009-07-22--11-02 by F Lundevall
 * Copyright abandoned. This file is in the public domain.
 */
#include <stdio.h>      /* Defines printf. */
#define ARRAYSIZE 5

/* Declare structured types (but not variables). */
struct ipair
{
  int v1;
  int v2;
};
struct nested
{
  int val;
  char c;
  struct ipair z;
  char str[7];
};

/* Declare global variable ipairarray - an array of structs. */
struct ipair ipairarray[ ARRAYSIZE ]; /* Array of ipairs. */

/* Declare some structured variables. */
struct ipair s1;
struct nested nes = { 17, 'Q', { 117, 217 }, "Hello!" }; /* Initialize. */

int main ()         /* Called as a method/function/subroutine. */
{
  int i;            /* Loop index variable. */
  int * ip;         /* Temporary pointer to int for printouts. */
  struct ipair * ipp;    /* Declare a pointer to struct ipair. */
  struct nested * nesp;  /* Declare a pointer to struct nested. */

  s1.v1 = 11;       /* Assign a value to val in s1. */
  s1.v2 = 17;       /* Assign a value to v2 in s1. */

  printf ("Message ST.01 from struct.c: Hello, structured World!\n");
  printf ("ST.02: s1: stored at %08x (hex), sizeof(s1) is %d (dec)\n",
          (int) &s1, (int) sizeof(s1) );
  printf ("ST.03: s1.v1 at %08x (hex) contains %d (dec), %08x (hex)\n",
          (int) &(s1.v1), s1.v1, s1.v1);
  printf ("ST.04: s1.v2 at %08x (hex) contains %d (dec), %08x (hex)\n",
          (int) &(s1.v2), s1.v2, s1.v2);

  ipp = &s1;        /* Pointer ipp now points to a struct ipair. */
  printf ("\nST.05: Executed ipp = &s1;\n");
  printf ("ST.06: ipp: stored at %08x (hex), contains %d (dec), %08x (hex)\n",
          (int) &ipp, (int) ipp, (int) ipp );  
  printf ("ST.07: Dereference pointer ipp and we find: (*ipp).v1=%d, (*ipp).v2=%d\n",
          (*ipp).v1, (*ipp).v2 );
  printf ("ST.08: Dereference with different syntax: ipp->v1=%d, ipp->v2=%d\n",
          ipp->v1, ipp->v2 );
          
  (*ipp).v1 = nes.val; /* Copy a value using dot-syntax. */
  printf ("\nST.09: Executed (*ipp).v1 = nes.val;\n");

  ipp -> v2 = 4711;  /* Assign a value using arrow syntax. */
  printf ("ST.10: Executed ipp -> v2 = 4711;\n");
  printf ("ST.11: Dereference pointer ipp and we find: (*ipp).v1=%d, (*ipp).v2=%d\n",
          (*ipp).v1, (*ipp).v2 );

  for( i = 0; i < ARRAYSIZE; i += 1 )
  {
    ipairarray[ i ].v1 = 1000 + i;
    ipairarray[ i ].v2 = 2000 + i;
  }
  printf ("\nST.12: Initialized ipairarray.\n");

  ip = (int *) ipairarray;
  for( i = 0; i < ARRAYSIZE * 2; i += 1 )
  {
    printf("ST.%.2d: Memory at %08x (hex) contains %d\n",
           i+13, (int) ip, *ip);
    ip += 1;
  }

  ipp = ipairarray;
  printf ("\nST.23: Executed ipp = ipairarray;\n");
  printf ("ST.24: ipp: stored at %08x (hex), contains %d (dec), %08x (hex)\n",
          (int) &ipp, (int) ipp, (int) ipp );  
  printf ("ST.25: Dereference pointer ipp and we find: ipp->v1=%d, ipp->v2=%d\n",
          ipp->v1, ipp->v2 );

  ipp = ipp + 1;
  printf ("\nST.26: Executed ipp = ipp + 1;\n");
  printf ("ST.27: ipp: stored at %08x (hex), contains %d (dec), %08x (hex)\n",
          (int) &ipp, (int) ipp, (int) ipp );
  printf ("ST.28: Dereference pointer ipp and we find: ipp->v1=%d, ipp->v2=%d\n",
          ipp->v1, ipp->v2 );

  printf ("\nST.29: nes: stored at %08x (hex), sizeof(nes) is %d (dec)\n",
          (int) &nes, (int) sizeof(nes) );
  printf ("ST.30: nes.val at %08x (hex) contains %d (dec), %08x (hex)\n",
          (int) &(nes.val), nes.val, nes.val);
  printf ("ST.31: nes.c at %08x (hex) contains '%c', 0x%02x (hex)\n",
          (int) &(nes.c), nes.c, nes.c);
  printf ("ST.32: nes.z: stored at %08x (hex)\n", (int) &(nes.z));
  printf ("ST.33: (nes.z).v1 at %08x (hex) contains %d (dec), %08x (hex)\n",
          (int) &((nes.z).v1), (nes.z).v1, (nes.z).v1);
  printf ("ST.34: (nes.z).v2 at %08x (hex) contains %d (dec), %08x (hex)\n",
          (int) &((nes.z).v2), (nes.z).v2, (nes.z).v2);
  printf ("ST.35: nes.str at %08x (hex) contains: %s\n",
          (int) &(nes.str), nes.str );

  nesp = &nes;      /* Let nesp point to nes. */
  printf ("\nST.36: Executed nesp = &nes;\n" );
  printf ("ST.37: nesp: stored at %08x (hex); contains %d (dec), %08x (hex)\n",
          (int) &nesp, (int) nesp, (int) nesp);
  printf ("ST.38: Dereference pointer nesp and we find: nesp->val=%d, and...\n",
          nesp->val );
  printf ("ST.39: nesp->c='%c', (nesp->z).v1=%d,(nesp->z).v2=%d, str=\"%s\"\n",
          nesp->c, (nesp->z).v1, (nesp->z).v2, nesp->str );

  nesp += 1;
  printf ("ST.37: nesp: stored at %08x (hex); contains %d (dec), %08x (hex)\n",
          (int) &nesp, (int) nesp, (int) nesp);
  printf ("ST.38: Dereference pointer nesp and we find: nesp->val=%d, and...\n",
          nesp->val );
  printf ("ST.39: nesp->c='%c', (nesp->z).v1=%d,(nesp->z).v2=%d, str=\"%s\"\n",
          nesp->c, (nesp->z).v1, (nesp->z).v2, nesp->str );

  return( 0 );
}
