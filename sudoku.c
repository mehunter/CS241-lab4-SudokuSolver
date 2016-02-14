/***************************************/
/* Matt Hunter -- CS241.01 Spring 2016 */
/* Lab 4: Sudoku Solver                */
/***************************************/


#define TRUE 1
#include <stdio.h>

int puzzle[81];

/*
 * This function reads in a sudoku puzzle, line-by-line.  It checks
 * each puzzle to see if the puzzle is 'well formed'.  Well formed
 * means the puzzle has the correct number of entries (81), and that 
 * all entries are either numbers or the '.' character.
 *                                                                 
 * Upon encountering an EOF, it sets a code that the loop in main()
 * uses to exit the program.
 */
int readPuzzle()
{
 int c, i, errorCode;
 c = i = errorCode = 0;
 /* errorCodes:
  * bit 1 - invalid character in puzzle
  * bit 2 - too many characters in puzzle
  * bit 3 - not enough characters in puzzle
  * bit 4 - end of file
  */
 
 while ( (c = getchar() ) != '\n' )
   {
     if ( c == EOF ) return 8;
     putchar(c);
     if ( c == '.') c = '0';                       /* assign . to 0 */
     if ( (c < '0') || (c > '9') ) errorCode |= 1;
     if ( i == 81 )
       {
         errorCode |= 2;
         i -= 81;                                  /* avoid array overflow */
       }
     puzzle[i] = c - '0';                          /* turn c in to number */
     ++i;
   }
 putchar('\n');
 if ( ( (errorCode & 2) != 2) && (i < 81)) errorCode |= 4;
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
      if (puzzleError & 8) break;                  /* reached EOF */
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
