/***************************************/
/* Matt Hunter -- CS241.01 Spring 2016 */
/* Lab 4: Sudoku Solver                */
/***************************************/

#define TRUE 1
#define FALSE 0

/* Set this definition to 1 to enable display of puzzle error messages */ 
#define ERRMSG 0

/* Set this definition to 1 to enable display bit info in crunchPuzzle */
#define WORKWITHBITS 0

/* Set this definition to 1 to enable display of Sudoku formated printing */
#define SUDOKUSTYLE 1


#include <stdio.h>

         int readPuzzle(int puzzle[]);
         int validatePuzzleRows(int puzzle[]);
         int validatePuzzleCols(int puzzle[]);
         int validatePuzzleBoxes(int puzzle[]);
        void crunchPuzzle(int puzzle[]);
        void printBitPuzzle(int bitPuzzle[]);
unsigned int bitCount(unsigned int value);
         int whichBit(int value);
         int solvePuzzle(int puzzle[]);
         int blankCell(int puzzle[]);
         int promisingNumber(int puzzle[], int i, int num);
         int rowConflict(int puzzle[], int i, int num);
         int colConflict(int puzzle[], int i, int num);
         int boxConflict(int puzzle[], int i, int num);
        void writePuzzle(int puzzle[]);



int main(void)
{
  int puzzle[81];
  int puzzleError;
  
  puzzleError = readPuzzle(puzzle);

  /* Check puzzles for errors, crunch, solve, and write.  Keep doing this
   * until error code 1 set which indicates EOF reached.
   */
  while ( !(puzzleError & 1) )
    {
      puzzleError |= validatePuzzleRows(puzzle);
      puzzleError |= validatePuzzleCols(puzzle);
      puzzleError |= validatePuzzleBoxes(puzzle);

      if (puzzleError)
	{
	  printf("Error");
	  if (ERRMSG) printf(" - %d", puzzleError);
	  printf("\n\n");
	}

      /* puzzle is 'well-formed', so solve it and write the results
       */
      else                                       
        {
          crunchPuzzle(puzzle);
          if (solvePuzzle(puzzle)) printf("SOLVED\n");
          else printf("UNSOLVED\n");
          writePuzzle(puzzle);
        }
      puzzleError = readPuzzle(puzzle);
    }
  return 0;
}


/* This function reads in a sudoku puzzle, line-by-line.  It checks each
 * read character to see if it is valid \n, EOF, '.', or 0 - 9.  If it
 * finds EOF, sets an error bit and returns immediately.  If it finds an 
 * invalid character, it sets an error bit.  It maintains a counter for 
 * the number of characters read to give an error for a puzzle that is too
 * long or too short if true sets an error bit. Echo puzzle to standard out.
 * 
 * errorCodes:
 *   bit 1 - end of file
 *   bit 2 - invalid character in puzzle
 *   bit 3 - too many characters in puzzle
 *   bit 4 - not enough characters in puzzle
 *
 *   No errors - return 0.
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

  for (i = 0; i < 81; i += 9)
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
 * if a duplicate is found.  Returns errorCode.  Loop through each box
 * converting the box into a mini-array, then checking the array for
 * duplicates.
 */
int validatePuzzleBoxes(int puzzle[])
{
  int  boxToRow[9];
  int i, j, k, box, boxDelta, errorCode;
  i = j = k = box = boxDelta = errorCode = 0;
  for (box = 0; box < 9; box++)
    {
      boxDelta = ((box % 3) * 3) + ((box / 3) * 27);
      for (k = 0; k < 9; k++)
        {
          boxToRow[k] = puzzle[ ((k / 3) * 9) + (k % 3) + boxDelta];
        }

      for (i = 0; i < 9; i++)
        {
          for (j = i+1; j < 9; j++)
            {
              if (boxToRow[i] != 0 && (boxToRow[i] == boxToRow[j]) )
                {
                  errorCode |= (1 << 6);                /* set bit 7 */
                }
            }
        }
    }
  return errorCode;
}

