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
static const char cInputFileName[] = "input05.txt";

static std::ifstream infile(cInputFileName);

// One stack is a vector of characters
typedef std::vector<char> stack_type;
// List of stacks
static std::vector<stack_type> stacks;

//-----------------------------------------------------------------------------

void printStacks(void)
{
    std::cout << "Stacks:" << std::endl;
    for (size_t i = 0; i < stacks.size(); ++i)
    {
        stack_type stk = stacks[i];
        printf("%02zu : ", i+1);
        for (auto crate : stk)
        {
            printf("%c ", crate);
        }
        printf("\n");
    }
    printf("\n");
}

//-----------------------------------------------------------------------------

void loadCrates(void)
{
    if (!infile)
    {
        std::cerr << "Error: unable to open input file: " << cInputFileName << std::endl;
        exit(1);
    }

    size_t numStacks = 0;

    std::cout << "Loading crates from input..." << std::endl;
    bool firstLine = true;
    std::string inputLine;
    while (!infile.eof())
    {
        getline(infile, inputLine);
        // Blank line means end of initial configuration of crates.
        if (inputLine.length() == 0)
        {
            break;
        }

        // Use first line to compute number of stacks
        // Each stack takes 4 chars: "[X] "
        if (firstLine)
        {
            firstLine = false;
            numStacks = (inputLine.size() + 1) / 4;
            stacks.resize(numStacks);
        }

        // Load crates into stacks
        for (size_t i = 0; i < numStacks; ++i)
        {
            size_t stackPos = (i * 4) + 1;
            char crate = inputLine[stackPos];
            // Ignore if crate is a number (last line)
            if ((crate != ' ') && !((crate >= '0') && (crate <= '9')) )
            {
                stacks[i].emplace(stacks[i].begin(), crate);
            }
        }
    }

    std::cout << "Num stacks: " << stacks.size() << std::endl;
}

//-----------------------------------------------------------------------------

// Premise...
void part_1(void)
{
    std::cout << "Rearranging crates..." << std::endl;

    std::string inputLine;
    while (!infile.eof())
    {
        getline(infile, inputLine);
        // Blank line means end of initial configuration of crates.
        if (inputLine.length() == 0)
        {
            break;
        }

        int nMove = 0;
        int from = 0;
        int to = 0;
        sscanf(inputLine.c_str(), "move %d from %d to %d", &nMove, &from, &to);
        printf("* Move %d from %d to %d ...\n", nMove, from, to);

        for (int n = 0; n < nMove; ++n)
        {
            char crate = stacks[from-1].back();
            stacks[from-1].pop_back();
            stacks[to-1].push_back(crate);
        }
    }
}

//-----------------------------------------------------------------------------

// Premise...
void part_2(void)
{
    std::cout << "Rearranging crates..." << std::endl;

    std::string inputLine;
    while (!infile.eof())
    {
        getline(infile, inputLine);
        // Blank line means end of initial configuration of crates.
        if (inputLine.length() == 0)
        {
            break;
        }

        int nMove = 0;
        int from = 0;
        int to = 0;
        sscanf(inputLine.c_str(), "move %d from %d to %d", &nMove, &from, &to);
        printf("* Move %d from %d to %d ...\n", nMove, from, to);

        stack_type temp;
        for (int n = 0; n < nMove; ++n)
        {
            char crate = stacks[from-1].back();
            stacks[from-1].pop_back();
            temp.push_back(crate);
        }
        for (int n = 0; n < nMove; ++n)
        {
            char crate = temp.back();
            temp.pop_back();
            stacks[to-1].push_back(crate);
        }
        printStacks();
        std::cout << "============================================" << std::endl;
    }
}

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    std::cout << std::endl;
    //-------------------------------------------------------------------------

    loadCrates();

    std::cout << "Stacks:" << std::endl;
    for (size_t i = 0; i < stacks.size(); ++i)
    {
        stack_type stk = stacks[i];
        printf("%02zu : ", i+1);
        for (auto crate : stk)
        {
            printf("%c ", crate);
        }
        printf("\n");
    }
    printf("\n");

    // part_1();
    part_2();

    printStacks();
    std::cout << "Crates on top: ";
    for (auto stk : stacks)
    {
        std::cout << stk.back();
    }
    std::cout << std::endl;

    //-------------------------------------------------------------------------

    std::cout << std::endl;
    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
