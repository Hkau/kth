/*
 * malloc.c - An exercise with malloc
 * 
 * Written 2009-07-24 and 2009-10-20 by F Lundevall
 *
 * Copyright abandoned. This file is in the public domain.
 */

#include <stdio.h>      /* Declares printf. */
#include <stdlib.h>     /* Declares malloc and free. */
#include <string.h>     /* Declares strlen and strncpy */

/* Note: on some weird systems, you may need to
 * #include malloc.h - but ONLY do this if you otherwise
 * get warnings for implicit declarations of malloc and free. */

#define SIZE (48)      /* Size of array. Change here if you
                          want to try a different size.  */

char cowabunga[] = "Cowabunga!";

/* gcount - count and print number of g's in a string. */
void gcount( char * inputstring, int stringsize )
{
  register int * gcountp;   /* Pointer to mallocated g-count. */
  register int i;           /* For-loop index variable. */

  /* Use malloc to allocate room for an int. */
  gcountp = (int *) malloc( sizeof( int ) );
  
  if( 0 == (int) gcountp )  /* Check for malloc failure. */
    perror( "g-count malloc failed" );
  else
  {
    printf( "g-count successfully allocated, with size %d bytes.\n",
          (int) sizeof( int ) );
    printf( "g-count malloc returned address %d (dec), %08x (hex).\n",
          (int) gcountp, (int) gcountp );

    *gcountp = 0;   /* Clear counter, then count. */
    for( i = 0; i < stringsize; i += 1 )
      if( 'g' == inputstring[ i ] ) *gcountp += 1;
    printf( "Number of g's in array is %d.\n", *gcountp );
    
    free( gcountp ); /* Free mallocated memory, so it can be re-used. */
    printf( "Executed free( gcountp );\n" );
  }
}  

int main()
{

  register char * arrayp = (char *) 0;  /* Will point to a mallocated array. */
  register int i;           /* For-loop control variable. */
  register char * extrap;   /* Pointer to another mallocated array. */
  register int alpha;       /* Counter for alphabet. */

  printf( "Message MA01 from malloc.c: Hello, memory-allocating World!\n" );

  /* Use malloc to allocate space for array */
  arrayp = (char *) malloc( SIZE * sizeof( char ) );
  /* Return values from library calls must always be checked. */
  if( 0 == (int) arrayp )
  { /* If malloc fails, it returns a null pointer.  */
    perror( "malloc for array failed" );
    exit( 1 );
  } /* End of return-value check for malloc */

  /* Print info about our mallocated array. */
  printf( "MA02: Main array successfully allocated, with size %d bytes.\n",
          SIZE );
  printf( "MA03: Main array malloc returned address %d (dec), %08x (hex).\n",
          (int) arrayp, (int) arrayp );

  /* Initialize array with an alphabet, repeated if necessary. */
  alpha = 'a';
  for( i = 0; i < (SIZE - 1); i += 1 )
  {
    arrayp[ i ] = alpha;
    alpha += 1;
    if( alpha > 'z' ) alpha = 'a';
  }
  /* Add null char at the end, to make array contain a valid C string. */
  arrayp[ SIZE - 1 ] = 0;

  printf( "MA04: Main array now contains the following string:\n%s\n", arrayp );

  gcount( arrayp, SIZE );  /* Call gcount, which uses malloc. */
  
  /* Now mallocate room for a copy of Cowabunga! */
  extrap = (char *) malloc( sizeof( cowabunga ) );
  /* Return values from library calls must always be checked */
  if( 0 == (int) extrap )
  {
    /* malloc returned null pointer, print error message.  */
    perror( "malloc for cowabunga failed" );
    exit( 2 );
  }

  printf( "MA05: Cowabunga array successfully allocated, with size %d bytes.\n",
           (int) sizeof( cowabunga ) );
  printf( "MA06: Cowabunga array malloc returned address %d (dec), %08x (hex).\n",
          (int) extrap, (int) extrap );

  if( 0 != (int) extrap )
  {
    strcpy( extrap, cowabunga );
    printf( "MA07: Cowabunga array now contains the following string: %s\n",
            extrap );

    /* Copy extrap to array, but stop if array is too small. */
    strncpy( arrayp, extrap, SIZE );
    /* Put null char at the end, in case strncpy had to stop early. */
    arrayp[ SIZE - 1 ] = 0;
  }
  
  printf( "MA08: Main array now contains the following string:\n%s\n", arrayp );

  gcount( arrayp, SIZE );  /* Call gcount, which uses malloc. */

  /* Free mallocated memory. */
  free( arrayp );   /* Free memory allocated to original array. */
  printf( "MA09: Executed free( arrayp );\n" );
  
  gcount( extrap, sizeof( cowabunga ) );  /* Call gcount, which uses malloc. */
  
  free( extrap );  /* Free memory allocated to new array. */
  printf( "MA10: Executed free( extrap );\n" );
} /* End of main program */
