#include <stdio.h>

int main (void)
{
  int charIn;
  int puzzle[81];
  int i = 0;
  
  charIn = getchar();     
  while (charIn != EOF)
    {
      /* read in a line of puzzle & echo */
      while (charIn != '\n')
        {
          putchar(charIn);
          if (charIn == '.') charIn = '0';
          puzzle[i] = (int) (charIn - '0');
          i++;
          i %= 81;
          charIn = getchar();
        }
      printf("\n\n");

      /* write it out sudoku style */
      i = 0;
      int col, row;
      for (row = 0; row < 9; row++)
        {
          if ( !(row % 3) ) printf("-------------------------------\n");
          for (col = 0; col < 9; col++)
            {
              if ( !(col % 3) ) printf("|");
              if (puzzle[(row * 9) + col] == 0 ) printf(" . ");
              else printf ("%2d ",puzzle[(row * 9) + col ]);
            }
          printf("|\n");
        }
      printf("-------------------------------\n");
      printf("\n");
      charIn = getchar();
    }
}           
