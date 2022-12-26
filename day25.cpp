//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <iterator>
#include <tuple>

//-----------------------------------------------------------------------------

// static const char cInputFileName[] = "test.txt";
static const char cInputFileName[] = "input25.txt";

static std::ifstream infile(cInputFileName);
static bool TEST = false;

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
    if ((line[0] == ';') && (line.find("DISABLE") != std::string::npos))
    {
        // std::cout << "--- INPUT DISABLED ---" << std::endl; // @DEBUG
        disblk = true;
        return true;
    }
    // Check if block disable is ended
    if ((line[0] == ';') && (line.find("ENABLE") != std::string::npos))
    {
        // std::cout << "--- INPUT ENABLED ---" << std::endl; // @DEBUG
        disblk = false;
        return true;
    }
    // Check if test input
    if (!disblk && (line[0] == ';') && (line.find("TEST") != std::string::npos))
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
    if (line[0] == ';')
    {
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------

const int64_t base = 5;

int64_t fromSnafu(std::string snafu)
{
    int64_t ret = 0;

    int64_t mult = 1;
    // if (TEST) std::cout << "  decimal = "; // @DEBUG
    for (int i = (int)snafu.size()-1; i >= 0; --i)
    {
        int64_t num = 0;

        if (snafu[i] == '=')
        {
            // if (TEST) std::cout << "(-2 * " << mult << ") + "; // @DEBUG
            num = -2 * mult;
        }
        else if (snafu[i] == '-')
        {
            // if (TEST) std::cout << "(-1 * " << mult << ") + "; // @DEBUG
            num = -1 * mult;
        }
        else
        {
            // 0, 1, or 2
            int64_t tmp = (snafu[i] - '0');
            // if (TEST) std::cout << "(" << tmp << " * " << mult << ") + "; // @DEBUG
            num = mult * tmp;
        }
 
        mult *= base;
        ret += num;
    }
    if (TEST) std::cout << "> " << snafu << " => " << ret << std::endl; // @DEBUG

    return ret;
}

std::string toSnafu(int64_t num)
{
    std::string ret;
    int64_t tmp1 = num;
    int64_t tmp2 = 0;
    int expand[32] = { 0 };

    int64_t log5Mult = log(num)/log(base);

    // if (TEST) std::cout << "log5Mult = " << log5Mult << std::endl; // @DEBUG

    // Convert to standard base-5
    while (log5Mult >= 0)
    {
        tmp2 = pow(base, log5Mult);
        tmp1 = num / tmp2;
        num -= tmp1 * tmp2;
        // if (TEST) std::cout << "  tmp2 = " << tmp2 << " tmp1 = " << tmp1 << " num = " << num << std::endl; // @DEBUG
        ret.push_back(tmp1 + '0');
        log5Mult--;
    }
    if (TEST) std::cout << "  Base5 = " << ret << std::endl; // @DEBUG

    // Convert to expanded SNAFU
    int expIx = 0; // Digit will be least-sig first, but so what.
    for (int i = ret.size() - 1; i >= 0; --i, expIx++)
    {
        char dig = ret[i];
        switch(dig)
        {
        case '0':
        case '1':
        case '2':
            expand[expIx] += dig - '0';
            break;

        case '3':
            expand[expIx] += -2;
            expand[expIx+1] = 1;
            break;

        case '4':
            expand[expIx] += -1;
            expand[expIx+1] = 1;
            break;

        default:
            break;
        }

        if (expand[expIx] == 3)
        {
            expand[expIx] = -2;
            expand[expIx+1] += 1;
        }
        else if (expand[expIx] == 4)
        {
            expand[expIx] = -1;
            expand[expIx+1] += 1;
        }
    }
    ret.clear();

    // Convert expanded SNAFU to SNAFU
    for ( ; expIx >= 0; --expIx)
    {
        switch(expand[expIx])
        {
        case 0:
        case 1:
        case 2:
            ret.push_back(expand[expIx] + '0');
            break;

        case -1:
            ret.push_back('-');
            break;

        case -2:
            ret.push_back('=');
            break;

        default:
            break;
        }
    }

    if (TEST) std::cout << "  ret = " << ret << std::endl; // @DEBUG

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

    TEST = false;

    int64_t dTotal = 0;
    std::string sTotal;

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

        // std::stringstream ipair(iline);
        std::string snafu;
        std::string tmp;
        int64_t decimal;

        // ipair >> snafu;
        // ipair >> tmp;
        // decimal = std::stoi(tmp);

        // if (TEST) std::cout << "> " << snafu << " => " << decimal << std::endl;  // @DEBUG

        snafu = iline;

        decimal = fromSnafu(snafu);
        dTotal += decimal;

        tmp = toSnafu(decimal);
    }

    std::cout << std::endl;
    sTotal = toSnafu(dTotal);

    std::cout << std::endl;
    std::cout << "Total: " << dTotal << " => " << sTotal << std::endl;
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

    // loadInputs();

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
