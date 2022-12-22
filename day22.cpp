//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <iterator>

//-----------------------------------------------------------------------------

static const char cInputFileName[] = "input22.txt";

static std::ifstream infile(cInputFileName);
static bool TEST = false;

struct mapRow
{
    std::string data;
    uint lpos; // Left-most position
};
static std::vector<mapRow> grid;
static std::string directions;

enum Facing
{
    eRight = 0,
    eDown  = 1,
    eLeft  = 2,
    eUp    = 3,
    eInvalid = -1
};
static const std::vector<std::string> FaceStr = {
    "Right",
    "Down",
    "Left",
    "Up"
};

struct Move
{
    size_t dist;
    char   dir;
};

static int row = 0;
static int col = 0;
static Facing fac = eRight; // Facing is 0 for right (>), 1 for down (v), 2 for left (<), and 3 for up (^)

//-----------------------------------------------------------------------------

void printGrid(void)
{
    if (TEST)
    {
        printf("\n");
        for (uint r=0; r < grid.size(); ++r)
        {
            for (uint c=0; c < grid[r].data.size(); ++c)
            {
                printf("%c", grid[r].data[c]);
            }
            printf("\n");
        }
    }
}

//-----------------------------------------------------------------------------

static bool disblk = false;

bool ignoreLine(std::string line)
{
    // Skip blank lines.
    if (line.length() == 0)
    {
        return true;
    }
    // Check if a block of input has been disabled
    if ((line[0] == ';') && (line.find("DISABLE") != std::string::npos))
    {
        // std::cout << "--- INPUT DISABLED ---" << std::endl; // @DEBUG
        disblk = true;
        return true;
    }
    // Check if block disable is ended
    if ((line[0] == ';') && (line.find("ENABLE") != std::string::npos))
    {
        // std::cout << "--- INPUT ENABLED ---" << std::endl; // @DEBUG
        disblk = false;
        return true;
    }
    // Check if test input
    if (!disblk && (line[0] == ';') && (line.find("TEST") != std::string::npos))
    {
        std::cout << "--- PROCESSING TEST INPUT ---" << std::endl; // @DEBUG
        TEST = true;
        return true;
    }
    // Skip disabled block
    if (disblk)
    {
        return true;
    }
    // Skip comment lines.
    if (line[0] == ';')
    {
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------

void loadInputs(void)
{
    if (!infile)
    {
        std::cerr << "Error: unable to open input file: " << cInputFileName << std::endl;
        exit(1);
    }

    TEST = false;

    std::cout << "Loading inputs..." << std::endl;
    std::string iline;
    while (!infile.eof())
    {
        getline(infile, iline);
        // Check if line should be ignored.
        if (ignoreLine(iline))
        {
            continue;
        }

        if ((iline[0] >= '0') && (iline[0] <= '9'))
        {
            // Final input is the directions list
            directions = iline;
            break;
        }

        mapRow row;
        row.data = iline;
        row.lpos = iline.find_last_of(' ') + 1;
        grid.push_back(row);
    }
    infile.close();

    printGrid();

    std::cout << std::endl;
    std::cout << "Inputs info: " << grid.size() << " rows input." << std::endl;
    std::cout << "    Directions list size: " << directions.size() << std::endl;
}

//-----------------------------------------------------------------------------

void where(void)
{
    std::cout << "[" << row << "," << col << "] = '" << grid[row].data[col]
              << "' Facing: " << FaceStr[fac] << " (" << (int)fac << ")"
              << std::endl;
}

//-----------------------------------------------------------------------------

Facing updateFacing(char dir)
{
    Facing ret = eRight;

    if (dir == '\0')
    {
        return fac;
    }

    switch (fac)
    {
    case eRight:
    {
        switch(dir)
        {
            case 'R': ret = eDown; break;
            case 'L': ret = eUp; break;
            default : ret = fac; break;
        }
        break;
    }

    case eLeft:
    {
        switch(dir)
        {
            case 'R': ret = eUp; break;
            case 'L': ret = eDown; break;
            default : ret = fac; break;
        }
        break;
    }

    case eUp:
    {
        switch(dir)
        {
            case 'R': ret = eRight; break;
            case 'L': ret = eLeft; break;
            default : ret = fac; break;
        }
        break;
    }

    case eDown:
    {
        switch(dir)
        {
            case 'R': ret = eLeft; break;
            case 'L': ret = eRight; break;
            default : ret = fac; break;
        }
        break;
    }

    default:
        break;
    }

    return ret;
}

//-----------------------------------------------------------------------------

static std::size_t pos{0};
Move getMove(void)
{
    Move m = {0, eInvalid};

    if (pos >= directions.size())
    {
        return m;
    }

    // If the current character is a digit, then we convert this and the following characters to an int
    if (std::isdigit(directions[pos]))
    {
        size_t i;
        // The stoi function will inform us, how many characters have been converted.
        m.dist = std::stoi(directions.substr(pos), &i);
        pos += i;
    }

    m.dir  = directions[pos++];

    if (TEST)
    {
        std::cout << ">> Pos: " << pos << " of " << directions.size() << std::endl;
        std::cout << ">> Next move: " << m.dist
        << ", facing " << FaceStr[fac]
        << (m.dir ? ", turn " : "") << m.dir
        << std::endl;
    }

    return m;
}

//-----------------------------------------------------------------------------

void performMove(Move m)
{
    int nr = row;
    int nc = col;

    grid[nr].data[nc] = '@';

    for (uint i = 0; i < m.dist; ++i)
    {
        // std::cout << "-- row: " << row << ", col: " << col << std::endl; // @DEBUG

        // Compute where we should go next
        switch (fac)
        {
        case eRight:
        {
            // Right, with wrap
            // Easy because the right side is always the size of the string.
            if (++nc >= (int)grid[nr].data.size())
            {
                nc = grid[nr].lpos;
            }
            break;
        }

        case eLeft:
        {
            // Left, with wrap
            // Easy because the right side is always the size of the string.
            if (--nc < (int)grid[nr].lpos)
            {
                nc = grid[nr].data.size() - 1;
            }
            break;
        }

        case eUp:
        {
            nr--;
            // Vertical wrapping is tougher...
            if ((nr < 0) || (grid[nr].data[nc] == ' '))
            {
                // Must move down until we find another space, or the end of rows, or nc is past the end of a row.
                for (++nr; ((nc < (int)grid[nr].data.size()) &&
                            (nr <= (int)grid.size()) &&
                            (grid[nr].data[nc] != ' ') ); ++nr)
                {
                    ;
                //    std::cout << "-- nr: " << nr << ", nc: " << nc << ", Total Rows: " << grid.size() << std::endl; // @DEBUG
                }
                nr--; // Go back to non-blank (or back onto grid)
            }
            break;
        }

        case eDown:
        {
            nr++;
            // Vertical wrapping is tougher...
            if ((nr > (int)grid.size()-1) || (grid[nr].data[nc] == ' '))
            {
                // Must move up until we find another space, or the start of rows, or nc is past the end of a row.
                for (--nr; ((nc < (int)grid[nr].data.size()) &&
                            (nr > 0) &&
                            (grid[nr].data[nc] != ' ') ); --nr)
                {
                    ;
                    // std::cout << "-- nr: " << nr << ", nc: " << nc << ", Total Rows: " << grid.size() << std::endl; // @DEBUG
                }
                nr++; // Go back to non-blank (or back onto grid)
            }
            break;
        }

        default:
            break;
        }

        // std::cout << "++ nr: " << nr << ", nc: " << nc << std::endl; // @DEBUG

        // Check if the next square is a wall
        if (grid[nr].data[nc] == '#')
        {
            break;
        }

        grid[nr].data[nc] = 'X';
        row = nr;
        col = nc;
    }
    // std::cout << "++ row: " << row << ", col: " << col << std::endl; // @DEBUG

}

//-----------------------------------------------------------------------------

void part_1(void)
{
    std::cout << "PART 1: Processing..." << std::endl;

    // Get start
    row = 0;
    col = grid[0].lpos;
    fac = eRight;
    std::cout << "START: ";
    where();

    Move m = getMove();
    while (m.dir != eInvalid)
    {
        // Perform move...
        performMove(m);
        // printGrid(); // @DEBUG

        // Update facing direction from turn direction
        fac = updateFacing(m.dir);

        // Where are we now?
        if (TEST) where();

        // Get next move
        m = getMove();
    }

    // printGrid();

    std::cout << std::endl;
    std::cout << "END: ";
    where();

    std::cout << std::endl;
    // Compute password:
    // Rows and columns start at 1, so add 1.
    // password = 1000 * row + 4 * column + facing
    int pw = 1000 * (row + 1) + 4 * (col + 1) + (int)fac;
    std::cout << "PASSWORD = " << pw << std::endl;
}

//-----------------------------------------------------------------------------

// OMFG - the map is a cube!
void part_2(void)
{
    std::cout << "PART 2: Processing..." << std::endl;

    // @TODO
    std::cout << "    FUCK THIS SHIT - I'M OUTTA HERE." << std::endl;
}

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    std::cout << std::endl;
    //-------------------------------------------------------------------------

    loadInputs();

    std::cout << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    part_1();

    std::cout << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    part_2();

    //-------------------------------------------------------------------------

    std::cout << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << std::endl;

    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
