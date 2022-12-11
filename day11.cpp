//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

//-----------------------------------------------------------------------------

// static const char cInputFileName[] = "test.txt";
static const char cInputFileName[] = "input11.txt";

static std::ifstream infile(cInputFileName);

struct monkeySpec {
    uint num;
    std::vector<int64_t> items;
    std::string op;
    int64_t testDiv;
    uint trueToMonkey;
    uint falseToMonkey;
    uint numInspected;
    monkeySpec() : num(0), testDiv(1), trueToMonkey(0), falseToMonkey(0), numInspected(0) {};
};

static std::vector<monkeySpec *> monkeys;

static int64_t monkeyBusiness = 0;

static uint64_t LCM = 1;

//-----------------------------------------------------------------------------

void initMonkey(monkeySpec *m)
{
    m->num = 0;
    m->items.clear();
    m->op.clear();
    m->testDiv = 1;
    m->trueToMonkey = 0;
    m->falseToMonkey = 0;
    m->numInspected = 0;
}

void printMonkey(monkeySpec *m)
{
    std::cout << std::endl;
    std::cout << "Monkey Info: " << m->num << std::endl;
    std::cout << "Items:";
    for (auto i : m->items)
    {
        printf("%lld ", (long long)i);
    }
    std::cout << std::endl;
    std::cout << "Operation: " << m->op << std::endl;
    std::cout << "Test:  divisible by " << m->testDiv << std::endl;
    std::cout << "True:  throw to " << m->trueToMonkey << std::endl;
    std::cout << "False: throw to " << m->falseToMonkey << std::endl;
    std::cout << "# inspected: " << m->numInspected << std::endl;
}

void printMonkeyItems(void)
{
    for (auto m : monkeys)
    {
        std::cout << "Monkey " << m->num << " has " << m->items.size() << " items:";
        for (std::vector<int64_t>::iterator item = m->items.begin(); item != m->items.end(); ++item)
        {
            printf("%ld ", *item);
        }
        std::cout << std::endl;
    }
}

void printMonkeyInspections(void)
{
    std::vector<int64_t> insp;
    for (auto m : monkeys)
    {
        std::cout << "Monkey " << m->num << " inspected " << m->numInspected << " items" << std::endl;
        insp.push_back(m->numInspected);
    }

    std::sort(insp.begin(), insp.end(), std::greater<int64_t>());
    monkeyBusiness = insp[0] * insp[1];
    std::cout << "Monkey business: "
              << insp[0] << " * " << insp[1]
              << " = " << monkeyBusiness << std::endl;
}

//-----------------------------------------------------------------------------

void loadInputs(void)
{
    if (!infile)
    {
        std::cerr << "Error: unable to open input file: " << cInputFileName << std::endl;
        exit(1);
    }

    monkeySpec *monkey = new monkeySpec();
    initMonkey(monkey);
    int mnum = 0;

    std::cout << "Loading inputs..." << std::endl;

    std::string iline;
    while (!infile.eof())
    {
        getline(infile, iline);
        // Blank line = start new monkey
        if (iline.length() == 0)
        {
            // End monkey : store it.
            monkey->num = mnum++;
            monkeys.push_back(monkey);
            printMonkey(monkey);    // @DEBUG

            // Init new one
            monkey = new monkeySpec();
            initMonkey(monkey);
            continue;
        }
        // std::cout << iline << std::endl; // @DEBUG

        size_t fpos = 0;
        if (iline.find("Monkey") != std::string::npos)
        {
            // No real info here.
            continue;
        }
        else if ((fpos = iline.find("Starting items: ")) != std::string::npos)
        {
            fpos += 16;
            std::istringstream input(iline.substr(fpos));
            for (char a[8]; input.getline(a, 8, ',');)
            {
                monkey->items.push_back(std::atoi(a));
            }
        }
        else if ((fpos = iline.find("Operation: ")) != std::string::npos)
        {
            fpos += 11;
            monkey->op = iline.substr(fpos);
        }
        else if ((fpos = iline.find("Test: divisible by ")) != std::string::npos)
        {
            fpos += 19;
            monkey->testDiv = std::stoi(iline.substr(fpos));
            LCM *= monkey->testDiv;
        }
        else if ((fpos = iline.find("If true: throw to monkey ")) != std::string::npos)
        {
            fpos += 25;
            monkey->trueToMonkey = std::stoi(iline.substr(fpos));
        }
        else if ((fpos = iline.find("If false: throw to monkey ")) != std::string::npos)
        {
            fpos += 26;
            monkey->falseToMonkey = std::stoi(iline.substr(fpos));
        }
    }
}

