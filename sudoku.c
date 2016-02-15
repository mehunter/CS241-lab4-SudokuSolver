/***************************************/
/* Matt Hunter -- CS241.01 Spring 2016 */
/* Lab 4: Sudoku Solver                */
/***************************************/


#include <stdio.h>

/* This function reads in a sudoku puzzle, line-by-line.  It checks each
 * read character to see if it is valid \n, EOF, '.', or 0 - 9.  If it
 * finds EOF, sets an error bit and returns immediately.  If it finds an 
 * invalid character, it sets an error bit.  It maintains a counter for 
 * the number of characters read to give an error for a puzzle that is too
 * long or too short if true sets an error bit. 
 * 
 * errorCodes:
 * bit 1 - end of file
 * bit 2 - invalid character in puzzle
 * bit 3 - too many characters in puzzle
 * bit 4 - not enough characters in puzzle
 *
 * If no errors, int readPuzzle() will return 0.
 */
int readPuzzle(int puzzle[])
{
  int c, i, errorCode;
  c = i = errorCode = 0;
 
  while ( (c = getchar() ) != '\n' )
    {
      if ( c == EOF )                               /* check for EOF */
        {
          errorCode |= 1;
          return errorCode;                         /* reached EOF, return */
        }
      putchar(c);                                   /* echo puzzle */
      if ( c == '.') c = '0';                       /* assign . to 0 */
      if ( (c < '0') || (c > '9') )
        {
          errorCode |= (1 << 1);                    /* set bit 2 */
        }
      if ( i == 81 )
        {
          errorCode |= (1 << 2);                    /* set bit 3 */
          i -= 81;                                  /* avoid array overflow */
        }
      puzzle[i] = c - '0';                          /* turn c in to number */
      ++i;
    }
  
  putchar('\n');                                    /* done with puzzle echo */

  if ( ( (errorCode & 4) != 4) && (i < 81))         /* not enough characters */
    {
      errorCode |= (1 << 3);                        /* set bit 4 */
    }
 
  return errorCode;
}


/* This function check rows for duplicates, sets bit 5 in errorCode if
 * a duplicate is found.  Returns errorCode.
 */
int validatePuzzleRows(int puzzle[])
{
  int i, j, errorCode;
  i = j = errorCode = 0;

  for (i = 0; i < 9; i++)
    {
      for (j = i+1; j < 9; j++)
        {
          if (puzzle[i] != 0 && (puzzle[i] == puzzle[j]) )
            {
              errorCode |= (1 << 4);                /* set bit 5 */
            }
        }
    }
  return errorCode;
}

/* This function checks columns for duplicates, sets bit 6 in errorCode
 * if a duplicate is found.  Returns errorCode.
 */
int validatePuzzleCols(int puzzle[])
{
  int i, j, errorCode;
  i = j = errorCode = 0;
  
  for (i = 0; i < 81; i += 9)
    {
      for (j = i+9; j < 81; j +=9)
        {
          if (puzzle[i] != 0 && (puzzle[i] == puzzle[j]) )
            {
              errorCode |= (1 << 5);               /* set bit 6 */
            }
        }
    }
  return errorCode;
}

/* This function check boxes for duplicates, sets bit 7 in errorCode 
 * if a duplicate is found.  Returns errorCode.
 */
int validatePuzzleBoxes(int puzzle[])
{
  /* WRITE ME */
  int errorCode;
  if (puzzle[0]) errorCode = 0;
  return errorCode;
}

/* This function logically reduces the given puzzle by using the rules of
 * Sudoku to fill in any spaces that can be found by inspecting rows,
 * columns, and boxes.
 */
void crunchPuzzle(int puzzle[])
{
  /* WRITE ME */
  if (puzzle[0]) return;
}

/* This function uses recursive backtracking to solve the puzzle.
 */
void solvePuzzle(int puzzle[])
{
  /* WRITE ME */
  if (puzzle[0]) return;
}


/* This function writes out the (hopefully solved) puzzle.
 */
void writePuzzle(int puzzle[])
{
  int i;
  for ( i = 0; i < 81; ++i)
    {
      if (puzzle[i] != 0 ) printf("%d", puzzle[i]);
      else printf(".");
    }
  printf("\n\n");
}


int main(int argc, char *argv[])
{
  int puzzle[81];
  int puzzleError;
  while (1)                                        /* read puzzles until EOF */
    {
      puzzleError = readPuzzle(puzzle);
      if (puzzleError & 1) break;                  /* reached EOF, done! */
      puzzleError |= validatePuzzleRows(puzzle);
      puzzleError |= validatePuzzleCols(puzzle);
      puzzleError |= validatePuzzleBoxes(puzzle);

      /* Command line argument -e used to print error numbers for debugging */
      if (puzzleError && (argc == 2) && (argv[1][0] == '-') &&
          (argv[1][1] == 'e') && (argv[1][2] == '\0'))
        {
          printf("Error - %d\n\n", puzzleError);            
        }
      else if (puzzleError) printf("Error\n\n");
      else                                         /* puzzle is 'well-formed' */
        {
          crunchPuzzle(puzzle);
          solvePuzzle(puzzle);
          writePuzzle(puzzle);
        }
    }
  return 0;
}
