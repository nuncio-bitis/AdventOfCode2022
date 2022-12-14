//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>

//-----------------------------------------------------------------------------

// static const char cInputFileName[] = "test.txt";
static const char cInputFileName[] = "input13.txt";

static std::ifstream infile(cInputFileName);

// typedef std::pair<std::string, std::string> pktPair;

//-----------------------------------------------------------------------------

void printIntVec(std::vector<int> v)
{
    for (auto x : v)
    {
        std::cout << " " << x;
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

std::size_t replace_all(std::string &inout, std::string what, std::string with)
{
    std::size_t count{};
    for (std::string::size_type pos{}; inout.npos != (pos = inout.find(what.data(), pos, what.length())); pos += with.length(), ++count)
    {
        inout.replace(pos, what.length(), with.data(), with.length());
    }
    return count;
}

std::vector<int> strip(std::string &str)
{
    std::vector<int> ret;

    replace_all(str, "[", " ");
    replace_all(str, "]", " ");
    replace_all(str, ",", " ");
    // std::cout << str << std::endl; // @DEBUG

    std::istringstream input(str);
    int x;
    while (input >> x)
    {
        ret.push_back(x);
    }

    return ret;
}

//-----------------------------------------------------------------------------

void part_1(void)
{
    if (!infile)
    {
        std::cerr << "Error: unable to open input file: " << cInputFileName << std::endl;
        exit(1);
    }

    size_t pairNum = 1;
    int total      = 0;

    std::cout << "Parsing pairs..." << std::endl;
    std::string iline;
    while (!infile.eof())
    {
        getline(infile, iline);
        // Check if line should be ignored.
        if (ignoreLine(iline))
        {
            continue;
        }
        // std::cout << " > " << iline << std::endl; // @DEBUG

        // Got first of pair
        size_t str1len = iline.size();
        std::vector<int> first = strip(iline);

        // Get second of pair
        getline(infile, iline);
        size_t str2len = iline.size();
        // std::cout << " > " << iline << std::endl; // @DEBUG
        std::vector<int> second = strip(iline);

        // @DEBUG
        std::cout << "* 1st:";
        printIntVec(first);
        std::cout << "* 2nd:";
        printIntVec(second);
        // @DEBUG

        // Get min size
        size_t minsz;
        if (first.size() <= second.size())
        {
            minsz = first.size();
        }
        else
        {
            minsz = second.size();
        }

        // Compare vectors
        bool unordered = false;
        bool allEqual = true;
        size_t i       = 0;
        for ( ; i < minsz; ++i)
        {
            if (first.size() == 0)
            {
                break;
            }
            if (second.size() == 0)
            {
                unordered = true;
                break;
            }
            if (first[i] > second[i])
            {
                unordered = true;
                allEqual  = false;
                break;
            }
            else if (first[i] < second[i])
            {
                allEqual = false;
            }
        }
        if (allEqual && (i < first.size()))
        {
            unordered = true;
        }
        if ((first.size() == 0) && (second.size() == 0) && (str1len > str2len))
        {
            unordered = true;
        }
        if (!unordered)
        {
            if (first.size() <= second.size())
            {
                std::cout << "* Pair " << pairNum << " is in order." << std::endl;
                total += pairNum;
            }
        }

        pairNum++;
        std::cout << "---" << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Compared " << pairNum-1 << " pairs." << std::endl;
    std::cout << "Total of ordered pairs: " << total << std::endl;
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
