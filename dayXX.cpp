//-----------------------------------------------------------------------------

#include "utils.h"

//-----------------------------------------------------------------------------

static Utils *pUtils = nullptr;
static const char cInputFileName[] = "input25.txt";

static std::ifstream infile(cInputFileName);
static bool TEST = false;

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
        if (pUtils->ignoreLine(iline))
        {
            continue;
        }
        TEST = pUtils->isTest();

        // @TODO

        pUtils->DPRINTF(" > %s", iline.c_str()); // @DEBUG
    }

    std::cout << std::endl;
    std::cout << "Inputs info: " << std::endl;
    std::cout << "  " << pUtils->nInputLines() << " input lines processed." << std::endl;
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

    pUtils = Utils::getInstance();
    loadInputs();

    // std::cout << std::endl;
    // part_1();

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
