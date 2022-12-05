//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <tuple>

#include "dataDefs.h"

//-----------------------------------------------------------------------------

// static const char cInputFileName[] = "test.txt";
static const char cInputFileName[] = "input04.txt";

//-----------------------------------------------------------------------------

// Count fully-contained ranges
void part_1(void)
{
    std::cout << "Loading task pairs..." << std::endl;

    std::ifstream infile(cInputFileName);
    if (!infile)
    {
        std::cerr << "Error: unable to open input file: " << cInputFileName << std::endl;
        return;
    }

    int total = 0;

    std::string inputLine;
    while (!infile.eof())
    {
        getline(infile, inputLine);

        // Skip blank line.
        if (inputLine.length() == 0)
        {
            continue;
        }

        // @TODO
        int elf1L = 0;
        int elf1H = 0;
        int elf2L = 0;
        int elf2H = 0;

        sscanf(inputLine.c_str(), "%d-%d,%d-%d", &elf1L, &elf1H, &elf2L, &elf2H);

        if ((elf1H - elf1L) >= (elf2H - elf2L))
        {
            // Elf1 has the larger range
            if ((elf2L >= elf1L) && (elf2H <= elf1H))
            {
                total++;
            }
        }
        else
        {
            // Elf2 has the larger range
            if ((elf1L >= elf2L) && (elf1H <= elf2H))
            {
                total++;
            }
        }

        // @TODO

        inputLine.clear();
    }
    infile.close();

    std::cout << std::endl;
    std::cout << "Total overlaps: " << total << std::endl;
    std::cout << std::endl;
}

//-----------------------------------------------------------------------------

// Count overlapping ranges
void part_2(void)
{
    std::cout << "Loading task pairs..." << std::endl;

    std::ifstream infile(cInputFileName);
    if (!infile)
    {
        std::cerr << "Error: unable to open input file: " << cInputFileName << std::endl;
        return;
    }

    int total = 0;

    std::string inputLine;
    while (!infile.eof())
    {
        getline(infile, inputLine);

        // Skip blank line.
        if (inputLine.length() == 0)
        {
            continue;
        }

        // @TODO
        int elf1L = 0;
        int elf1H = 0;
        int elf2L = 0;
        int elf2H = 0;

        sscanf(inputLine.c_str(), "%d-%d,%d-%d", &elf1L, &elf1H, &elf2L, &elf2H);

        if (elf1L < elf2L)
        {
            // Elf1's range starts lower. Check if Elf2's range starts before his ends.
            if (elf2L <= elf1H)
            {
                total++;
            }
        }
        else
        {
            // Elfs's range starts lower. Check if Elf1's range starts before his ends.
            if (elf1L <= elf2H)
            {
                total++;
            }
        }

        // @TODO

        inputLine.clear();
    }
    infile.close();

    std::cout << std::endl;
    std::cout << "Total overlaps: " << total << std::endl;
    std::cout << std::endl;
}

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    std::cout << std::endl;
    //-------------------------------------------------------------------------

    // part_1();
    part_2();

    //-------------------------------------------------------------------------

    std::cout << std::endl;
    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
