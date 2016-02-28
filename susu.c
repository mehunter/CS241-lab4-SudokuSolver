/* Set DEBUG to FALSE to perform per spec.  Set to TRUE to run in debug mode */
#define DEBUG FALSE
#define TRUE 1
#define FALSE 0

#include <stdio.h>

int main (void)
{
   int puzzle[9][9];
   int error;
   
   int readPuzzle(int puzzle[9][9]);
  void writePuzzle(int puzzle[9][9]);
   int solvePuzzle(int puzzle[9][9]);

  error = readPuzzle(puzzle);
  if (error) printf("Error\n\n");
  else
  {
    /* in DEBUG mode, show initial puzzle in standard sudoku form */
    if (DEBUG) writePuzzle(puzzle);
    solvePuzzle(puzzle);
    writePuzzle(puzzle);
  }
  return 0;
}

/*  readPuzzle function fills in the array puzzle[][] with sudoku values from 
 *  standard in.  If it finds a value that is not . or 1-9, then it returns
 *  TRUE as an error code.  Also, if a puzzle consists of some quantity other
 *  than 81 entries, it returns TRUE as an error code.  If there are no errors
 *  in the quantity or range of values, it returns FALSE.  
 *  As required by the spec, whatever puzzle it reads is echoed to standard out.
 */
int readPuzzle(int puzzle[9][9])
{
  int i, num, row, col;
  int error = FALSE;

  for (i = 0; (num = getchar()) != '\n'; i++)
    {
      putchar(num);
      if ((num < '1' || num > '9') && (num != '.')) error = TRUE;
      if (num == '.') num = '0';

      row = (i / 9) % 9;
      col = i % 9;
      puzzle[row][col] = num - '0';
    }
  putchar('\n');
  if (i != 81) error = TRUE;
  return error;
}

/* writePuzzle function writes out the sudoku puzzle from the array puzzle[][].
 * If DEBUG is TRUE, then it writes out the puzzle in standard sudoku form. 
 * If DEBUG is FALSE, it writes out the puzzle in the form required by the spec.
 */
void writePuzzle(int puzzle[9][9])
  {
    int row, col;
    for (row = 0; row < 9; row++)
      {
        if (DEBUG) printf("\n");
        if ((DEBUG) && (row == 3 || row == 6))
        {
          printf("----------------------\n");
        }
        for (col = 0; col < 9; col++)
          {
            if (DEBUG) printf(" ");
            if (puzzle[row][col]) printf("%d", puzzle[row][col]);
            else printf(".");
            if ((DEBUG) && (col == 2 || col == 5)) printf(" |");
          }
      }
    printf("\n\n");
  }

int solvePuzzle(int puzzle[9][9])
{
  int row, col, emptyCellFlag;

  emptyCellFlag = FALSE;
  for (row = 0; row < 9; row++)
  {
    for (col = 0; col < 9; col++)
    {
      if (puzzle[row][col] == 0) emptyCellFlag = TRUE;
    }
  }

  if (emptyCellFlag == FALSE)
  {
    printf("Solved\n");
    return TRUE;
  }

  return FALSE;
}
