//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//-----------------------------------------------------------------------------

static const char cInputFileName[] = "test.txt";
// static const char cInputFileName[] = "input09.txt";

static std::ifstream infile(cInputFileName);

//-----------------------------------------------------------------------------

void printSomething(void)
{
    std::cout << "Something:" << std::endl;
    // @TODO
    printf("\n");
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
        // Skip blank lines.
        if (iline.length() == 0)
        {
            break;
        }

        // @TODO

    }

    std::cout << "Inputs info: " << std::endl;
}

//-----------------------------------------------------------------------------

void part_1(void)
{
    std::cout << "Processing..." << std::endl;

    // @TODO
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
    printSomething();

    part_1();
    printSomething();

    // part_2();
    // printSomething();

    // std::cout << "Results: " << std::endl;
    // // @TODO
    // std::cout << std::endl;

    //-------------------------------------------------------------------------

    std::cout << std::endl;
    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
