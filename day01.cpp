
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//-----------------------------------------------------------------------------

static const char cInputFileName[] = "input.txt";

static std::vector<int> elves;

//-----------------------------------------------------------------------------

int loadInput(void)
{
    std::cout << "Loading elves' data..." << std::endl;

    std::ifstream infile(cInputFileName);
    if (!infile)
    {
        std::cerr << "Error: unable to open input file: " << cInputFileName << std::endl;
        return -1;
    }

    bool nextElf = true;
    int thisElf;
    int total  = 0;
    int nElves = 0;
    while (!infile.eof())
    {
        std::string line;
        getline(infile, line);
        if (line.length() < 1)
        {
            thisElf = total;
            elves.push_back(thisElf);
            nextElf = true;
            total   = 0;
            nElves++;
            continue;
        }
        if (nextElf)
        {
            int cals = std::stoi(line);
            total += cals;
            thisElf.items.push_back(cals);
        }
    }
    infile.close();
    return nElves;
}

//-----------------------------------------------------------------------------

void part_1(void)
{
    std::cout << std::endl << "********** DAY 1, PART 1 **********" << std::endl;
    int maxElf = 0;
    int nElf   = 1;
    int elfIdx = 1;
    for (auto elf : elves)
    {
        // std::cout << "Elf total cals = " << elf << std::endl;
        if (elf > maxElf)
        {
            maxElf = elf;
            nElf   = elfIdx;
        }
        elfIdx++;
    }
    std::cout << "Elf #" << nElf << " carries the most calories = " << maxElf << std::endl;
}

//-----------------------------------------------------------------------------

void part_2(void)
{
    std::cout << std::endl << "********** DAY 1, PART 2 **********" << std::endl;

    std::sort(elves.begin(), elves.end(), std::greater<int>());

    // Add up the first three
    int totalCals = elves[0] + elves[1] + elves[2];
    std::cout << "Sum of the top 3 elves' calories = " << totalCals << std::endl;
}

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    std::cout << std::endl;
    //-------------------------------------------------------------------------

    int nElves = loadInput();
    if (nElves < 0)
    {
        std::cout << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Number of elves: " << nElves << std::endl;

    part_1();
    part_2();

    //-------------------------------------------------------------------------

    std::cout << std::endl;
    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
