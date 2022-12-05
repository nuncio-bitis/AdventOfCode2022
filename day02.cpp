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

//static const char cInputFileName[] = "test.txt";
static const char cInputFileName[] = "input2.txt";

//-----------------------------------------------------------------------------
// Part 1
namespace Part1 {

/*
A = X = Rock     = 1
B = Y = Paper    = 2
C = Z = Scissors = 3

Rock > Scissors
Scissors > Paper
Paper > Rock

Scoring:
  A win gets (value of choice) + 6
  A loss gets (value of choice)
  A tie gets (value of choice) + 3 for both players
Them You
  A   Y  => Rock vs Paper => You win,  You:(2 + 6) Them:(1 + 0)
  B   X  => Paper vs Rock => They win, You:(1 + 0) Them:(2 + 6)
  C   Z  => Scissors vs Scissors => Tie, You(3 + 3), Them:(3 + 3)
 */
static const std::map< std::string, std::tuple<int, int> > outcomes {
    {"A X", {(1+3), (1+3)}},
    {"A Y", {(1+0), (2+6)}},
    {"A Z", {(1+6), (3+0)}},
    {"B X", {(2+6), (1+0)}},
    {"B Y", {(2+3), (2+3)}},
    {"B Z", {(2+0), (3+6)}},
    {"C X", {(3+0), (1+6)}},
    {"C Y", {(3+6), (2+0)}},
    {"C Z", {(3+3), (3+3)}}
};

void part_1(void)
{
    std::cout << "Loading strategy guide..." << std::endl;

    std::ifstream infile(cInputFileName);
    if (!infile)
    {
        std::cerr << "Error: unable to open input file: " << cInputFileName << std::endl;
        return;
    }

    std::string play;
    int totalThem = 0;
    int totalYou = 0;

    while (!infile.eof())
    {
        getline(infile, play);

        // Skip blank line.
        if (play.length() == 0)
        {
            continue;
        }

        std::tuple<int, int> points = outcomes.at(play);

        totalThem += std::get<0>(points);
        totalYou += std::get<1>(points);

        play.clear();
    }
    infile.close();

    std::cout << "Totals:" << std::endl;
    std::cout << "Them: " << totalThem;
    std::cout << "\t You: " << totalYou;
    std::cout << std::endl;
}

} // end Part 1

//-----------------------------------------------------------------------------
// Part 2

namespace Part2 {

/*
A = Rock     = 1
B = Paper    = 2
C = Scissors = 3
X = Lose
Y = Draw
Z = Win

Rock > Scissors
Scissors > Paper
Paper > Rock

Scoring:
  A win gets (value of choice) + 6
  A loss gets (value of choice)
  A tie gets (value of choice) + 3 for both players
Them You
  A   Y  => Draw (Pick Rock vs Rock)     => (1+3) = 4
  B   X  => Lose (Pick Rock vs Paper)    => (1+0) = 1
  C   Z  => Win  (Pick Rock vs Scissors) => (1+6) = 7
  Your total: 12
 */
static const std::map< std::string, std::tuple<int, int> > outcomes {
    {"A X", {(1+6), (3+0)}}, // Lose against Rock => Scissors
    {"A Y", {(1+3), (1+3)}}, // Draw
    {"A Z", {(1+0), (2+6)}}, // Win against Rock => Paper
    {"B X", {(2+6), (1+0)}}, // Lose against Paper => Rock
    {"B Y", {(2+3), (2+3)}}, // Draw
    {"B Z", {(2+0), (3+6)}}, // Win against Paper => Scissors
    {"C X", {(3+6), (2+0)}}, // Lose against Scissors => Paper
    {"C Y", {(3+3), (3+3)}}, // Draw
    {"C Z", {(3+0), (1+6)}}  // Win against Scissors => Rock
};

void part_2(void)
{
    std::cout << "Loading strategy guide..." << std::endl;

    std::ifstream infile(cInputFileName);
    if (!infile)
    {
        std::cerr << "Error: unable to open input file: " << cInputFileName << std::endl;
        return;
    }

    std::string play;
    int totalThem = 0;
    int totalYou = 0;

    while (!infile.eof())
    {
        getline(infile, play);

        // Skip blank line.
        if (play.length() == 0)
        {
            continue;
        }

        std::tuple<int, int> points = outcomes.at(play);

        totalThem += std::get<0>(points);
        totalYou += std::get<1>(points);

        play.clear();
    }
    infile.close();

    std::cout << "Totals:" << std::endl;
    std::cout << "Them: " << totalThem;
    std::cout << "\t You: " << totalYou;
    std::cout << std::endl;
}

} // end Part 2

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    std::cout << std::endl;
    //-------------------------------------------------------------------------

    Part1::part_1();

    std::cout << std::endl;

    Part2::part_2();

    //-------------------------------------------------------------------------

    std::cout << std::endl;
    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
