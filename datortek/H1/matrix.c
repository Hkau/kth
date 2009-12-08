/* matrix.c - test rows and columns of a matrix
 * Copyright (C) 2004-2005 F Lundevall and J Wennlund */

#include <stdio.h>
#define SIZE 5

int matris1[ SIZE ][ SIZE ];
char matris2[ SIZE ][ SIZE ];
double matris3[ SIZE ][ SIZE ];
int matris4[ SIZE ][ SIZE ];

int main()
{
  int i;
  int j;

  int * ptr1;
  char * ptr2;
  double * ptr3;
  int * ptr4;

  for( i = 0; i < SIZE; i = i + 1 )
    for( j = 0; j < SIZE; j = j + 1 )
    {
      matris1[ i ][ j ] = 10000 + 100*i + j;
      matris2[ i ][ j ] = 10*i + j;
      matris3[ i ][ j ] = 1.0 + i/100.0 + j/10000.0;
      matris4[ i ][ j ] = 0;
    };

  printf( "\n Examining matris1:\n" );
  for( ptr1 = &matris1[ 0 ][ 0 ];
       ptr1 <= &matris1[ SIZE - 1 ][ SIZE - 1 ]; ptr1 = ptr1 + 1 )
    printf( "memory at: %08x contains value: %d\n", ptr1, *ptr1 );

  printf( "\n Examining matris2:\n" );
  for( ptr2 = &matris2[ 0 ][ 0 ];
       ptr2 <= &matris2[ SIZE - 1 ][ SIZE - 1 ]; ptr2 = ptr2 + 1 )
    printf( "memory at: %08x contains value: %d\n", ptr2, *ptr2 );

  printf( "\n Examining matris3:\n" );
  for( ptr3 = &matris3[ 0 ][ 0 ];
       ptr3 <= &matris3[ SIZE - 1 ][ SIZE - 1 ]; ptr3 = ptr3 + 1 )
    printf( "memory at: %08x contains value: %f\n", ptr3, *ptr3 );

/* plats för kopiering av innehållet i matris1 till matris4 */

  printf( "\n Examining matris4:\n" );
  for( ptr4 = &matris4[ 0 ][ 0 ];
       ptr4 <= &matris4[ SIZE - 1 ][ SIZE - 1 ]; ptr4 = ptr4 + 1 )
    printf( "memory at: %08x contains value: %d\n", ptr4, *ptr4 );

  return( 0 );
}