/* This function logically reduces the given puzzle by using the rules of
 * Sudoku to fill in any spaces that can be found by inspecting rows,
 * columns, and boxes.  For blank squares in puzzle, sets bits 1-9. For filled
 * squares, sets bit for number - i.e. if 7 in square, sets bit #7.  Once
 * put in this form, uses NAND over each row, column, and box.  The bits
 * left on in the square represent the possible values that square could
 * have.  Keep looping over the puzzle until no more bits change.  If total
 * number of bits in puzzle is 81- then puzzle is solved!  If any square has
 * zero bits turned on- then puzzle is un-solvable.
 */
void crunchPuzzle(int puzzle[])
{
  int bitPuzzle[81];
  int i, j, temp, totalBits, crunchItAgain, boxStart;
  int boxMoves[] = {0, 1, 2, 9, 10, 11, 18, 19, 20};
  
  /* create bitPuzzle with # of bit on equal to value in puzzle */
  for (i = 0; i < 81; i++)
    {
      if (puzzle[i] == 0) bitPuzzle[i] = 0;
      else bitPuzzle[i] = 1 << (puzzle[i]-1);
    }

  crunchItAgain = 1;     /* flag variable to track if puzzle needs crunch */
  while (crunchItAgain)
    {
      crunchItAgain = 0;
      /* Find a cell that is zero.  Use bitwise OR to find out which bits
       * are used in its row, column, and box.
       */
      for (i = 0; i < 81; i++)
        {
          if (bitPuzzle[i] == 0)
            {
              temp = 0;

              /*loop for rows */
              for (j = 0; j < 9; j++)
                {
                  temp |= bitPuzzle[((i / 9) * 9) + j];
                }
              
              /* loop for columns  */
              for (j = 0; j < 9; j++)
                {
                  temp |= bitPuzzle[(i + (j * 9)) % 81];
                }

             /* loop for boxes */
              boxStart = ( ( i / 27 ) * 27) + ( ( (i % 9) / 3 ) * 3);
              for (j = 0; j < 9; j++)
                {                                 
                  temp |= bitPuzzle[(boxStart + boxMoves[j])];
                }

              /* temp is numbers used in row, col and box, but which values
               * were not used- those are the remaining possibilities
               */
              temp = 0x1ff - temp;
                            
              /* if left with a single bit on, then cell is determined */
              if (bitCount(temp) == 1)
                {
                  bitPuzzle[i] = temp;
                  puzzle[i] =  whichBit(temp);
                  crunchItAgain = 1;
                }
            }
        }
    }
  
  /* code turned on to show bit process of working to reduce puzzle with
   * obvious answers
   */
  if (WORKWITHBITS)
    {
      printBitPuzzle(bitPuzzle);

      /* calculate total bitCount of the puzzle - solved if 81 */
      totalBits = 0;
      for (i = 0; i < 81; i++) totalBits += bitCount(bitPuzzle[i]);
      if (totalBits == 81) printf("SOLVED -- ");
      printf(" totalBits = %d\n\n", totalBits);
    }

  if (puzzle[0]) return;
}

/* Helper function for crunchPuzzle that counts number of bits turned on
 */
unsigned int bitCount (unsigned int value)
{
  unsigned int count = 0;
  while (value > 0)                         /* until all bits are zero */
    {
      if ((value & 1) == 1)                 /* check lower bit */
      count++;
      value >>= 1;                          /* shift bits, remove lower bit */
    }
  return count;
}

/* Helper function for crunchPuzzle that returns a decimal for a single bit
 * turned on.
 */
int whichBit (int value)
{
  int decimal = 0;
  while (value > 0)
    {
      decimal++;
      value >>= 1;
    }
  return decimal;
}

/* Helper function for crunchPuzzle that prints out bitPuzzle in hex 
 */
