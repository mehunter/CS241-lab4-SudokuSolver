/***************************************/
/* Matt Hunter -- CS241.01 Spring 2016 */
/* Lab 4: Sudoku Solver                */
/***************************************/


#define TRUE 1
#include <stdio.h>

int puzzle[81];                         /* global variable for puzzle */

/*
 * This function reads in a sudoku puzzle, line-by-line.  It checks
 * each puzzle to see if the puzzle is 'well-formed'.  Well-formed
 * means the puzzle has the correct number of entries (81), that 
 * all entries are either numbers or the '.' character, and that
 * there are no duplicates in rows, columns, and boxes.
 *                                                                 
 * Upon encountering an EOF, it sets a code that the loop in main()
 * uses to exit the program.
 *
 * Well-formed puzzle will return 0
 */
int readPuzzle()
{
  int c, i, j, errorCode;
  c = i = j = errorCode = 0;
 /* errorCodes:
  * bit 1 - invalid character in puzzle
  * bit 2 - too many characters in puzzle
  * bit 3 - not enough characters in puzzle
  * bit 4 - bad row
  * bit 5 - bad column
  * bit 6 - bad box
  * bit 7 - end of file
  */
 
 while ( (c = getchar() ) != '\n' )
   {
     if ( c == EOF ) return (1 << 6);              /* set bit 7 and return */
     putchar(c);
     if ( c == '.') c = '0';                       /* assign . to 0 */
     if ( (c < '0') || (c > '9') ) errorCode |= 1; /* set bit 1 */
     if ( i == 81 )
       {
         errorCode |= (1 << 1);                    /* set bit 2 */
         i -= 81;                                  /* avoid array overflow */
       }
     puzzle[i] = c - '0';                          /* turn c in to number */
     ++i;
   }
 putchar('\n');
 if ( ( (errorCode & 2) != 2) && (i < 81))         /* not enough characters */
   {
     errorCode |= (1 << 2);                        /* set bit 3 */
   }
 /* check rows for duplicates */
 for (i = 0; i < 9; i++)
   {
     for (j = i+1; j < 9; j++)
       {
         if (puzzle[i] != 0 && (puzzle[i] == puzzle[j]) )
           {
             errorCode |= (1 << 3);                /* set bit 4 */
           }
       }
   }

 /* check columns for duplicates */
 for (i = 0; i < 81; i += 9)
   {
     for (j = i+9; j < 81; j +=9)
       {
         if (puzzle[i] != 0 && (puzzle[i] == puzzle[j]) )
           {
             errorCode |= (1 << 4);               /* set bit 5 */
           }
       }
   }


 /* check boxes for duplicates */

 
 return errorCode;
}

/*
 * This function logically reduces the given puzzle by using the rules of
 * Sudoku to fill in any spaces that can be found by inspecting rows,
 * columns, and boxes.
 */
void crunchPuzzle()
{
  /* WRITE ME */
}

void solvePuzzle()
{
  /* WRITE ME */
}

void writePuzzle()
{
  int i;
  for ( i = 0; i < 81; ++i)
    {
      if (puzzle[i] != 0 ) printf("%d", puzzle[i]);
      else printf(".");
    }
  printf("\n\n");
}


int main(void)
{
  int puzzleError;
  while (TRUE)                                     /* read puzzles until EOF */
    {
      puzzleError = readPuzzle();
      if (puzzleError & (1 << 6)) break;           /* reached EOF */
      if (puzzleError)      
        {
          printf("Error");
          printf(" - %d", puzzleError);            /* comment out line */
          printf("\n\n");
        }
      else                                         /* puzzle is 'well formed */
        {
          crunchPuzzle();
          solvePuzzle();
          writePuzzle();
        }
    }
}