//-----------------------------------------------------------------------------

int64_t permformOp(std::string op, int64_t item)
{
    // Note: operation slways starts with "new = old "
    std::istringstream inp(op.substr(10));

    std::string opern, sec;
    int64_t secnum = 0;
    int64_t result = 0;

    inp >> opern; // +, -, *, /
    inp >> sec; // a number or 'old'

    // Get number to work with
    if (sec.compare("old") == 0)
    {
        secnum = item;
    }
    else
    {
        secnum = std::stoi(sec);
    }

    // Process operation
    if (opern[0] == '+')
    {
        result = item + secnum;
    }
    else if (opern[0] == '-')
    {
        result = item - secnum;
    }
    else if(opern[0] == '*')
    {
        result = item * secnum;
    }
    else if (opern[0] == '/')
    {
        result = item / secnum;
    }
    return result;
}

//-----------------------------------------------------------------------------

void part_1(void)
{
    std::cout << "PART 1: Processing 20 rounds..." << std::endl;

    for (int round = 1; round <= 20; ++round)
    {
        std::cout << std::endl;
        std::cout << "===== Round " << round << " =====" << std::endl;

        for (uint mnum = 0; mnum < monkeys.size(); ++mnum)
        {
            monkeySpec *m = monkeys[mnum];

            // std::cout << "Monkey " << m->num << std::endl; // @DEBUG
            for (std::vector<int64_t>::iterator item = m->items.begin(); item != m->items.end();)
            {
                // Monkey inspects item
                m->numInspected++;

                // std::cout << "  item=" << *item; // @DEBUG

                // Compute new worry level.
                int64_t worry = permformOp(m->op, *item) / 3;
                // std::cout << " worry=" << worry; // @DEBUG

                if (std::fmod(worry, m->testDiv) == 0)
                {
                    // std::cout << " => " << m->trueToMonkey << std::endl; // @DEBUG
                    monkeys[m->trueToMonkey]->items.push_back(worry);
                }
                else
                {
                    // std::cout << " => " << m->falseToMonkey << std::endl; // @DEBUG
                    monkeys[m->falseToMonkey]->items.push_back(worry);
                }
                item = m->items.erase(m->items.begin());

            } // end item
        } // end monkey

        // End of round: Print monkeys' items
        printMonkeyItems();
    } // end round
}

//-----------------------------------------------------------------------------

void part_2(void)
{
    std::cout << "PART 2: Processing 10000 rounds..." << std::endl;

    for (int round = 1; round <= 10000; ++round)
    {
        if ((round == 1) || (round == 20) || ((round % 1000) == 0))
        {
            std::cout << std::endl;
            std::cout << "===== Round " << round << " =====" << std::endl;
        }
        for (uint mnum = 0; mnum < monkeys.size(); ++mnum)
        {
            monkeySpec *m = monkeys[mnum];

            // std::cout << "Monkey " << m->num << std::endl; // @DEBUG
            for (std::vector<int64_t>::iterator item = m->items.begin(); item != m->items.end();)
            {
                // Monkey inspects item
                m->numInspected++;

                // std::cout << "  item=" << *item; // @DEBUG

                // Compute new worry level.
                int64_t worry = permformOp(m->op, *item);
                // std::cout << " worry=" << worry; // @DEBUG

                worry = (worry % LCM); // @XXX

                // printf(" worry=%ld, testDiv=%d, fmod=%ld\n", worry, m->testDiv, fmod(worry, m->testDiv)); // @DEBUG
                if ((worry % m->testDiv) == 0)
                {
                    monkeys[m->trueToMonkey]->items.push_back(worry);
                }
                else
                {
                    monkeys[m->falseToMonkey]->items.push_back(worry);
                }
                item = m->items.erase(m->items.begin());

            } // end item
        }     // end monkey

        // End of round: Print inspection results
        if ((round == 1) || (round == 20) || ((round % 1000) == 0))
        {
            printMonkeyInspections();
        }
    } // end round
}

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    std::cout << std::endl;
    //-------------------------------------------------------------------------

    loadInputs();
    std::cout << std::endl;
    std::cout << "Inputs loaded; " << monkeys.size() << " monkeys loaded." << std::endl;
    printMonkeyItems();

    // std::cout << std::endl;
    // part_1();
    // std::cout << std::endl;
    // printMonkeyInspections();

    std::cout << std::endl;
    part_2();

    //-------------------------------------------------------------------------

    std::cout << std::endl;
    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
