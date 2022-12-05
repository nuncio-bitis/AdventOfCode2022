//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <tuple>

//-----------------------------------------------------------------------------

// static const char cInputFileName[] = "test.txt";
static const char cInputFileName[] = "input3.txt";

//-----------------------------------------------------------------------------

int getPriority(char ch)
{
    if ((ch >= 'a') && (ch <= 'z'))
    {
        return (ch - 'a' + 1);
    }
    else if ((ch >= 'A') && (ch <= 'Z'))
    {
        return (ch - 'A' + 27);
    }
    else
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------

void part_1(void)
{
    std::cout << "Loading inventories..." << std::endl;

    std::ifstream infile(cInputFileName);
    if (!infile)
    {
        std::cerr << "Error: unable to open input file: " << cInputFileName << std::endl;
        return;
    }

    int priTotal = 0;

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
        int cSize = inputLine.length();
        std::cout << "*** (" << cSize << ") " << inputLine << std::endl;
        for (int i = 0; i < (cSize/2); ++i)
        {
            char ch = inputLine[i];
            int last = inputLine.find_last_of(ch);
            if ((last >= cSize/2) && (i != last))
            {
                std::cout << "\t Found " << ch << " at " << i << " and " << last
                        << " => " << getPriority(ch) << std::endl;
                priTotal += getPriority(ch);
                break;
            }
        }
        // @TODO

        inputLine.clear();
    }
    infile.close();

    std::cout << std::endl;
    std::cout << "Priority total: " << priTotal << std::endl;
    std::cout << std::endl;
}

//-----------------------------------------------------------------------------

void part_2(void)
{
    std::cout << "Loading inventories..." << std::endl;

    std::ifstream infile(cInputFileName);
    if (!infile)
    {
        std::cerr << "Error: unable to open input file: " << cInputFileName << std::endl;
        return;
    }

    int priTotal = 0;

    std::string inputLine1;
    std::string inputLine2;
    std::string inputLine3;
    while (!infile.eof())
    {
        getline(infile, inputLine1);
        getline(infile, inputLine2);
        getline(infile, inputLine3);

        // Skip blank line.
        if (inputLine1.length() == 0)
        {
            continue;
        }

        // @TODO
        int cSize1 = inputLine1.length();
        for (int i = 0; i < cSize1; ++i)
        {
            char ch = inputLine1[i];
            int pos2 = inputLine2.find_first_of(ch);
            int pos3 = inputLine3.find_first_of(ch);
            if ((pos2 != std::string::npos) && (pos3 != std::string::npos))
            {
                std::cout << "\t Found " << ch << " at " << i << ", " << pos2
                        << ", and " << pos3 << " => " << getPriority(ch) << std::endl;
                priTotal += getPriority(ch);
                break;
            }
        }
        // @TODO

        inputLine1.clear();
        inputLine2.clear();
        inputLine3.clear();
    }
    infile.close();

    std::cout << std::endl;
    std::cout << "Priority total: " << priTotal << std::endl;
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
