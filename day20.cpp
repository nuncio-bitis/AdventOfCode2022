//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <iterator>

//-----------------------------------------------------------------------------

// static const char cInputFileName[] = "test.txt";
static const char cInputFileName[] = "input20.txt";

static std::ifstream infile(cInputFileName);

static std::vector<int> inList;  // initial input
static std::vector<int> mixList; // after part 1

//-----------------------------------------------------------------------------

void printIntVec(std::vector<int> v)
{
    for (auto x : v)
    {
        printf(" %2d", x);
    }
    std::cout << std::endl;
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
    if ((line[0] == '#') && (line.find("DISABLE") != std::string::npos))
    {
        // std::cout << "--- INPUT DISABLED ---" << std::endl; // @DEBUG
        disblk = true;
        return true;
    }
    // Check if block disable is ended
    if ((line[0] == '#') && (line.find("ENABLE") != std::string::npos))
    {
        // std::cout << "--- INPUT ENABLED ---" << std::endl; // @DEBUG
        disblk = false;
        return true;
    }
    // Skip disabled block
    if (disblk)
    {
        return true;
    }
    // Skip comment lines.
    if (line[0] == '#')
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

        int num = std::stoi(iline);
        inList.push_back(num);
    }

    std::cout << std::endl;
    std::cout << "Inputs info: input list length = " << inList.size() << std::endl;
}

//-----------------------------------------------------------------------------

void part_1(void)
{
    std::cout << std::endl;
    std::cout << "Mixing original input list..." << std::endl;

    mixList = inList;
    std::cout << "Inputs info: initial mix list length = " << mixList.size() << std::endl;

    int coord1Idx;
    int coord2Idx;
    int coord3Idx;

    // std::cout << "inList:";
    // printIntVec(inList);

    for (size_t i = 0; i < inList.size(); ++i)
    {
        int num = inList[i];

        // Nothing happens if 0
        if (0 == num)
        {
            continue;
        }

        // std::cout << std::endl;

        // Find number in new list
        auto cur = mixList.begin();
        int loc = 0;
        while (cur != mixList.end())
        {
            if (*cur == num)
            {
                break;
            }
            cur++;
            loc++;
        }
        // printf("%2d: Found %2d at %d\n", num, *cur, loc);
        // printf("cur: %2d\n", *cur);

        // Calculate new location
        int newLoc = (loc + num);
        // printf("move to index %2d\n", newLoc);
        if (newLoc < 0)
        {
            newLoc = newLoc + mixList.size();
        }
        else if (newLoc > 0)
        {
            newLoc++;
        }
        newLoc = newLoc % mixList.size();
        // printf("move to index %2d\n", newLoc);

        auto nx = std::next(mixList.begin(), newLoc);
        if (newLoc == 0)
        {
            // Add to the end
            // printf("Add to end\n");
            mixList.push_back(num);
        }
        else
        {
            // printf("move to before %2d\n", *nx);
            // Insert number to new location
            mixList.insert(nx, num);

            // Account for old number being moved.
            if (newLoc < loc) loc++;
        }

        // std::cout << "mixList:";
        // printIntVec(mixList);

        // Delete number from the old location
        // printf("Remove %2d from location %2d\n", num, loc);
        cur = std::next(mixList.begin(), loc);
        mixList.erase(cur);

        // std::cout << "mixList:";
        // printIntVec(mixList);
    }

    // std::cout << "mixList:";
    // printIntVec(mixList);

    // Find the index of 0
    int i0 = 0;
    for (i0 = 0; i0 < mixList.size(); ++i0)
    {
        if (mixList[i0] == 0)
        {
            break;
        }
    }
    std::cout << "* 0 found at index " << i0 << std::endl;

    coord1Idx = (1000 + i0) % mixList.size();
    coord2Idx = (2000 + i0) % mixList.size();
    coord3Idx = (3000 + i0) % mixList.size();
    std::cout << "Indices: " << coord1Idx << ", " << coord2Idx << ", " << coord3Idx << std::endl;
    std::cout << "Coords: "
              << mixList[coord1Idx] << ", "
              << mixList[coord2Idx] << ", "
              << mixList[coord3Idx] << std::endl;

    std::cout << "Sum of coords: "
              << (mixList[coord1Idx] + mixList[coord2Idx] + mixList[coord3Idx])
              << std::endl;

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
    std::cout << std::endl;
    //-------------------------------------------------------------------------

    loadInputs();

    std::cout << std::endl;
    part_1();

    // std::cout << std::endl;
    // part_2();

    // std::cout << "Results: " << std::endl;
    // // @TODO
    // std::cout << std::endl;

    //-------------------------------------------------------------------------

    std::cout << std::endl;
    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
