//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>

//-----------------------------------------------------------------------------

// static const char cInputFileName[] = "test.txt";
static const char cInputFileName[] = "input10.txt";

static std::ifstream infile(cInputFileName);

static uint cycle = 1;
static int X      = 1;
static int ss     = 1;

static std::vector<int> sigStrengths;

// 6 rows of 40 pixels
static char crt[6*40];
static uint px = 0;

//-----------------------------------------------------------------------------

void printCRT(void)
{
    for (uint r = 0; r < 6; ++r)
    {
        for (uint c = 0; c < 40; ++c)
        {
            printf("%c", crt[r * 40 + c]);
        }
        printf("\n");
    }
}

char drawPixel(int X, int px)
{
    int p = px % 40;
    if ((p >= X-1) && (p <= (X+1)))
    {
        crt[px] = '#';
    }
    else
    {
        crt[px] = '.';
    }
    return crt[px];
}

//-----------------------------------------------------------------------------

void part_1(void)
{
    std::cout << "Processing inputs..." << std::endl;
    if (!infile)
    {
        std::cerr << "Error: unable to open input file: " << cInputFileName << std::endl;
        exit(1);
    }

    std::string iline;
    while (!infile.eof())
    {
        getline(infile, iline);
        // Blank lines.
        if (iline.length() == 0)
        {
            // Start over
            cycle = 1;
            X     = 1;
            ss    = 1;
            sigStrengths.clear();
            std::cout << std::endl; // @DEBUG
            continue;
        }
        // Ignore comment lines
        if (iline[0] == '#')
        {
            continue;
        }

        printf("%-10s", iline.c_str()); // @DEBUG

        std::istringstream input(iline);
        std::string cmd;
        int arg = 0;
        input >> cmd;
        input >> arg;

        int ss = 1;
        if (cmd == "noop")
        {
            ss = X * cycle;
            sigStrengths.push_back(ss);
            printf("%3d : arg=%d X=%d ss=%d => %d:%c\n", cycle, arg, X, ss, px, drawPixel(X, px)); // @DEBUG
        }
        else if (cmd == "addx")
        {
            char p = drawPixel(X, px);
            ss = X * cycle;
            sigStrengths.push_back(ss);
            printf("%3d : arg=%d X=%d ss=%d => %d:%c\n", cycle, arg, X, ss, px, p); // @DEBUG
            cycle++;
            px++;

            printf("%-10s", ""); // @DEBUG
            p  = drawPixel(X, px);
            ss = X * cycle;
            sigStrengths.push_back(ss);
            X += arg;
            printf("%3d : arg=%d X=%d ss=%d => %d:%c\n", cycle, arg, X, ss, px, p); // @DEBUG
        }
        cycle++;
        px++;
    }

    std::cout << std::endl;
    std::cout << "sigStrengths[20] = "  << sigStrengths[20 - 1]  << std::endl
              << "sigStrengths[60] = "  << sigStrengths[60 - 1]  << std::endl
              << "sigStrengths[100] = " << sigStrengths[100 - 1] << std::endl
              << "sigStrengths[140] = " << sigStrengths[140 - 1] << std::endl
              << "sigStrengths[180] = " << sigStrengths[180 - 1] << std::endl
              << "sigStrengths[220] = " << sigStrengths[220 - 1] << std::endl;

    int totalSS = sigStrengths[20  - 1]
                + sigStrengths[60  - 1]
                + sigStrengths[100 - 1]
                + sigStrengths[140 - 1]
                + sigStrengths[180 - 1]
                + sigStrengths[220 - 1];

    std::cout << std::endl;
    std::cout << "Total SS: " << totalSS << std::endl;
}

//-----------------------------------------------------------------------------

void part_2(void)
{
    std::cout << "Processing..." << std::endl;

    // @TODO
}

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    //-------------------------------------------------------------------------

    std::cout << std::endl;
    part_1();

    // Part 2
    std::cout << std::endl;
    printCRT();

    //-------------------------------------------------------------------------

    std::cout << std::endl;
    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
