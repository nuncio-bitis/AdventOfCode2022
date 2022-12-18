//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <utility>

//-----------------------------------------------------------------------------

// static const char cInputFileName[] = "test.txt";
static const char cInputFileName[] = "input16.txt";

static std::ifstream infile(cInputFileName);

struct node
{
    std::string name;
    int rate;
    std::vector<std::string> connects;
};
std::map<std::string, node> graph;

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
        if (ignoreLine(iline))
        {
            continue;
        }

        std::cout << " > " << iline << std::endl; // @DEBUG

        char nname[3];
        node thisNode;
        sscanf(iline.c_str(), "Valve %s has flow rate=%d; tunnels lead to valves", nname, &thisNode.rate);
        thisNode.name = nname;

        // Input connected nodes
        size_t pos = iline.find_last_of("valve");

        std::string consList = iline.substr(pos + 2);

        // replace all commas with a space
        std::replace(consList.begin(), consList.end(), ',', ' ');

        std::istringstream cons(consList);
        while (!cons.eof())
        {
            std::string nname;
            cons >> nname;
            if (nname.size() == 0)
            {
                break;
            }
            thisNode.connects.push_back(nname);
        }

        graph.emplace(std::make_pair(thisNode.name, thisNode));
    }

    std::cout << std::endl;
    std::cout << "Inputs info: " << std::endl;
    for (auto [name, node] : graph)
    {
        std::cout << name << std::endl;
        std::cout << "  Flow rate: " << node.rate << std::endl;
        std::cout << "  Connected nodes: ";
        for (auto cn : node.connects)
        {
            std::cout << cn << " ";
        }
        std::cout << std::endl;
    }
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
