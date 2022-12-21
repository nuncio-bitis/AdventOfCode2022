//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <iterator>
#include <map>

//-----------------------------------------------------------------------------

static const char cInputFileName[] = "input21.txt";

static std::ifstream infile(cInputFileName);
static bool TEST = false;

struct job
{
    int64_t result;
    std::string left;
    char op;
    std::string right;
};
// monkey = name, job
std::map<std::string, job> mjobs;
// monkey = name, number
std::map<std::string, int64_t> mnums;

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
        disblk = true;
        return true;
    }
    // Check if block disable is ended
    if ((line[0] == '#') && (line.find("ENABLE") != std::string::npos))
    {
        disblk = false;
        return true;
    }
    // Check if test input
    if (!disblk && (line[0] == '#') && (line.find("TEST") != std::string::npos))
    {
        std::cout << "--- PROCESSING TEST INPUT ---" << std::endl;
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

        // Get name
        std::string mname = iline.substr(0, 4);

        // Check if this is a number monkey
        if ((iline[6] >= '0') && (iline[6] <= '9'))
        {
            mnums[mname] = std::stoi(iline.substr(6).c_str()); // Part 1: take number from input

            // Part 2: humn is you.
            // if (mname == "humn")
            // {
                // mnums["humn"] = 301;
            // }

            // if (TEST) std::cout << "  " << mname << " => " << mnums[mname] << std::endl; // @DEBUG
        }
        else
        {
            // Monkey does math
            mjobs[mname].result = INT_MIN;
            mjobs[mname].left = iline.substr(6, 4);
            mjobs[mname].op = iline[11];
            mjobs[mname].right = iline.substr(13, 4);

            // if (TEST) std::cout << "  " << mname << " => "
            //         << mjobs[mname].left
            //         << " " << mjobs[mname].op << " "
            //         << mjobs[mname].right
            //         << std::endl; // @DEBUG
        }
    }

    std::cout << "Inputs info: "
              << mnums.size() << " number monkeys, "
              << mjobs.size() << " math monkeys"
              << std::endl;
}

//-----------------------------------------------------------------------------

int64_t performOp(const std::string mname, job &job, std::string indent)
{
    int64_t left  = INT_MIN;
    int64_t right = INT_MIN;
    int64_t diff  = INT_MIN;

    if (TEST) std::cout << indent << mname << ": " << job.left
                        << " " << job.op << " "
                        << job.right << std::endl; // @DEBUG

    if (mnums.contains(job.left))
    {
        // Left is a number monkey
        left = mnums[job.left];
    }
    else
    {
        // Left is a math monkey. Get its result.
        left = performOp(job.left, mjobs[job.left], indent + "  ");
    }
    // if (TEST) std::cout << indent << "  left = " << left << std::endl; // @DEBUG

    if (mnums.contains(job.right))
    {
        // Right is a number monkey
        right = mnums[job.right];
    }
    else
    {
        // Right is a math monkey. Get its result.
        right = performOp(job.right, mjobs[job.right], indent + "  ");
    }
    // if (TEST) std::cout << indent << "  right = " << right << std::endl; // @DEBUG

    switch (job.op)
    {
    case '+':
        job.result = left + right;
        break;

    case '-':
        job.result = left - right;
        break;

    case '*':
        job.result = left * right;
        break;

    case '/':
        job.result = left / right;
        break;

    // Part 2: root checks if 2 numbers are equal
    case '=':
        job.result = (left == right);
        diff = (left - right);
        std::cout << indent << "*** DIFF = " << left << " - " << right << " = " << diff << std::endl; // @DEBUG
        break;

    default:
        break;
    }
    if (TEST) std::cout << indent << mname
            << " result = (" << left << " " << job.op << " " << right << ") = "
            << job.result << std::endl; // @DEBUG

    return job.result;
}

//-----------------------------------------------------------------------------

void part_1(void)
{
    std::cout << "Processing..." << std::endl;

    performOp("root", mjobs["root"], "");
    std::cout << "root: " << mjobs["root"].result << std::endl;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// Perform operation substitutions
std::string performSubst(const std::string mname, job &job)
{
    std::string left;
    std::string right;

    int64_t l;
    int64_t r;

    if (TEST) std::cout << mname << ": " << job.left
                        << " " << job.op << " "
                        << job.right << std::endl; // @DEBUG

    if (mnums.contains(job.left))
    {
        // Left is a number monkey
        if (job.left == "humn")
        {
            left = job.left;
        }
        else
        {
            left = std::to_string(mnums[job.left]);
            l = mnums[job.left];
        }
    }
    else
    {
        // Left is a math monkey. Get its result.
        left = performSubst(job.left, mjobs[job.left]);
    }
    if (TEST) std::cout << "  left = " << left << std::endl; // @DEBUG

    if (mnums.contains(job.right))
    {
        // Right is a number monkey
        if (job.right == "humn")
        {
            right = job.right;
        }
        else
        {
            right = std::to_string(mnums[job.right]);
            r = mnums[job.right];
        }
    }
    else
    {
        // Right is a math monkey. Get its result.
        right = performSubst(job.right, mjobs[job.right]);
    }
    if (TEST) std::cout << "  right = " << right << std::endl; // @DEBUG

    // ----------------------

    switch (job.op)
    {
    case '+':
        job.result = l + r;
        break;

    case '-':
        job.result = l - r;
        break;

    case '*':
        job.result = l * r;
        break;

    case '/':
        job.result = l / r;
        break;

    // Part 2: root checks if 2 numbers are equal
    case '=':
    {
        job.result = (left == right);
        int64_t diff = (l - r);
        std::cout << "*** DIFF = " << l << " - " << l << " = " << diff << std::endl; // @DEBUG
        break;
    }

    default:
        break;
    }

    // ----------------------

    std::string ret;
    if (job.left == "humn")
    {
        ret = "(humn ";
        ret.push_back(job.op);
        ret += " ";
        ret += std::to_string(r);
        ret += ")";
    }
    else if (job.right == "humn")
    {
        ret = "(";
        ret += std::to_string(l);
        ret += " ";
        ret.push_back(job.op);
        ret += " humn)";
    }
    else
    {
        ret = std::to_string(job.result);
    }
    // if (TEST) std::cout << ret << std::endl; // @DEBUG
    return ret;
}

//-----------------------------------------------------------------------------

// Assuming humn is in the left set of ops...
void part_2(void)
{
    std::cout << "Processing..." << std::endl;

    mjobs["root"].op = '=';

    std::cout << performSubst(mjobs["root"].left, mjobs[mjobs["root"].left])
              << std::endl;
    std::cout  << " = " << mjobs[mjobs["root"].right].result << std::endl;
}

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    std::cout << std::endl;
    //-------------------------------------------------------------------------

    loadInputs();

    std::cout << std::endl;
    part_1();

    std::cout << std::endl;
    part_2();

    //-------------------------------------------------------------------------

    std::cout << std::endl;
    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
