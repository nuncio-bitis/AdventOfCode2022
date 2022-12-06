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
static const char cInputFileName[] = "input06.txt";

static std::ifstream infile(cInputFileName);

static std::string msg;
static int sop = 0;
static int som = 0;

//-----------------------------------------------------------------------------

bool isDup(unsigned int pos, unsigned int end)
{
    bool ret = false;
    if ((end - pos) == 1)
    {
        return (msg[pos] == msg[end]);
    }
    else
    {
        for (unsigned int i = pos+1; i <= end; ++i)
        {
            ret = ret || (msg[pos] == msg[i]);
        }
        ret = ret || isDup(pos+1, end);
    }
    return ret;
}

//-----------------------------------------------------------------------------

void part_1(void)
{
    std::cout << "Processing input line..." << std::endl;

    unsigned i = 0;
    int score  = 0;
    for (i = 0; i < msg.size(); ++i)
    {
        for (unsigned int j = i + 1; j < i + 4; ++j)
        {
            if (msg[i] != msg[j])
            {
                score++;
            }
            else
            {
                score = 0;
                break;
            }
        }

        if (score > 6) // sum(1..3)
        {
            sop = i + 2;
            break;
        }
    }
}

//-----------------------------------------------------------------------------

void part_2(void)
{
    std::cout << "Processing message..." << std::endl;

    int score  = 0;
    for (unsigned int i = 0; i < msg.size()-14; ++i)
    {
        if (!isDup(i, (i + 13)))
        {
            som = i + 14;
            break;
        }
    }
}

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    std::cout << std::endl;
    //-------------------------------------------------------------------------

    while (!infile.eof())
    {
        getline(infile, msg);
        // Skip blank lines.
        if (msg.length() == 0)
        {
            break;
        }
        // Skip commented lines
        if (msg[0] == '#')
        {
            continue;
        }

        std::cout << msg << std::endl;
        std::cout << "Size: " << msg.size() << std::endl;
        part_1();
        std::cout << "*** SOP @ " << sop << std::endl;
        part_2();
        std::cout << "*** SOM @ " << som << std::endl;

        std::cout << std::endl;
    }

    infile.close();

    //-------------------------------------------------------------------------

    std::cout << std::endl;
    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
