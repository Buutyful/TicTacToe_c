#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 3
char board[SIZE][SIZE];

typedef struct
{
    int X;
    int Y;
}Cell;

typedef struct
{
    Cell From;
    Cell To;

}Line;
bool IsInside(int x, int y)
{
    return x >= 0 && x < SIZE && y >= 0 && y < SIZE;
}
bool IsEmpty(int x, int y)
{
    return IsInside(x, y) && board[x][y] == ' ';
}
bool MakeMove(Cell* cell, int currentPlayer)
{
    int x = cell->X;
    int y = cell->Y;
    if (IsEmpty(x, y))
    {
        board[x][y] = currentPlayer % 2 == 1 ? 'X' : 'O';
        return true;
    }
    return false;
}
//get all lines a cell belongs too
Line* GetLines(Cell* cell, int* lineCount)
{
    bool diag = cell->X == cell->Y;
    bool antiDiag = cell->X + cell->Y == 2;
    int lines = diag && antiDiag ? 4
        : diag || antiDiag ? 3 : 2;
    *lineCount = lines;

    Line* cellLines = (Line*)malloc(sizeof(Line) * lines);
    int i = 0;
    Line row = { {0, cell->Y}, {2, cell->Y} };
    cellLines[i] = row; i++;
    Line col = { {cell->X, 0}, {cell->X, 2} };
    cellLines[i] = col; i++;

    if (diag)
    {
        Line diag = { {0, 0}, {2, 2} };
        cellLines[i] = diag; i++;
    }
    if (antiDiag)
    {
        Line ant = { {0, 2}, {2, 0} };
        cellLines[i] = ant; i++;
    }
    return cellLines;
}
//get all lines and then check if a line contains all the marks of the currentplayer
bool CheckWin(int currentPlayer, Cell cell)
{
    int lineCount;
    Line* lines = GetLines(&cell, &lineCount);
    char mark = currentPlayer % 2 == 1 ? 'X' : 'O';

    for (int i = 0; i < lineCount; i++)
    {
        bool win = true;
        Line line = lines[i];
        //props to chatgpt for this clever way to calculate the increment steps, math is hard :C
        int xStep = (line.To.X - line.From.X) / (SIZE - 1);
        int yStep = (line.To.Y - line.From.Y) / (SIZE - 1);

        for (int j = 0; j < SIZE; j++)
        {
            int x = line.From.X + j * xStep;
            int y = line.From.Y + j * yStep;
            if (board[x][y] != mark)
            {
                win = false;
                break;
            }
        }
        if (win)
        {
            free(lines);
            return true;
        }
    }
    free(lines);
    return false;
}

void InitializeBoard()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            board[i][j] = ' ';
        }
    }
}
void DisplayBoard()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            printf(" %c ", board[i][j]);
            if (j < SIZE - 1) printf("|");
        }
        printf("\n");
        if (i < SIZE - 1) printf("---|---|---\n");
    }
}

int main()
{
    InitializeBoard();

    int currentPlayer = 1;
    int moveCount = 0;
    bool gameWon = false;
    bool draw = false;

    while (!gameWon && !draw)
    {
        DisplayBoard();
        printf("Player %d, enter your move (row and column): ", currentPlayer);
        int row, col;
        scanf_s("%d %d", &row, &col);
        Cell cell = { row, col };

        if (MakeMove(&cell, currentPlayer))
        {
            gameWon = CheckWin(currentPlayer, cell);
            if (gameWon)
            {
                DisplayBoard();
                printf("Player %d wins!\n", currentPlayer);
            }
            else
            {
                if (moveCount == 9)
                {
                    draw = true;
                    DisplayBoard();
                    printf("The game is a draw!\n");
                }
                moveCount++;
                currentPlayer = 3 - currentPlayer;
            }
        }
        else printf("Insert correct row and col.\n");

    }

    return 0;
}