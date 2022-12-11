//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//-----------------------------------------------------------------------------

// static const char cInputFileName[] = "test.txt";
static const char cInputFileName[] = "input09.txt";

static std::ifstream infile(cInputFileName);

struct coord
{
    int row;
    int col;
};

static std::vector<coord> tailMoves;
// static const size_t nKnots = 2; // Part 1
static const size_t nKnots = 10; // Part 2
static coord knots[nKnots];

//-----------------------------------------------------------------------------

// rows: + = D, - = U
// cols: + = R, - = L
void moveKnot(coord &knot, int rows, int cols)
{
    knot.row += rows;
    knot.col += cols;
}

void recordTail(void)
{
    bool found = false;
    for (auto pt : tailMoves)
    {
        found = found || ((pt.row == knots[nKnots - 1].row) && (pt.col == knots[nKnots - 1].col));
    }
    if (!found)
    {
        tailMoves.push_back(knots[nKnots - 1]);
    }
}

bool processMove(char move, int n)
{
    std::cout << "Move: " << move << " by " << n << std::endl;

    // Move Head
    int extentRow = 0;
    int extentCol = 0;
    switch (move)
    {
    case 'L':
        extentCol = -1;
        break;

    case 'R':
        extentCol = 1;
        break;

    case 'U':
        extentRow = -1;
        break;

    case 'D':
        extentRow = 1;
        break;

    default:
        return false;
        break;
    }

    for (int i = 0; i < n; ++i)
    {
        // Move Head
        moveKnot(knots[0], extentRow, extentCol);
        // std::cout << "  Head @ [" << H.row << "," << H.col << "]; "; // @DEBUG
        // std::cout << "Tail @ [" << T.row << "," << T.col << "] ... "; // @DEBUG

        // Move Tail knots if their row and column differ from the one before by more than 1
        for (size_t k = 1; k < nKnots; ++k)
        {
            int tailMoveR = 0;
            int tailMoveC = 0;
            int rowDiff   = (knots[k - 1].row - knots[k].row);
            int colDiff   = (knots[k - 1].col - knots[k].col);
            // std::cout << "diff=[" << rowDiff << "," << colDiff << "] "; // @DEBUG

            if (abs(rowDiff) > 1)
            {
                tailMoveR = rowDiff / abs(rowDiff);
                if (abs(colDiff) > 0)
                {
                    tailMoveC = colDiff / abs(colDiff);
                }
            }
            if (abs(colDiff) > 1)
            {
                tailMoveC = colDiff / abs(colDiff);
                if (abs(rowDiff) > 0)
                {
                    tailMoveR = rowDiff / abs(rowDiff);
                }
            }

            // std::cout << "Tmove=[" << tailMoveR << "," << tailMoveC << "] "; // @DEBUG

            moveKnot(knots[k], tailMoveR, tailMoveC);
            // std::cout << "Tail @ [" << T.row << "," << T.col << "]" << std::endl; // @DEBUG
            recordTail();
        }
    }

    return true;
}

//-----------------------------------------------------------------------------

void part_1(void)
{
    std::cout << std::endl;
    std::cout << "Loading and processing moves..." << std::endl;

    // Load moves
    std::string inputLine;
    while (!infile.eof())
    {
        getline(infile, inputLine);
        // Blank line means end of moves.
        if (inputLine.length() == 0)
        {
            break;
        }

        char move          = inputLine[0];
        std::string extent = inputLine.substr(2);
        int n              = std::stoi(extent);

        if (!processMove(move, n))
        {
            std::cout << "ERROR: Illegal move; " << inputLine << std::endl;
        }
    }
    std::cout << std::endl;
#if 0  // @DEBUG
   for (auto pt : tailMoves)
   {
       std::cout << "[" << pt.row << "," << pt.col << "], ";
   }
   std::cout << std::endl;
#endif // @DEBUG

    std::cout << "Total visited: " << tailMoves.size() << std::endl;
}

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    std::cout << std::endl;
    //-------------------------------------------------------------------------

    for (size_t k = 0; k < nKnots; ++k)
    {
        knots[k] = {0, 0};
    }

    part_1();

    //-------------------------------------------------------------------------

    std::cout << std::endl;
    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
