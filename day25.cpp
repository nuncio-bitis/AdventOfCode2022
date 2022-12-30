//-----------------------------------------------------------------------------

#include "utils.h"
#include <math.h>

//-----------------------------------------------------------------------------

static Utils *pUtils = nullptr;

static const char cInputFileName[] = "input25.txt";

static std::ifstream infile(cInputFileName);
static bool TEST = false;

//-----------------------------------------------------------------------------

const int64_t base = 5;

int64_t fromSnafu(std::string snafu)
{
    int64_t ret = 0;

    int64_t mult = 1;
    for (int i = (int)snafu.size()-1; i >= 0; --i)
    {
        int64_t num = 0;

        if (snafu[i] == '=')
        {
            num = -2 * mult;
        }
        else if (snafu[i] == '-')
        {
            num = -1 * mult;
        }
        else
        {
            // 0, 1, or 2
            int64_t tmp = (snafu[i] - '0');
            num = mult * tmp;
        }
 
        mult *= base;
        ret += num;
    }
    pUtils->DPRINTF("> [%s] => [%lld]", snafu.c_str(), ret); // @DEBUG

    return ret;
}

std::string toSnafu(int64_t num)
{
    std::string ret;
    int64_t tmp1 = num;
    int64_t tmp2 = 0;
    int expand[32] = { 0 };

    int64_t log5Mult = log(num)/log(base);

    // Convert to standard base-5
    while (log5Mult >= 0)
    {
        tmp2 = pow(base, log5Mult);
        tmp1 = num / tmp2;
        num -= tmp1 * tmp2;
        ret.push_back(tmp1 + '0');
        log5Mult--;
    }
    pUtils->DPRINTF("  Base5 = %s", ret.c_str()); // @DEBUG

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

    pUtils->DPRINTF("  ret = %s", ret.c_str()); // @DEBUG

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
        if (pUtils->ignoreLine(iline))
        {
            continue;
        }

        std::string snafu;
        std::string tmp;
        int64_t decimal;

        snafu = iline;

        decimal = fromSnafu(snafu);
        dTotal += decimal;

        tmp = toSnafu(decimal);
    }

    sTotal = toSnafu(dTotal);

    std::cout << std::endl;
    std::cout << "Total: " << dTotal << " => " << sTotal << std::endl;
}

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    pUtils = Utils::getInstance();

    std::cout << std::endl;
    part_1();

    // No part 2 on last day.

    //-------------------------------------------------------------------------

    std::cout << std::endl;
    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
