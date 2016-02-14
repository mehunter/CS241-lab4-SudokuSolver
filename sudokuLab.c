void readPuzzle(int puzzle[9][9]);
void isValid(int puzzle[9][9]);
int isSafe(int puzzle, int index, int n);
int n;

int solve(int puzzle[9][9], int index)
{
  /* keep position in bounds, see if position is full or not */
  int row = index / 9;
  int col = index % 9;
  
  if (row == 9) return 1; /* keep us in bounds row wise */
  if (puzzle[row][col] != '.')
  {
    return solve(puzzle, ++index); /* recursive call */
  }

  else
    {
      for ( n = 1; n < 10; n++)
        {
          if (isSafe(puzzle, index, n))
            {
              puzzle[row][col] = n;
              if (solve(puzzle, ++index)) return 1;
            }
        }
      
      puzzle[row][col] = '.';
      return 0;
      
      
    }
}

int main()
{
  int puzzle[9][9];
  void readPuzzle(int puzzle[9][9]);
  void isValid(int puzzle[9][9]);
  int solve(int puzzle[9][9], int index);
}