void printBitPuzzle(int bitPuzzle[])
{
  int i;
  for (i = 0; i < 81; i += 3)
    {
      if ( !( i % 27) )
        {
          printf("\n -------------------------------------------\n | ");
        }
      if ( !(i%9) && (i%27) ) printf("\n | ");
      printf("%03x %03x %03x", bitPuzzle[i], bitPuzzle[i+1], bitPuzzle[i+2]);
      printf(" | ");
    }
  printf("\n -------------------------------------------\n");
}

/* This function uses recursive backtracking to solve the puzzle.
 */
int solvePuzzle(int puzzle[])
{
  /* Find an a cell with value 0, not solved
   * If there are none, return true
   * 
   * For digits 1 to 9
   *   if there is no conflict for row, col, box
   *      assign digit to cell and recursively try to fill rest of puzzle
   *      if recursion successful, return true
   *      if !successful, remove digit and try another
   * if all digits have been tried and nothing worked, return false to trigger backtracking
   */

  int i, num, solved;

  if ((i = blankCell(puzzle)) == FALSE) return TRUE;  /* Solved puzzle! All cells filled! */
  printf("i = %d\n", i);
  for (num = 1; num <= 9; num++)                /* check all values 1 - 9 */
    {
      if (promisingNumber(puzzle, i, num))      /* potentially a solution */
        {
          puzzle[i] = num;                      /* put num into puzzle */
          if (solvePuzzle(puzzle)) return TRUE;
          puzzle[i] = 0;
        }
    }
  return FALSE;
}

int blankCell(int puzzle[])
{
  int i;
  for (i = 0; i < 81; i++)
    {
      if (puzzle[i] == 0) return i;
    }
  return FALSE;
}

int promisingNumber(int puzzle[], int i, int num)
{
  return !rowConflict(puzzle, i, num) &&
    !colConflict(puzzle, i, num) &&
    !boxConflict(puzzle, i, num);

}

int rowConflict(int puzzle[], int i, int num)
{
  int j;
  for (j = 0; j < 9; j++)
    {
      if (puzzle[(i + j) % 9] == num) return TRUE;
    }
  printf("%d col clear\n", i);
  return FALSE;
}

int colConflict(int puzzle[], int i, int num)
{
  int j;
  for (j = 0; j < 9; j++)
    {
      if (puzzle[((i + (j * 9)) % 81)] == num) return TRUE;
    }
  return FALSE;
}

int boxConflict(int puzzle[], int i, int num)
{
  int j, boxStart;
  int boxMoves[] = {0, 1, 2, 9, 10, 11, 18, 19, 20};
  boxStart = ( ( i / 27 ) * 27) + ( ( (i % 9) / 3 ) * 3);
  for (j = 0; j < 9; j++)
    {                                 
      if (puzzle[(boxStart + boxMoves[j])] == num) return TRUE;
    }
  return FALSE;
}

/* This function writes out the (hopefully solved) puzzle.  If definition
 * for SUDOKUSTYLE = 1, then prints out as standard human readable puzzle.
 * Otherwise, prints out on a single line per project spec.
 */
void writePuzzle(int puzzle[])
{
  int i, pctSolved;
  if (SUDOKUSTYLE)
    {
      pctSolved = 0;
      for (i = 0; i < 81; i++)
	{
          if ( (i != 0) && !(i % 9) ) printf("|\n");
          if ( (i == 0) || (i == 27) || (i == 54) )
               printf(" ------------------------------------\n");
          if ( !(i % 3) ) printf(" | ");
          
          
          if (puzzle[i] == 0) printf(" . ");
          else printf("%2d ", puzzle[i]);
          pctSolved += puzzle[i];
        }
      printf("|\n ------------------------------------\n");
      printf("%d%% solved\n\n", pctSolved * 100 / 405);
    }

  else
    {
      for ( i = 0; i < 81; ++i)
	{
	  if (puzzle[i] != 0 ) printf("%d", puzzle[i]);
	  else printf(".");
	}
      printf("\n\n");

    }
}
