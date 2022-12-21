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
static bool TEST = false;

struct entry
{
    int val;
    bool moved;
};
static std::vector<entry> inList;  // initial input
static std::vector<entry> mixList; // after part 1
static size_t listSize = 0;

//-----------------------------------------------------------------------------

void printIntVec(std::vector<entry> v)
{
    for (auto x : v)
    {
        printf(" %2d", x.val);
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
    // Check if test input
    if (!disblk && (line[0] == '#') && (line.find("TEST") != std::string::npos))
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

        int num    = std::stoi(iline);
        entry data = {num, false};
        inList.push_back(data);
    }

    listSize = inList.size();

    std::cout << std::endl;
    std::cout << "Inputs info: input list length = " << listSize << std::endl;

    // @DEBUG
    if (TEST)
    {
        std::cout << "inList:";
        printIntVec(inList);
    }
}

//-----------------------------------------------------------------------------

// Assume there are duplicates in the data.
void part_1(void)
{
    std::cout << "Mixing original input list..." << std::endl;

    // Duplicate original list
    mixList = inList;
    mixList.reserve(listSize + 2);

    // @DEBUG
    if (TEST)
    {
        std::cout << std::endl;
        std::cout << "START mixList:";
        printIntVec(mixList);
        std::cout << "Size: " << mixList.size() << std::endl;
        std::cout << "Capacity: " << mixList.capacity() << std::endl;
    }

    auto cur = mixList.begin();
    int loc = 0;
    while (cur != mixList.end())
    {
        if (TEST)
            std::cout << std::endl; // @DEBUG

        // Move to first unmoved number
        loc = 0;
        for (cur = mixList.begin(); cur != mixList.end(); cur++, loc++)
        {
            if (TEST) printf("SEARCH Loc:%2d, Val:%2d\n", loc, cur->val); // @DEBUG
            if (!cur->moved) break;
        }

        if (loc >= (int)listSize)
        {
            break;
        }

        if (TEST) printf("Loc:%2d, Val:%2d\n", loc, cur->val); // @DEBUG

        // Nothing happens if 0
        if (0 == cur->val)
        {
            if (TEST) printf("0: NOP\n");
            cur->moved = true;
            // @DEBUG
            if (TEST)
            {
                std::cout << "mixList:";
                printIntVec(mixList);
            }
            continue;
        }

        // Calculate new location
        int newLoc = (loc + cur->val);
        if (newLoc < 0)
        {
            newLoc = newLoc + listSize;
        }
        else if (newLoc > 0)
        {
            // Need to insert BEFORE the next number
            newLoc++;
        }
        newLoc = newLoc % listSize;

        if (newLoc == 0)
        {
            // Add to the end
            if (TEST)
                printf("Add to end\n"); // @DEBUG
        }

        if (TEST)
            printf("move to index %2d\n", newLoc); // @DEBUG

        cur->moved = true;

        if (newLoc == 0)
        {
            // Add to the end
            if (TEST)
                printf("Add to end\n"); // @DEBUG
            mixList.push_back(*cur);
        }
        else
        {
            auto nx = std::next(mixList.begin(), newLoc);
            if (TEST)
                printf("move to before %2d\n", nx->val); // @DEBUG

            // Insert number to new location
            mixList.insert(nx, *cur);

            if (nx < cur) cur++;
        }

        // Delete number from the old location
        if (TEST) printf("Remove %2d from location %2d\n", cur->val, loc);
        mixList.erase(cur);
        if (TEST) printf("Loc:%2d, Val:%2d\n", loc, cur->val); // @DEBUG

        // @DEBUG
        if (TEST)
        {
            std::cout << "mixList:";
            printIntVec(mixList);
        }
    }

    // @DEBUG
    if (TEST)
    {
        std::cout << std::endl;
        std::cout << "END mixList:";
        printIntVec(mixList);
    }

    // Find the index of 0
    size_t i0 = 0;
    for (i0 = 0; i0 < listSize; ++i0)
    {
        if (mixList[i0].val == 0)
        {
            break;
        }
    }
    std::cout << "* 0 found at index " << i0 << " (" << mixList[i0].val << ")" << std::endl;

    int coord1Idx = (1000 + i0) % listSize;
    int coord2Idx = (2000 + i0) % listSize;
    int coord3Idx = (3000 + i0) % listSize;
    std::cout << "Indices: " << coord1Idx << ", " << coord2Idx << ", " << coord3Idx << std::endl;
    std::cout << "Coords: " << mixList[coord1Idx].val << ", "
                << mixList[coord2Idx].val << ", "
                << mixList[coord3Idx].val << std::endl;

    int sum = mixList[coord1Idx].val + mixList[coord2Idx].val + mixList[coord3Idx].val;

    std::cout << "Sum of coords: " << sum << std::endl;
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
